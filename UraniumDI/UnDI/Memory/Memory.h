#pragma once
#include <UnDI/RTTI/RTTI.h>
#include <UnDI/Memory/Ptr.h>
#include <UnDI/Memory/SystemAllocator.h>

namespace UN
{
    //! \brief Create a reference counted object.
    //!
    //! This function allocates storage for a ReferenceCounter and an object of type T.
    //! It attaches reference counter to the allocated object.
    //!
    //! \param pAllocator - Allocator to use for allocation and deallocation of the object.
    //! \param args       - Arguments to call constructor of T with.
    //!
    //! \tparam T          - Type of object to allocate.
    //! \tparam TAllocator - Type of allocator to use for allocation and deallocation of the object.
    //! \tparam Args       - Types of arguments to call constructor of T with.
    //!
    //! \return The created object.
    template<class T, class TAllocator, class... Args>
    inline T* AllocateObjectEx(TAllocator* pAllocator, Args&&... args)
    {
        USize counterSize = AlignUp(sizeof(ReferenceCounter), alignof(T));
        USize wholeSize   = sizeof(T) + counterSize;

        auto* ptr     = static_cast<UInt8*>(pAllocator->Allocate(wholeSize, alignof(T)));
        auto* counter = new (ptr) ReferenceCounter(pAllocator);

        T* object = new (ptr + counterSize) T(std::forward<Args>(args)...);
        object->AttachRefCounter(counter);
        return object;
    }

    //! \brief Create a reference counted object.
    //!
    //! This function allocates storage for a ReferenceCounter and an object of type T.
    //! It attaches reference counter to the allocated object.
    //!
    //! \param args - Arguments to call constructor of T with.
    //!
    //! \tparam T    - Type of object to allocate.
    //! \tparam Args - Types of arguments to call constructor of T with.
    //!
    //! \return The created object.
    template<class T, class... Args>
    inline T* AllocateObject(Args&&... args)
    {
        return AllocateObjectEx<T, SystemAllocator>(SystemAllocator::Get(), std::forward<Args>(args)...);
    }
} // namespace UN
