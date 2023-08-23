#include <UnDI/Config/ConfigurationBuilder.h>
#include <UnDI/Config/Reg/CommandLineConfigurationRegistry.h>

namespace UN::DI
{
    CommandLineConfigurationRegistry::CommandLineConfigurationRegistry(int argc, char** argv)
    {
        for (int i = 1; i < argc; ++i)
        {
            m_Data.Push(argv[i]);
        }
    }

    void CommandLineConfigurationRegistry::Load(ConfigurationSectionBuilder& builder)
    {
        for (const auto& pair : m_Data)
        {
            auto tokens = pair.Split('=');
            if (tokens.Size() != 2)
            {
                continue;
            }

            auto [path, value] = tokens.AsTuple<2>();
            builder.SetValue(path, value);
        }
    }
} // namespace UN::DI
