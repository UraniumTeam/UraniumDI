#pragma once
#include <UnDI/RTTI/RTTI.h>
#include <UnDI/Memory/IAllocator.h>

namespace UN
{
    //! \brief This allocator uses platform-specific aligned versions of malloc() and free().
    class SystemAllocator : public IAllocator
    {
        static SystemAllocator m_Instance;

    public:
        UN_RTTI_Class(SystemAllocator, "6C2B53D6-3A9A-447F-A127-1052253189C4");

        void* Allocate(USize size, USize alignment) override;
        void Deallocate(void* pointer) override;
        [[nodiscard]] const char* GetName() const override;

        //! \brief Get global static instance of the system allocator.
        inline static SystemAllocator* Get()
        {
            return &m_Instance;
        }
    };

    inline SystemAllocator SystemAllocator::m_Instance;

    inline void* SystemAllocator::Allocate(USize size, USize alignment)
    {
        return UN_AlignedMalloc(size, alignment);
    }

    inline void SystemAllocator::Deallocate(void* pointer)
    {
        return UN_AlignedFree(pointer);
    }

    inline const char* SystemAllocator::GetName() const
    {
        return "System allocator";
    }
} // namespace UN
