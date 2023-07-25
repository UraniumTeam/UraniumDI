#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/Lifetime/ILifetimeScope.h>
#include <UnDI/Memory/Memory.h>

namespace UN::DI
{
    class IContainer : public ILifetimeScope
    {
    public:
        UN_RTTI_Class(IContainer, "C8321BA9-205F-4DFB-8339-DE405337F5CC");

        virtual ILifetimeScope* GetRootScope() = 0;
    };

#define UN_Injectable(Type, ...)                                                                                                 \
    UN_PUSH_MSVC_WARNING(4100);                                                                                                  \
    inline static void CreateHelper(__VA_ARGS__)                                                                                 \
    {                                                                                                                            \
    }                                                                                                                            \
    UN_POP_MSVC_WARNING;                                                                                                         \
    Type(__VA_ARGS__)
} // namespace UN::DI
