#pragma once
#include <UnDI/RTTI/RTTI.h>
#include <UnDI/ServiceActivator.h>
#include <functional>

namespace UN::DI
{
    enum class ServiceLifetime
    {
        Singleton,
        Scoped,
        Transient
    };

    struct ServiceRegistration final
    {
        UUID ID;
        ServiceActivator Activator;
        ServiceLifetime Lifetime;

        UN_RTTI_Struct(ServiceRegistration, "00BFB3D7-E5E0-4A62-A716-B6668B79B771");
    };
} // namespace UN::DI
