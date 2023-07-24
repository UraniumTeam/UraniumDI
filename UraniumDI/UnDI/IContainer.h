#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/Memory/Memory.h>

namespace UN::DI
{
    class IContainer : public ILifetimeScope
    {
    public:
        UN_RTTI_Class(IContainer, "C8321BA9-205F-4DFB-8339-DE405337F5CC");

        virtual ILifetimeScope* GetRootScope() = 0;
    };
} // namespace UN::DI
