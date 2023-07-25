#pragma once
#include <UnDI/Memory/Memory.h>
#include <UnDI/Utils/Result.h>

namespace UN::DI
{
    class ILifetimeScope;

    class IServiceProvider : public IObject
    {
    public:
        UN_RTTI_Class(IServiceProvider, "2DC07B09-66A8-4FAE-9556-9D77EFAB6465");

        virtual Result<ILifetimeScope*, ErrorCode> BeginScope()                 = 0;
        virtual Result<IObject*, ErrorCode> Resolve(const UUID& registrationID) = 0;

        template<class T>
        inline Result<T*, ErrorCode> Resolve()
        {
            return Resolve(un_typeid<T>()).Map([](IObject* object) {
                return static_cast<T*>(object);
            });
        }
    };
} // namespace UN::DI
