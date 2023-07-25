#pragma once
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/Memory/Memory.h>
#include <unordered_map>

namespace UN::DI
{
    class IServiceRegistry;
    struct ServiceRegistration;

    class LifetimeScope : public Object<ILifetimeScope>
    {
        // TODO: use IAllocator for this map
        std::unordered_map<UUID, IObject*> m_ServiceCache;

        LifetimeScope* m_pParentScope;
        LifetimeScope* m_pRootScope;
        IServiceRegistry* m_pRegistry;
        bool m_IsValid;

        void EndImpl();

        Result<IObject*, ErrorCode> ActivateImpl(ServiceRegistration* registration);
        Result<IObject*, ErrorCode> ResolveImpl(ServiceRegistration* registration);

    public:
        UN_RTTI_Class(LifetimeScope, "F1888F2D-89A7-4617-8C5D-BB03F15260A9");

        ~LifetimeScope() override;

        inline explicit LifetimeScope(IServiceRegistry* pRegistry)
            : m_pParentScope(nullptr)
            , m_pRootScope(this)
            , m_pRegistry(pRegistry)
            , m_IsValid(true)
        {
        }

        inline LifetimeScope(IServiceRegistry* pRegistry, LifetimeScope* pParentScope, LifetimeScope* pRootScope)
            : m_pParentScope(pParentScope)
            , m_pRootScope(pRootScope)
            , m_pRegistry(pRegistry)
            , m_IsValid(true)
        {
        }

        Result<IObject*, ErrorCode> Resolve(const UUID& registrationID) override;
        Result<ILifetimeScope*, ErrorCode> BeginScope() override;
        VoidResult<ErrorCode> End() override;
    };
} // namespace UN::DI
