#include <UnDI/Registry/ServiceRegistry.h>

namespace UN::DI
{
    Result<ServiceRegistration*, ErrorCode> ServiceRegistry::GetRegistration(const UUID& uuid)
    {
        for (auto& item : m_Registrations)
        {
            if (item.ID == uuid)
            {
                return &item;
            }
        }

        return Err(ErrorCode::NotFound);
    }
} // namespace UN::DI
