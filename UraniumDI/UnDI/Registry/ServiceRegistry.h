#pragma once
#include <UnTL/Containers/List.h>
#include <UnDI/Registry/IServiceRegistry.h>
#include <UnDI/ServiceRegistration.h>

namespace UN::DI
{
    class ServiceRegistry : public Object<IServiceRegistry>
    {
        List<ServiceRegistration> m_Registrations;

    public:
        UN_RTTI_Class(ServiceRegistry, "A66D1FC1-9792-4DBB-852F-C7F0E87ABB14");

        ~ServiceRegistry() override = default;

        explicit ServiceRegistry(List<ServiceRegistration>&& registrations)
            : m_Registrations(std::move(registrations))
        {
        }

        Result<ServiceRegistration*, ErrorCode> GetRegistration(const UUID& uuid) override;
    };
} // namespace UN::DI
