#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/IContainer.h>
#include <UnDI/Memory/Memory.h>

namespace UN::DI
{
    class ContainerBuilder final
    {
    public:
        Result<IContainer*, ErrorCode> Build();
    };
} // namespace UN::DI
