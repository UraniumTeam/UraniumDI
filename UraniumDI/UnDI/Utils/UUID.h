#pragma once
#include <UnDI/Base/Base.h>
#include <array>
#include <cctype>
#include <string_view>
#include <ostream>

namespace UN
{
    //! \brief A struct to work with UUIDs.
    struct alignas(8) UUID
    {
        std::array<UInt8, 16> Data{};

        inline UUID() = default;

        inline UUID(const UUID& other) noexcept
        {
            memcpy(Data.data(), other.Data.data(), 16);
        }

        inline UUID& operator=(const UUID& other) noexcept
        {
            memcpy(Data.data(), other.Data.data(), 16);
            return *this;
        }

        //! \brief Parse a UUID from a string in form `"xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"`.
        inline explicit UUID(const char* str) noexcept
        {
            assert(TryParse(str, *this, false) && "Invalid format");
        }

        inline static bool TryParse(const char* str, UUID& result, bool assertLength = true)
        {
            static char digits[]    = "0123456789ABCDEF";
            constexpr auto getValue = [](char c) {
                return static_cast<UInt8>(std::find(digits, digits + 16, std::toupper(c)) - digits);
            };

            USize idx  = 0;
            auto parse = [&](Int32 n) -> bool {
                for (Int32 i = 0; i < n; ++i)
                {
                    auto v1 = getValue(*str++);
                    if (v1 >= 16)
                        return false;

                    auto v2 = getValue(*str++);
                    if (v2 >= 16)
                        return false;

                    result.Data[idx] = v1 << 4;
                    result.Data[idx++] |= v2 & 0x0F;
                }

                return true;
            };

            // clang-format off
            if (!parse(4))       return false;
            if (*str++ != '-')   return false;
            if (!parse(2))       return false;
            if (*str++ != '-')   return false;
            if (!parse(2))       return false;
            if (*str++ != '-')   return false;
            if (!parse(2))       return false;
            if (*str++ != '-')   return false;
            if (!parse(6))       return false;
            // clang-format on

            if (*str != '\0' && assertLength)
                return false;

            return true;
        }

        inline std::string ToString() const
        {
            std::string buffer;
            static char digits[] = "0123456789ABCDEF";
            Int32 idx            = 0;
            buffer.reserve(36);
            auto append = [&](UInt32 n) {
                for (UInt32 i = 0; i < n; ++i)
                {
                    UInt8 c = Data[idx++];
                    buffer.push_back(digits[(c & 0xF0) >> 4]);
                    buffer.push_back(digits[(c & 0x0F) >> 0]);
                }
            };

            append(4);
            buffer.push_back('-');
            append(2);
            buffer.push_back('-');
            append(2);
            buffer.push_back('-');
            append(2);
            buffer.push_back('-');
            append(6);

            return buffer;
        }
    };

    inline std::strong_ordering operator<=>(const UUID& lhs, const UUID& rhs)
    {
        auto* l = reinterpret_cast<const UInt64*>(lhs.Data.data());
        auto* r = reinterpret_cast<const UInt64*>(rhs.Data.data());

        auto c0 = l[0] <=> r[0];
        auto c1 = l[1] <=> r[1];

        if (c0 != std::strong_ordering::equal)
        {
            return c0;
        }

        return c1;
    }

    inline bool operator==(const UUID& lhs, const UUID& rhs)
    {
        return std::equal(lhs.Data.begin(), lhs.Data.end(), rhs.Data.begin());
    }

    inline bool operator!=(const UUID& lhs, const UUID& rhs)
    {
        return !(lhs == rhs);
    }
} // namespace UN

namespace std
{
    //!\ brief Calculate hash of a FE::UUID.
    template<>
    struct hash<UN::UUID>
    {
        inline size_t operator()(const UN::UUID& value) const noexcept
        {
            return hash<string_view>{}(string_view(reinterpret_cast<const char*>(value.Data.data()), value.Data.size()));
        }
    };

    inline std::ostream& operator<<(std::ostream& stream, const ::UN::UUID& uuid)
    {
        stream << uuid.ToString();
        return stream;
    }
} // namespace std
