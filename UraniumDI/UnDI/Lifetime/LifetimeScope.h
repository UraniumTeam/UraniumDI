#pragma once
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/Memory/Memory.h>

namespace UN::DI
{
    class LifetimeScope : public Object<ILifetimeScope>
    {
        bool m_IsAlive;

        VoidResult<ErrorCode> EndImpl();

    public:
        UN_RTTI_Class(LifetimeScope, "F1888F2D-89A7-4617-8C5D-BB03F15260A9");

        ~LifetimeScope() override;

        Result<ILifetimeScope*, ErrorCode> BeginScope() override;
        VoidResult<ErrorCode> End() override;
    };
} // namespace UN::DI
