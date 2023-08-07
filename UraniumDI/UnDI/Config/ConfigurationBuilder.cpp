#include <UnDI/Config/Configuration.h>
#include <UnDI/Config/ConfigurationBuilder.h>
#include <UnDI/Config/ConfigurationSection.h>
#include <UnDI/Config/IConfigurationRegistry.h>

namespace UN::DI
{
    ConfigurationSectionBuilder::ConfigurationSectionBuilder()
    {
        m_pRootSection = AllocateObject<ConfigurationSection>();
    }

    void ConfigurationSectionBuilder::SetValue(const StringSlice& path, const StringSlice& value)
    {
        un_assert_cast<ConfigurationSection*>(m_pRootSection.Get())->SetValue(path, value);
    }

    void ConfigurationBuilder::AddRegistry(IConfigurationRegistry* pRegistry)
    {
        pRegistry->Load(m_Builder);
    }

    IConfiguration* ConfigurationBuilder::Build()
    {
        return AllocateObject<Configuration>(m_Builder.m_pRootSection.Get());
    }
} // namespace UN::DI
