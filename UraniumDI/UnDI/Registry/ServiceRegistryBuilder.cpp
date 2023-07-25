#include <UnDI/Registry/ServiceRegistryBuilder.h>
#include <UnDI/Registry/ServiceRegistry.h>

namespace UN::DI
{
    VoidResult<ErrorCode> ServiceRegistryBuilder::Register(const ServiceRegistration& registration)
    {
        (void)registration;
        return Err(ErrorCode::NotImplemented);
    }

    IServiceRegistry* ServiceRegistryBuilder::Build()
    {
        return AllocateObject<ServiceRegistry>(std::move(m_Registrations));
    }
} // namespace UN::DI
