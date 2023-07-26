#pragma once
#include <UnTL/Base/Base.h>
#include <UnDI/IContainer.h>
#include <UnDI/Registry/ServiceRegistryBuilder.h>

namespace UN::DI
{
    class ContainerBuilder final
    {
        ServiceRegistryBuilder m_Builder;

    public:
        UN_RTTI_Struct(ContainerBuilder, "812AF89B-8158-4831-9F11-18DBC5D81124");

        template<class TInterface>
        [[nodiscard]] inline Internal::RegistryBindBuilder<TInterface> Bind()
        {
            return m_Builder.Bind<TInterface>();
        }

        IContainer* Build();
    };
} // namespace UN::DI
