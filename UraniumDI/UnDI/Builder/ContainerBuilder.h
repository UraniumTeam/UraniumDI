#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/IContainer.h>
#include <UnDI/Memory/Memory.h>

namespace UN::DI
{
    class ContainerBuilder final
    {
    public:
        UN_RTTI_Struct(ContainerBuilder, "812AF89B-8158-4831-9F11-18DBC5D81124");
        Result<IContainer*, ErrorCode> Build();
    };
} // namespace UN::DI
