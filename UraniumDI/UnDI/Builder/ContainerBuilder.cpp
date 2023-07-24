#include <UnDI/Builder/ContainerBuilder.h>

namespace UN::DI
{
    Result<IContainer*, ErrorCode> ContainerBuilder::Build()
    {
        return Err(ErrorCode::NotImplemented);
    }
} // namespace UN::DI
