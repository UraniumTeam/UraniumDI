#pragma once
#include <UnDI/Config/IConfiguration.h>
#include <optional>

namespace UN::DI
{
    class IConfigurationSection : public IConfiguration
    {
    public:
        [[nodiscard]] virtual StringSlice GetPath() const                 = 0;
        [[nodiscard]] virtual std::optional<StringSlice> GetValue() const = 0;

        [[nodiscard]] inline StringSlice GetKey() const
        {
            auto path  = GetPath();
            auto begin = path.FindLastOf(':');
            return { begin == path.end() ? path.begin() : ++begin, path.end() };
        }
    };
} // namespace UN::DI
