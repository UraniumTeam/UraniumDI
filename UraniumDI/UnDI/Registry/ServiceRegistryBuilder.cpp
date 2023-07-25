#include <UnDI/Registry/ServiceRegistryBuilder.h>
#include <UnDI/Registry/ServiceRegistry.h>

namespace UN::DI
{
    VoidResult<ErrorCode> ServiceRegistryBuilder::Register(const ServiceRegistration& registration)
    {
        // TODO: check for duplicates
        m_Registrations.Push(registration);
        return {};
    }

    IServiceRegistry* ServiceRegistryBuilder::Build()
    {
        return AllocateObject<ServiceRegistry>(std::move(m_Registrations));
    }
} // namespace UN::DI
