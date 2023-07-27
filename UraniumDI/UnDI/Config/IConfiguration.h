#pragma once
#include <UnTL/Containers/ArraySlice.h>
#include <UnTL/Memory/Memory.h>
#include <UnTL/Strings/StringSlice.h>
#include <optional>

namespace UN::DI
{
    class IConfigurationSection;

    class IConfiguration : public IObject
    {
    public:
        [[nodiscard]] virtual std::optional<StringSlice> Get(const StringSlice& path) const          = 0;
        [[nodiscard]] virtual const IConfigurationSection* GetSection(const StringSlice& path) const = 0;
        [[nodiscard]] virtual ArraySlice<IConfigurationSection*> GetChildren()                       = 0;
    };
} // namespace UN::DI
