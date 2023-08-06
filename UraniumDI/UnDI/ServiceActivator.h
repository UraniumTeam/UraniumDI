#pragma once
#include <UnDI/IServiceProvider.h>
#include <UnTL/Memory/Memory.h>
#include <functional>
#include <optional>
#include <utility>

namespace UN::DI
{
    namespace Internal
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
        struct RemovePtrHelper<T*>
        {
            typedef T Type;
        };

        template<class T>
        using RemovePtr = typename RemovePtrHelper<std::remove_cvref_t<T>>::Type;

        template<class T>
        concept IsPointer = std::is_pointer_v<T> || std::is_same_v<Ptr<RemovePtr<T>>, std::remove_cvref_t<T>>;
    }

    using ActivatorFunc = std::function<Result<IObject*, ErrorCode>(IServiceProvider*)>;

    class ServiceActivator final
    {
        ActivatorFunc m_Func;

        class Context
        {
            IAllocator* m_pAllocator;
            IServiceProvider* m_pLifetimeScope;
            std::optional<ErrorCode> m_ResolveError;

            struct ArgResolver
            {
                Context* Ctx;

                UN_FINLINE explicit constexpr ArgResolver(Context* ctx)
                    : Ctx(ctx)
                {
                }

                template<Internal::IsPointer T>
                UN_FINLINE operator T() // NOLINT(google-explicit-constructor)
                {
                    return Ctx->GetService<Internal::RemovePtr<T>>();
                }
            };

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

            template<class T, class TArg>
            inline std::enable_if_t<!std::is_constructible_v<T, TArg>, T*> CreateServiceImpl(TArg)
            {
                return AllocateObjectEx<T>(m_pAllocator);
            }

            template<class T, class TArg>
            inline std::enable_if_t<std::is_constructible_v<T, TArg>, T*> CreateServiceImpl(TArg a)
            {
                return AllocateObjectEx<T>(m_pAllocator, a);
            }

            template<class T, class TArg1, class TArg2, class... TArgs>
            inline std::enable_if_t<std::is_constructible_v<T, TArg1, TArg2, TArgs...>, T*> //
            CreateServiceImpl(TArg1 a1, TArg2 a2, TArgs... args)
            {
                return AllocateObjectEx<T>(m_pAllocator, a1, a2, args...);
            }

            template<class T, class TArg1, class TArg2, class... TArgs>
            inline std::enable_if_t<!std::is_constructible_v<T, TArg1, TArg2, TArgs...>, T*> //
            CreateServiceImpl(TArg1, TArg2 a2, TArgs... args)
            {
                return CreateServiceImpl<T>(a2, args...);
            }

            template<class T>
            inline T* CreateService()
            {
                return CreateServiceImpl<T>(ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this),
                                            ArgResolver(this));
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
            result.m_Func = std::move(function);
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

                auto* obj = ctx.CreateService<T>();
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
