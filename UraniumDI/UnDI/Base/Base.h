#pragma once
#include <UnDI/Base/Platform.h>
#include <atomic>
#include <cassert>
#include <cstdint>
#include <intrin.h>
#include <string_view>

namespace UN
{
    using Int8  = int8_t;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;

    using UInt8  = uint8_t;
    using UInt16 = uint16_t;
    using UInt32 = uint32_t;
    using UInt64 = uint64_t;

    using Float32 = float;
    using Float64 = double;

    using USize = UInt64;
    using SSize = Int64;

#ifdef UN_DEBUG
    //! \brief True on debug builds.
    inline constexpr bool IsDebugBuild = true;
#else
    //! \brief True on debug builds.
    inline constexpr bool IsDebugBuild = false;
#endif

    //! \brief Empty structure with no members.
    struct EmptyStruct
    {
    };

    //! \internal
    namespace Internal
    {
        //! \brief A simple `std::string_view` wrapper
        //!
        //! This is useful for function signatures when compiling with MSVC. `std::string_view` is a template class
        //! (`std::basic_string_view< ... >`). It makes difficult to retrieve typename from a template function
        //! signature since it makes more than one template.
        struct SVWrapper
        {
            std::string_view value; //!< Actual value of the string view.
        };

        //! \brief Remove leading and trailing spaces from a string view.
        inline constexpr std::string_view TrimTypeName(std::string_view name)
        {
            name.remove_prefix(name.find_first_not_of(' '));
            name.remove_suffix(name.length() - name.find_last_not_of(' ') - 1);
            return name;
        }

        //! \brief Internal implementation of \ref FE::TypeName.
        template<class T>
        inline constexpr SVWrapper TypeNameImpl()
        {
#if UN_COMPILER_MSVC
            std::string_view fn = __FUNCSIG__;
            fn.remove_prefix(fn.find_first_of("<") + 1);
            fn.remove_suffix(fn.length() - fn.find_last_of(">"));
#else
            std::string_view fn = __PRETTY_FUNCTION__;
            fn.remove_prefix(fn.find_first_of('=') + 1);
            fn.remove_suffix(fn.length() - fn.find_last_of(']'));
#endif
            return SVWrapper{ TrimTypeName(fn) };
        }
    } // namespace Internal
    //! \endinternal

    //! \brief Get name of a type as a compile-time constant.
    //!
    //! This implementation uses the `__PRETTY_FUNCTION__` hack to retrieve typename from a function signature
    //! at compile-time.
    template<class T>
    inline constexpr std::string_view TypeName()
    {
        return Internal::TypeNameImpl<T>().value;
    }

    //! \brief Maximum alignment value, should be enough for anything
    inline constexpr USize MaximumAlignment = 16;

    //! \brief Align up an integer.
    //!
    //! \param [in] x     - Value to align.
    //! \param [in] align - Alignment to use.
    template<class T, class U = T>
    inline T AlignUp(T x, U align)
    {
        return (x + (align - 1u)) & ~(align - 1u);
    }

    //! \brief Align up a pointer.
    //!
    //! \param [in] x     - Value to align.
    //! \param [in] align - Alignment to use.
    template<class T>
    inline T* AlignUpPtr(const T* x, USize align)
    {
        return reinterpret_cast<T*>(AlignUp(reinterpret_cast<USize>(x), align));
    }

    //! \brief Align up an integer.
    //!
    //! \param [in] x     - Value to align.
    //! \tparam A         - Alignment to use.
    template<UInt32 A, class T>
    inline constexpr T AlignUp(T x)
    {
        return (x + (A - 1)) & ~(A - 1);
    }

    //! \brief Align down an integer.
    //!
    //! \param [in] x     - Value to align.
    //! \param [in] align - Alignment to use.
    template<class T, class U = T>
    inline T AlignDown(T x, U align)
    {
        return (x & ~(align - 1));
    }

    //! \brief Align down a pointer.
    //!
    //! \param [in] x     - Value to align.
    //! \param [in] align - Alignment to use.
    template<class T>
    inline constexpr T* AlignDownPtr(const T* x, USize align)
    {
        return reinterpret_cast<T*>(AlignDown(reinterpret_cast<USize>(x), align));
    }

    //! \brief Align down an integer.
    //!
    //! \param [in] x     - Value to align.
    //! \tparam A         - Alignment to use.
    template<UInt32 A, class T>
    inline constexpr T AlignDown(T x)
    {
        return (x & ~(A - 1));
    }

    //! \brief Create a bitmask.
    //!
    //! \param [in] bitCount  - The number of ones in the created mask.
    //! \param [in] leftShift - The number of zeros to the right of the created mask.
    template<class T>
    inline constexpr T MakeMask(T bitCount, T leftShift)
    {
        auto typeBitCount = sizeof(T) * 8;
        auto mask         = bitCount == typeBitCount ? static_cast<T>(-1) : ((1 << bitCount) - 1);
        return static_cast<T>(mask << leftShift);
    }

    inline void HashCombine(std::size_t& /* seed */) {}

    //! \brief Combine hashes of specified values with seed.
    //!
    //! \tparam Args - Types of values.
    //!
    //! \param [in,out] seed - Initial hash value to combine with.
    //! \param [in]     args - The values to calculate hash of.
    template<typename T, typename... Args>
    inline void HashCombine(std::size_t& seed, const T& value, const Args&... args)
    {
        std::hash<T> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        HashCombine(seed, args...);
    }

    //! \brief Define std::hash<> for a type.
#define UN_MAKE_HASHABLE(TypeName, Template, ...)                                                                                \
    namespace std                                                                                                                \
    {                                                                                                                            \
        template<Template>                                                                                                       \
        struct hash<TypeName>                                                                                                    \
        {                                                                                                                        \
            inline size_t operator()(const TypeName& value) const noexcept                                                       \
            {                                                                                                                    \
                size_t seed = 0;                                                                                                 \
                ::FE::HashCombine(seed, __VA_ARGS__);                                                                            \
                return seed;                                                                                                     \
            }                                                                                                                    \
        };                                                                                                                       \
    }

} // namespace UN
