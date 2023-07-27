#pragma once
#include <UnTL/Memory/Object.h>

namespace UN::DI
{
    class ConfigurationSectionBuilder;

    class IConfigurationRegistry : public IObject
    {
    public:
        virtual void Load(ConfigurationSectionBuilder& builder) = 0;
    };
} // namespace UN::DI
