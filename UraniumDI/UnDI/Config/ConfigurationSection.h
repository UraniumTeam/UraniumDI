#pragma once
#include <UnDI/Config/IConfigurationSection.h>
#include <UnTL/Strings/String.h>

namespace UN::DI
{
    class ConfigurationSection : public Object<IConfigurationSection>
    {
        List<IConfigurationSection*> m_Children;
        std::optional<StringSlice> m_Value;
        String m_Path;

        [[nodiscard]] inline ConfigurationSection* GetChildImpl(const StringSlice& name) const
        {
            for (auto* child : m_Children)
            {
                if (child->GetKey() == name)
                {
                    return un_assert_cast<ConfigurationSection*>(child);
                }
            }

            return nullptr;
        }

    public:
        inline ConfigurationSection() = default;
        ~ConfigurationSection() override;

        [[nodiscard]] std::optional<StringSlice> Get(const StringSlice& path) const override;
        [[nodiscard]] const IConfigurationSection* GetSection(const StringSlice& path) const override;
        [[nodiscard]] ArraySlice<IConfigurationSection*> GetChildren() override;
        [[nodiscard]] StringSlice GetPath() const override;
        [[nodiscard]] std::optional<StringSlice> GetValue() const override;

        void SetValue(const StringSlice& path, const StringSlice& value);
    };
} // namespace UN::DI
