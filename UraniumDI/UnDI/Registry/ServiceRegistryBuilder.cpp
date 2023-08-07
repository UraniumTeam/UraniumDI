#include <UnDI/Registry/ServiceRegistryBuilder.h>
#include <UnDI/Registry/ServiceRegistry.h>

namespace UN::DI
{
    VoidResult<ErrorCode> ServiceRegistryBuilder::Register(ServiceRegistration&& registration)
    {
        // TODO: check for duplicates
        m_Registrations.Push(std::move(registration));
        return {};
    }

    IServiceRegistry* ServiceRegistryBuilder::Build()
    {
        m_Registrations.SortByMember(&ServiceRegistration::ID);
        return AllocateObject<ServiceRegistry>(std::move(m_Registrations));
    }
} // namespace UN::DI
