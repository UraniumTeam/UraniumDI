#pragma once
#include <UnDI/Config/IConfigurationRegistry.h>
#include <UnTL/Strings/String.h>

namespace UN::DI
{
    class MemoryConfigurationRegistry final : public Object<IConfigurationRegistry>
    {
        UnorderedMap<String, String> m_Data;

    public:
        void Add(const StringSlice& path, const StringSlice& value);
        void Load(ConfigurationSectionBuilder& builder) override;
    };
} // namespace UN::DI
