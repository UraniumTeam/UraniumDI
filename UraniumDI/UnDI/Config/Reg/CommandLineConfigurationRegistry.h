#pragma once
#include <UnDI/Config/IConfigurationRegistry.h>
#include <UnTL/Strings/String.h>

namespace UN::DI
{
    class CommandLineConfigurationRegistry final : public Object<IConfigurationRegistry>
    {
        List<String> m_Data;

    public:
        CommandLineConfigurationRegistry(int argc, char** argv);
        void Load(ConfigurationSectionBuilder& builder) override;
    };
} // namespace UN::DI
