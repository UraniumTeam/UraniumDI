#pragma once
#include <UnDI/ServiceActivator.h>
#include <UnTL/Base/Flags.h>
#include <UnTL/RTTI/RTTI.h>
#include <functional>

namespace UN::DI
{
    //! \brief Specifies the lifetime of an injectable service.
    enum class ServiceLifetime : Int8
    {
        Singleton, //!< Specifies that only a single instance of the service will be created.
        Scoped,    //!< Specifies that an instance of the service will be created for each ILifetimeScope.
        Transient  //!< Specifies that an instance of the service will be created for each call to IServiceProvider::Resolve.
    };

    //! \brief Specifies injectable service registration flags.
    enum class ServiceRegistrationFlags : Int8
    {
        None     = 0,         //!< Specifies an empty value.
        Constant = UN_BIT(0), //!< Specifies that the service was registered with a constant value.
        Function = UN_BIT(1)  //!< Specifies that the service was registered with a factory function.
    };

    UN_ENUM_OPERATORS(ServiceRegistrationFlags)

    //! \brief Represents a registered injectable service.
    struct ServiceRegistration final
    {
        UUID ID;                        //!< Service interface ID.
        ServiceActivator Activator;     //!< ServiceActivator used to create an instance of the registered service.
        ServiceLifetime Lifetime;       //!< ServiceLifetime associated with the registered service.
        ServiceRegistrationFlags Flags; //!< ServiceRegistrationFlags associated with the registration.

        UN_RTTI_Struct(ServiceRegistration, "00BFB3D7-E5E0-4A62-A716-B6668B79B771");

        inline ServiceRegistration()
            : Lifetime(ServiceLifetime::Singleton)
            , Flags(ServiceRegistrationFlags::None)
        {
        }

        ServiceRegistration(const ServiceRegistration&)            = delete;
        ServiceRegistration& operator=(const ServiceRegistration&) = delete;

        inline ServiceRegistration(ServiceRegistration&& other) noexcept
            : ID(other.ID)
            , Activator(other.Activator)
            , Lifetime(other.Lifetime)
            , Flags(other.Flags)
        {
            other.ID        = {};
            other.Activator = {};
            other.Lifetime  = {};
            other.Flags     = {};
        }

        inline ServiceRegistration& operator=(ServiceRegistration&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            ID        = other.ID;
            Activator = other.Activator;
            Lifetime  = other.Lifetime;
            Flags     = other.Flags;

            other.ID        = {};
            other.Activator = {};
            other.Lifetime  = {};
            other.Flags     = {};
            return *this;
        }

        inline ~ServiceRegistration()
        {
            if (AllFlagsActive(Flags, ServiceRegistrationFlags::Constant))
            {
                // Constant activator function just returns a constant value, hence does not use the service provider.
                Activator.Invoke(nullptr).Unwrap()->Release();
            }
        }
    };
} // namespace UN::DI
