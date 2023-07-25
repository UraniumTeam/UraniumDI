#pragma once
#include <UnDI/RTTI/RTTI.h>
#include <UnDI/Memory/ReferenceCounter.h>

namespace UN
{
    //! \brief Base interface for dynamic reference counted objects.
    class IObject
    {
    public:
        UN_RTTI_Class(IObject, "C2ED715F-3362-41C5-8FDD-F951E67F0D2B");

        virtual ~IObject() = default;

        //! \brief Add a strong reference to object's reference counter.
        //!
        //! \return The new (incremented) value of strong reference counter.
        virtual UInt32 AddRef() = 0;

        //! \brief Remove a strong reference from object's reference counter.
        //!
        //! If reference counter reaches zero the object will commit suicide by deallocating
        //! it's own memory and calling it's own destructor.
        //!
        //! \return The new (decremented) value of strong reference counter.
        virtual UInt32 Release() = 0;

        //! \brief Attach a ReferenceCounter to this object.
        //!
        //! This function must be called right after the object was allocated. It must provide the pointer to
        //! reference counter created exclusively for this object.
        //!
        //! \param counter - The reference counter to attach to this object.
        virtual void AttachRefCounter(ReferenceCounter* counter) = 0;

        //! \brief Get reference counter that belongs to this object.
        //!
        //! \return The attached reference counter.
        virtual ReferenceCounter* GetRefCounter() = 0;
    };

    //! \brief Base class for dynamic reference counted objects.
    //!
    //! Example:
    //! \code{.cpp}
    //!     class IObject {};
    //!     class IMyInterface : public IObject {};
    //!     class MyObject  : public Object<IMyInterface> {};
    //! \endcode
    //!
    //! \tparam TInterface - The type of interface the object must implement.
    template<class TInterface, std::enable_if_t<std::is_base_of_v<IObject, TInterface>, bool> = true>
    class Object : public TInterface
    {
        ReferenceCounter* m_RefCounter = nullptr;

    protected:
        template<class F>
        inline UInt32 Release(F&& destroyCallback)
        {
            return m_RefCounter->ReleaseStrongRef([this, &destroyCallback] {
                destroyCallback();
                this->~Object();
            });
        }

    public:
        UN_RTTI_Class(Object<TInterface>, "5617B2CC-C963-45D5-8191-F14A06420954");

        Object() = default;

        virtual ~Object() = default;

        Object(const Object&) = delete;
        Object(Object&&)      = delete;

        //! \brief Add a strong reference to the object.
        //!
        //! \return The new (incremented) number of strong references.
        inline UInt32 AddRef() override
        {
            return m_RefCounter->AddStrongRef();
        }

        //! Directly uses ReferenceCounter::ReleaseStrongRef, but also calls the virtual destructor
        //! of itself (commits suicide) when the reference counter reaches zero.
        inline UInt32 Release() override
        {
            return Release([] {});
        }

        //! \brief Attach a reference counter to the object.
        //!
        //! \param pRefCounter - Reference counter to attach.
        inline void AttachRefCounter(ReferenceCounter* pRefCounter) override
        {
            m_RefCounter = pRefCounter;
        }

        //! \brief Get reference counter attached to this object.
        inline ReferenceCounter* GetRefCounter() override
        {
            return m_RefCounter;
        }
    };
} // namespace UN
