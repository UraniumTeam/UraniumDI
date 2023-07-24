#pragma once
#include <UnDI/ServiceActivator.h>
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/RTTI/RTTI.h>
#include <functional>

namespace UN::DI
{
    struct ServiceRegistration
    {
        UUID Id;
        ServiceActivator Activator;
        ILifetimeScope* pLifetime;
    };
} // namespace UN::DI
