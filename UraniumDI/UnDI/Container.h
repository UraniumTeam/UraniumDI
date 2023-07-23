#pragma once
#include <UnDI/Base/Base.h>
#include <concepts>
#include <functional>
#include <memory>
#include <unordered_map>

namespace UN::DI
{
    template<class T>
    using Ptr = std::shared_ptr<T>;

    class IService
    {
    public:
    };

    template<class T>
    concept ServiceInterface = std::derived_from<T, IService> && requires
    {
        { T::GetTypeId() } -> std::same_as<int>;
    };

    template<class T>
    concept Service = std::derived_from<T, IService>;

    class Container
    {
        template<class T>
        struct RemovePtrHelper
        {
            typedef T Type;
        };

        template<class T>
        struct RemovePtrHelper<Ptr<T>>
        {
            typedef T Type;
        };

        template<class T>
        using RemovePtr = typename RemovePtrHelper<std::remove_cvref_t<T>>::Type;

        std::unordered_map<int, std::function<Ptr<IService>(Container&)>> m_Map;

        template<class T, class TFunc, class... Types>
        inline Ptr<T> CreateService([[maybe_unused]] TFunc (*f)(Types...))
        {
            return std::make_shared<T>(GetService<RemovePtr<Types>>()...);
        }

    public:
        template<Service T, ServiceInterface TAs>
        inline void RegisterService()
        {
            m_Map[TAs::GetTypeId()] = [](Container& container) {
                return container.CreateService<T>(T::CreateHelper);
            };
        }

        template<Service T>
        inline Ptr<T> GetService()
        {
            return std::static_pointer_cast<T>(m_Map[T::GetTypeId()](*this));
        }
    };
} // namespace UN::DI

#define UN_Injectable(Type, ...)                                                                                                 \
    UN_PUSH_MSVC_WARNING(4100);                                                                                                  \
    inline static void CreateHelper(__VA_ARGS__)                                                                                 \
    {                                                                                                                            \
    }                                                                                                                            \
    UN_POP_MSVC_WARNING;                                                                                                         \
    Type(__VA_ARGS__)
