#pragma once
#include <UnDI/IServiceProvider.h>
#include <UnTL/Memory/Memory.h>
#include <functional>
#include <optional>

namespace UN::DI
{
    using ActivatorFunc = std::function<Result<IObject*, ErrorCode>(IServiceProvider*)>;

    class ServiceActivator final
    {
        ActivatorFunc m_Func;

        class Context
        {
            IAllocator* m_pAllocator;
            IServiceProvider* m_pLifetimeScope;
            std::optional<ErrorCode> m_ResolveError;

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
            struct RemovePtrHelper<T*>
            {
                typedef T Type;
            };

            template<class T>
            using RemovePtr = typename RemovePtrHelper<std::remove_cvref_t<T>>::Type;

        public:
            inline explicit Context(IAllocator* pAllocator, IServiceProvider* pScope)
                : m_pAllocator(pAllocator)
                , m_pLifetimeScope(pScope)
                , m_ResolveError()
            {
            }

            [[nodiscard]] inline std::optional<ErrorCode> GetResolveError() const
            {
                return m_ResolveError;
            }

            template<class T>
            inline T* GetService()
            {
                if (m_ResolveError.has_value())
                {
                    return nullptr;
                }

                auto id     = un_typeid<T>();
                auto result = m_pLifetimeScope->Resolve(id);
                if (result)
                {
                    return static_cast<T*>(result.Unwrap());
                }

                m_ResolveError = result.UnwrapErr();
                return nullptr;
            }

            template<class T, class TFunc, class... Types>
            inline T* CreateService([[maybe_unused]] TFunc (*f)(Types...))
            {
                return AllocateObjectEx<T>(m_pAllocator, GetService<RemovePtr<Types>>()...);
            }
        };

    public:
        UN_RTTI_Struct(ServiceActivator, "E876EADA-6B71-4F3E-B072-C84B0486A47B");

        inline Result<IObject*, ErrorCode> Invoke(IServiceProvider* pProvider)
        {
            return m_Func(pProvider);
        }

        inline static ServiceActivator CreateForFunc(ActivatorFunc function)
        {
            ServiceActivator result{};
            result.m_Func = function;
            return result;
        }

        template<class T>
        inline static ServiceActivator CreateForType()
        {
            ServiceActivator result{};
            result.m_Func = [](IServiceProvider* pScope) -> Result<IObject*, ErrorCode> {
                // TODO: resolve allocator from container
                IAllocator* pAllocator = SystemAllocator::Get();
                Context ctx(pAllocator, pScope);

                auto* obj = ctx.CreateService<T>(T::CreateHelper);
                if (auto err = ctx.GetResolveError())
                {
                    return Err(err.value());
                }

                return static_cast<IObject*>(obj);
            };

            return result;
        }
    };
} // namespace UN::DI
