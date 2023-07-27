#pragma once
#include <UnTL/Memory/Memory.h>
#include <UnTL/Strings/StringSlice.h>
#include <UnDI/Config/IConfiguration.h>
#include <UnDI/Config/IConfigurationSection.h>

namespace UN::DI
{
    class IConfigurationRegistry;

    class ConfigurationSectionBuilder final
    {
        friend class ConfigurationBuilder;
        Ptr<IConfigurationSection> m_pRootSection;

    public:
        ConfigurationSectionBuilder();

        void SetValue(const StringSlice& path, const StringSlice& value);
    };

    class ConfigurationBuilder final
    {
        ConfigurationSectionBuilder m_Builder;

    public:
        [[nodiscard]] ConfigurationBuilder AddRegistry(IConfigurationRegistry* pRegistry);
        [[nodiscard]] IConfiguration* Build();
    };
} // namespace UN::DI
