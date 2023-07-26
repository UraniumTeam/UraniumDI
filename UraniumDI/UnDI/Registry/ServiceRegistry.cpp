#include <UnDI/Registry/ServiceRegistry.h>

namespace UN::DI
{
    Result<ServiceRegistration*, ErrorCode> ServiceRegistry::GetRegistration(const UUID& uuid)
    {
        auto iter = std::lower_bound(m_Registrations.begin(), m_Registrations.end(), uuid, [](auto reg, auto id) {
            return reg.ID < id;
        });

        if (iter != m_Registrations.end() && iter->ID == uuid)
        {
            return iter;
        }

        return Err(ErrorCode::NotFound);
    }
} // namespace UN::DI
