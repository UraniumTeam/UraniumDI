#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/RTTI/RTTI.h>
#include <concepts>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace UN::DI
{
    template<class T>
    using Ptr = std::shared_ptr<T>;

    class IService
    {
    public:
        UN_CLASS_RTTI(IService, "53A3EFC1-552E-47F4-9E0C-3DB11932316D");
    };

    template<class T>
    concept Service = std::derived_from<T, IService> && ImplementsStaticRTTI<T>;

    template<class T>
    concept ServiceImpl = Service<T> && ImplementsRTTI<T> && requires
    {
        &T::CreateHelper;
    };

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

        std::unordered_map<TypeID, std::function<Ptr<IService>(Container&)>> m_Map;

        template<class T, class TFunc, class... Types>
        inline Ptr<T> CreateService([[maybe_unused]] TFunc (*f)(Types...))
        {
            return std::make_shared<T>(GetService<RemovePtr<Types>>()...);
        }

    public:
        template<ServiceImpl T, Service TAs>
        inline void RegisterService()
        {
            m_Map[un_typeid<TAs>()] = [](Container& container) {
                return container.CreateService<T>(T::CreateHelper);
            };
        }

        template<Service T>
        inline Ptr<T> GetService()
        {
            auto typeId = un_typeid<T>();
            assert(m_Map.contains(typeId) && "Service not found");
            return std::static_pointer_cast<T>(m_Map.at(typeId)(*this));
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
