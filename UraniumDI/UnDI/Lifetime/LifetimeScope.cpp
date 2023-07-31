#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/Lifetime/LifetimeScope.h>
#include <UnDI/Registry/IServiceRegistry.h>
#include <UnDI/ServiceRegistration.h>

namespace UN::DI
{
    Result<ILifetimeScope*, ErrorCode> DI::LifetimeScope::BeginScope()
    {
        UN_Guard(m_IsValid, ErrorCode::InvalidOperation);
        return AllocateObject<LifetimeScope>(m_pRegistry, this, m_pRootScope);
    }

    LifetimeScope::~LifetimeScope()
    {
        if (!m_IsValid)
        {
            return;
        }

        EndImpl();
    }

    VoidResult<ErrorCode> LifetimeScope::End()
    {
        UN_Guard(m_IsValid, ErrorCode::InvalidOperation);
        EndImpl();
        return {};
    }

    void LifetimeScope::EndImpl()
    {
        for (auto& item : m_ServiceCache)
        {
            item.second->Release();
        }

        m_ServiceCache.clear();
    }

    Result<IObject*, ErrorCode> LifetimeScope::Resolve(const UUID& registrationID)
    {
        if (registrationID == un_typeid<IServiceProvider>())
        {
            return this;
        }

        auto result = m_pRegistry->GetRegistration(registrationID);
        UN_Guard(result, result.UnwrapErr());
        auto* registration = result.Unwrap();

        switch (registration->Lifetime)
        {
        case ServiceLifetime::Singleton:
            return m_pRootScope->ResolveImpl(registration);
        case ServiceLifetime::Scoped:
            return ResolveImpl(registration);
        case ServiceLifetime::Transient:
            return ActivateImpl(registration);
        default:
            return Err(ErrorCode::InvalidArguments);
        }
    }

    Result<IObject*, ErrorCode> LifetimeScope::ResolveImpl(ServiceRegistration* registration)
    {
        auto iter = m_ServiceCache.find(registration->ID);
        if (iter != m_ServiceCache.end())
        {
            return iter->second;
        }

        auto result = ActivateImpl(registration);
        if (result)
        {
            auto* obj = result.Unwrap();
            obj->AddRef();
            m_ServiceCache[registration->ID] = obj;
        }

        return result;
    }

    Result<IObject*, ErrorCode> LifetimeScope::ActivateImpl(ServiceRegistration* registration)
    {
        return registration->Activator.Invoke(this);
    }
} // namespace UN::DI
