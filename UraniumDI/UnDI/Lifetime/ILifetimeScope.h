#pragma once
#include <UnDI/IServiceProvider.h>
#include <UnTL/Memory/Memory.h>
#include <UnTL/Utils/Result.h>
#include <UnDI/Utils/ErrorCode.h>

namespace UN::DI
{
    class ILifetimeScope : public IServiceProvider
    {
    public:
        UN_RTTI_Class(ILifetimeScope, "2916B8E9-603A-4BD6-A675-0DC6C43F9267");

        virtual VoidResult<ErrorCode> End() = 0;
    };
} // namespace UN::DI
