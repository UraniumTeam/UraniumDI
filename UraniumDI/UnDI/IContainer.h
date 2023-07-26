#pragma once
#include <UnTL/Base/Base.h>
#include <UnDI/IServiceProvider.h>
#include <UnTL/Memory/Memory.h>

namespace UN::DI
{
    class ILifetimeScope;

    class IContainer : public IServiceProvider
    {
    public:
        UN_RTTI_Class(IContainer, "C8321BA9-205F-4DFB-8339-DE405337F5CC");

        [[nodiscard]] virtual ILifetimeScope* GetRootScope() const = 0;
    };

#define UN_Injectable(Type, ...)                                                                                                 \
    UN_PUSH_MSVC_WARNING(4100);                                                                                                  \
    inline static void CreateHelper(__VA_ARGS__)                                                                                 \
    {                                                                                                                            \
    }                                                                                                                            \
    UN_POP_MSVC_WARNING;                                                                                                         \
    Type(__VA_ARGS__)
} // namespace UN::DI
