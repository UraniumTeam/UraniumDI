#pragma once
#include <UnDI/Config/IConfiguration.h>
#include <UnDI/Config/IConfigurationSection.h>

namespace UN::DI
{
    class Configuration : public Object<IConfiguration>
    {
        Ptr<IConfigurationSection> m_pRootSection;

    public:
        inline explicit Configuration(IConfigurationSection* pRoot)
            : m_pRootSection(pRoot)
        {
        }

        [[nodiscard]] std::optional<StringSlice> Get(const StringSlice& path) const override;
        [[nodiscard]] const IConfigurationSection* GetSection(const StringSlice& path) const override;
        [[nodiscard]] ArraySlice<IConfigurationSection*> GetChildren() override;
    };
} // namespace UN::DI
