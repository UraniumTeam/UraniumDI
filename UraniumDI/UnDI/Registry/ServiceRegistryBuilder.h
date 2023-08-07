#pragma once
#include <UnDI/ServiceRegistration.h>
#include <UnTL/Containers/List.h>

namespace UN::DI
{
    class ServiceRegistryBuilder;
    class IServiceRegistry;

    namespace Internal
    {
        class RegistryToBuilder
        {
            template<class TInterface>
            friend class RegistryBindBuilder;
            ServiceRegistration* m_pTarget;

            inline explicit RegistryToBuilder(ServiceRegistration* registration)
                : m_pTarget(registration)
            {
            }

        public:
            inline void InScope(ServiceLifetime lifetime)
            {
                m_pTarget->Lifetime = lifetime;
            }

            inline void InSingletonScope()
            {
                InScope(ServiceLifetime::Singleton);
            }

            inline void InTransientScope()
            {
                InScope(ServiceLifetime::Transient);
            }
        };

        template<class TInterface>
        class RegistryBindBuilder
        {
            friend class ::UN::DI::ServiceRegistryBuilder;
            ServiceRegistration* m_pTarget;

            inline explicit RegistryBindBuilder(ServiceRegistration* registration)
                : m_pTarget(registration)
            {
            }

        public:
            template<class TImpl>
            inline RegistryToBuilder To()
            {
                m_pTarget->Activator = ServiceActivator::CreateForType<TImpl>();
                return RegistryToBuilder(m_pTarget);
            }

            inline RegistryToBuilder ToFunc(ActivatorFunc function)
            {
                m_pTarget->Activator = ServiceActivator::CreateForFunc(function);
                m_pTarget->Flags |= ServiceRegistrationFlags::Function;
                return RegistryToBuilder(m_pTarget);
            }

            inline void ToConst(TInterface* pConst)
            {
                auto factory = [pConst](IServiceProvider*) -> Result<IObject*, ErrorCode> {
                    return static_cast<IObject*>(pConst);
                };

                m_pTarget->Activator = ServiceActivator::CreateForFunc(factory);
                m_pTarget->Lifetime  = ServiceLifetime ::Singleton;
                m_pTarget->Flags |= ServiceRegistrationFlags::Constant;
                pConst->AddRef();
            }

            inline RegistryToBuilder ToSelf()
            {
                m_pTarget->Activator = ServiceActivator::CreateForType<TInterface>();
                return RegistryToBuilder(m_pTarget);
            }
        };
    } // namespace Internal

    class ServiceRegistryBuilder final
    {
        List<ServiceRegistration> m_Registrations;

    public:
        UN_RTTI_Struct(ServiceRegistryBuilder, "55690DA9-4065-4E60-9438-30016CF4CDEC");

        [[nodiscard]] VoidResult<ErrorCode> Register(ServiceRegistration&& registration);
        [[nodiscard]] IServiceRegistry* Build();

        template<class TInterface>
        [[nodiscard]] Internal::RegistryBindBuilder<TInterface> Bind()
        {
            auto& target    = m_Registrations.Emplace();
            auto id         = un_typeid<TInterface>();
            target.ID       = id;
            target.Lifetime = ServiceLifetime::Scoped;
            target.Flags    = ServiceRegistrationFlags::None;
            return Internal::RegistryBindBuilder<TInterface>(&target);
        }
    };
} // namespace UN::DI
