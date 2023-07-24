#pragma once
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/Memory/Memory.h>
#include <optional>

namespace UN::DI
{
    enum class ServiceActivateErrorCode
    {
        CircularDependency
    };

    class ServiceActivator final
    {
        typedef Result<IObject*, ServiceActivateErrorCode> (*ActivatorImpl)(ILifetimeScope*);

        ActivatorImpl m_pImpl;

        class Context
        {
            IAllocator* m_pAllocator;
            ILifetimeScope* m_pLifetimeScope;
            std::optional<ServiceResolveErrorCode> m_ResolveError;

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

        public:
            inline explicit Context(IAllocator* pAllocator, ILifetimeScope* pScope)
                : m_pAllocator(pAllocator)
                , m_pLifetimeScope(pScope)
                , m_ResolveError()
            {
            }

            [[nodiscard]] inline std::optional<ServiceResolveErrorCode> GetResolveError() const
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
                    return result.Unwrap();
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
        template<class T>
        inline static ServiceActivator CreateForType()
        {
            ServiceActivator result{ .m_pImpl = [](ILifetimeScope* pScope) {
                // TODO: resolve allocator from container
                IAllocator* pAllocator = SystemAllocator::Get();
                Context ctx(pAllocator, pScope);

                auto* obj = ctx.CreateService<T>(pAllocator, T::CreateHelper);
                if (auto err = ctx.GetResolveError())
                {
                    return Err(err.value());
                }

                return obj;
            } };

            return result;
        }
    };
} // namespace UN::DI
