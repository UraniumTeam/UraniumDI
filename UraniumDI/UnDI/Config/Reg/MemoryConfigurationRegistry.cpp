#include <UnDI/Config/ConfigurationBuilder.h>
#include <UnDI/Config/Reg/MemoryConfigurationRegistry.h>

namespace UN::DI
{
    void MemoryConfigurationRegistry::Load(ConfigurationSectionBuilder& builder)
    {
        for (auto& [path, value] : m_Data)
        {
            builder.SetValue(path, value);
        }
    }

    void MemoryConfigurationRegistry::Add(const StringSlice& path, const StringSlice& value)
    {
        m_Data[path] = value;
    }
} // namespace UN::DI
