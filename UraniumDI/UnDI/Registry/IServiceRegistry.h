#pragma once
#include <UnDI/Memory/Memory.h>
#include <UnDI/ServiceRegistration.h>

namespace UN::DI
{
    class IServiceRegistry : public IObject
    {
    public:
        UN_RTTI_Class(IServiceRegistry, "3DB975F2-F1EE-4F7C-86F1-068A000FFD91");

        virtual Result<ServiceRegistration*, ErrorCode> GetRegistration(const UUID& uuid) = 0;
    };
} // namespace UN::DI
