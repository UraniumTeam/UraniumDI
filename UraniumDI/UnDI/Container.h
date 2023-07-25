#pragma once
#include <UnDI/IContainer.h>
#include <UnDI/Lifetime/LifetimeScope.h>
#include <UnDI/Registry/IServiceRegistry.h>
#include <UnDI/RTTI/RTTI.h>

namespace UN::DI
{
    class Container : public Object<IContainer>
    {
        Ptr<LifetimeScope> m_pRootScope;
        Ptr<IServiceRegistry> m_pRegistry;

    public:
        UN_RTTI_Class(Container, "53A3EFC1-552E-47F4-9E0C-3DB11932316D");

        inline explicit Container(IServiceRegistry* pRegistry)
            : m_pRegistry(pRegistry)
        {
            m_pRootScope = AllocateObject<LifetimeScope>(pRegistry);
        }

        ~Container() override = default;

        Result<ILifetimeScope*, ErrorCode> BeginScope() override;
        [[nodiscard]] ILifetimeScope* GetRootScope() const override;
        Result<IObject*, ErrorCode> Resolve(const UUID& registrationID) override;
    };
} // namespace UN::DI
