#include <UnDI/Container.h>

namespace UN::DI
{
    ILifetimeScope* Container::GetRootScope() const
    {
        return m_pRootScope.Get();
    }

    Result<IObject*, ErrorCode> Container::Resolve(const UUID& registrationID)
    {
        return m_pRootScope->Resolve(registrationID);
    }

    Result<ILifetimeScope*, ErrorCode> Container::BeginScope()
    {
        return m_pRootScope->BeginScope();
    }
} // namespace UN::DI
