#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/Memory/IAllocator.h>
#include <UnDI/RTTI/RTTI.h>

namespace UN
{
    class IObject;

    //! \brief The reference counter that holds the number of references to the object.
    //!
    //! This class holds number of references to the object and a pointer to the allocator that
    //! was used to allocate this object. It assumes the following memory layout:
    //! \code
    //!     +------------------+ <--- this pointer
    //!     | ReferenceCounter |
    //!     --------------------
    //!     |      Object      |
    //!     +------------------+
    //! \endcode
    //!
    //! It will delete `this` assuming that a single block was used to allocate the object and the counter.\n
    //!
    //! Example (pseudo-code):
    //! \code{.cpp}
    //!     class MyObject : Object<IObject> {};
    //!
    //!     ReferenceCounter* rc = malloc(sizeof(ReferenceCounter) + sizeof(MyObject));
    //!     MyObject* obj        = rc + sizeof(ReferenceCounter);
    //!     // ...
    //!     free(rc); // frees memory of both the counter and the object.
    //! \endcode
    //!
    //! This layout is used for better locality and performance: it groups two allocations into one.
    //! The internal reference counting system also supports copying shared pointers using their raw pointers:
    //! \code{.cpp}
    //!     Ptr<MyObject> pObj1 = AllocateObject<MyObject>(); // refcount = 1
    //!     Ptr<MyObject> pObj2 = pObj1;                      // refcount = 2 <-- Valid for std::shared_ptr too
    //!     Ptr<MyObject> pObj3 = pObj1.Get();                // refcount = 3 <-- Also valid here!
    //! \endcode
    class ReferenceCounter final
    {
        std::atomic<Int32> m_StrongRefCount;
        mutable IAllocator* m_pAllocator;

    public:
        UN_RTTI_Struct(ReferenceCounter, "CEDB61B5-D75B-4D08-BE6F-E1C11806989B");

        //! \brief Create a new reference counter with specified allocator.
        //!
        //! The specified allocator will be used to free memory after the counter reaches zero.
        //! This constructor initializes the counter to _zero_.
        //!
        //! \param pAllocator - The allocator to use to free memory.
        inline explicit ReferenceCounter(IAllocator* pAllocator)
            : m_StrongRefCount(0)
            , m_pAllocator(pAllocator)
        {
        }

        //! \brief Add a strong reference to the counter.
        //!
        //! \return The new (incremented) number of strong references.
        inline UInt32 AddStrongRef()
        {
            return ++m_StrongRefCount;
        }

        //! \brief Remove a strong reference from the counter.
        //!
        //! This function will delete the counter itself if number of references reaches zero.
        //!
        //! \param destroyCallback - A function to invoke _before_ deallocation if the counter reached zero.
        //!                          This is typically a lambda that calls object destructor.
        //! \tparam F - Type of callback function.
        //!
        //! \return The new (decremented) number of strong references.
        template<class F>
        inline UInt32 ReleaseStrongRef(F&& destroyCallback)
        {
            if (--m_StrongRefCount == 0)
            {
                destroyCallback();
                m_pAllocator->Deallocate(this);
            }

            return m_StrongRefCount;
        }

        //! \brief Get number of strong references.
        inline UInt32 GetStrongRefCount() const
        {
            return m_StrongRefCount;
        }
    };
} // namespace UN
