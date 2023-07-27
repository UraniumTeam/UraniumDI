#include <UnDI/Config/ConfigurationSection.h>

namespace UN::DI
{
    std::optional<StringSlice> ConfigurationSection::Get(const StringSlice& path) const
    {
        auto* section = GetSection(path);
        if (section == nullptr)
        {
            return std::nullopt;
        }

        return section->GetValue();
    }

    const IConfigurationSection* ConfigurationSection::GetSection(const StringSlice& path) const
    {
        const auto* section = this;

        auto tokens = path.Split(':');
        for (const auto& token : tokens)
        {
            section = section->GetChildImpl(token);
            if (section == nullptr)
            {
                return nullptr;
            }
        }

        return section;
    }

    ArraySlice<IConfigurationSection*> ConfigurationSection::GetChildren()
    {
        return m_Children;
    }

    StringSlice ConfigurationSection::GetPath() const
    {
        return m_Path;
    }

    std::optional<StringSlice> ConfigurationSection::GetValue() const
    {
        return m_Value;
    }

    void ConfigurationSection::SetValue(const StringSlice& path, const StringSlice& value)
    {
        auto* section = this;

        auto tokens = path.Split(':');
        for (USize i = 0; i < tokens.Size(); ++i)
        {
            auto next = section->GetChildImpl(tokens[i]);
            if (next == nullptr)
            {
                next = AllocateObject<ConfigurationSection>();
                next->AddRef();
                next->m_Path = String::Join(":", ArraySlice<StringSlice>(tokens)(0, i + 1));
                section->m_Children.Push(next);
            }

            section = next;
        }

        section->m_Value = value;
    }

    ConfigurationSection::~ConfigurationSection()
    {
        for (auto* child : m_Children)
        {
            child->Release();
        }
    }
} // namespace UN::DI
