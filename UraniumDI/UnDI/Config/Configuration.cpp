#include <UnDI/Config/Configuration.h>
#include <UnDI/Config/IConfigurationSection.h>

namespace UN::DI
{
    std::optional<StringSlice> Configuration::Get(const StringSlice& path) const
    {
        return m_pRootSection->Get(path);
    }

    const IConfigurationSection* Configuration::GetSection(const StringSlice& path) const
    {
        return m_pRootSection->GetSection(path);
    }

    ArraySlice<IConfigurationSection*> Configuration::GetChildren()
    {
        return m_pRootSection->GetChildren();
    }
} // namespace UN::DI
