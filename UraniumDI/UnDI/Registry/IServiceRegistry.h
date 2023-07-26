#pragma once
#include <UnTL/Memory/Memory.h>
#include <UnTL/Utils/Result.h>
#include <UnDI/Utils/ErrorCode.h>

namespace UN::DI
{
    struct ServiceRegistration;

    class IServiceRegistry : public IObject
    {
    public:
        UN_RTTI_Class(IServiceRegistry, "3DB975F2-F1EE-4F7C-86F1-068A000FFD91");

        virtual Result<ServiceRegistration*, ErrorCode> GetRegistration(const UUID& uuid) = 0;
    };
} // namespace UN::DI
