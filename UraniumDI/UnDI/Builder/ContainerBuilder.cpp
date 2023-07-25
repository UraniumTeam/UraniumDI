#include <UnDI/Builder/ContainerBuilder.h>
#include <UnDI/Container.h>

namespace UN::DI
{
    IContainer* ContainerBuilder::Build()
    {
        return AllocateObject<Container>(m_Builder.Build());
    }
} // namespace UN::DI
