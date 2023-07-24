#include <UnDI/Lifetime/LifetimeScope.h>
#include <UnDI/Lifetime/ILifetimeScope.h>

namespace UN::DI
{
    Result<ILifetimeScope*, ErrorCode> DI::LifetimeScope::BeginScope()
    {
        return Err(ErrorCode::NotImplemented);
    }

    LifetimeScope::~LifetimeScope()
    {
        if (!m_IsAlive)
        {
            return;
        }

        EndImpl().Unwrap();
    }

    VoidResult<ErrorCode> LifetimeScope::End()
    {
        if (!m_IsAlive)
        {
            return Err(ErrorCode::InvalidOperation);
        }

        return EndImpl();
    }

    VoidResult<ErrorCode> LifetimeScope::EndImpl()
    {
        return Err(ErrorCode::NotImplemented);
    }
} // namespace UN::DI
