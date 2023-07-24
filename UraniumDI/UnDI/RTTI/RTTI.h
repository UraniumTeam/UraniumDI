#pragma once
#include <UnDI/Base/Base.h>
#include <UnDI/Utils/UUID.h>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace UN
{
    using TypeID = UUID;

    //! \brief Define common RTTI functions for a struct.
    //!
    //! Same as UN_RTTI_Class, but for `final` classes that don't inherit from other classes.\n
    //! Use this whenever possible, because UN_RTTI_Class implements virtual functions.
    //!
    //! \see UN_RTTI_Class
#define UN_RTTI_Struct(name, uuid)                                                                                               \
    inline void UnRTTI_Checks()                                                                                                  \
    {                                                                                                                            \
        using ThisType = std::remove_reference_t<decltype(*this)>;                                                               \
        static_assert(std::is_same_v<name, ThisType>);                                                                           \
    }                                                                                                                            \
                                                                                                                                 \
    inline static const ::UN::TypeID& UnRTTI_GetSID()                                                                            \
    {                                                                                                                            \
        static ::UN::TypeID id = ::UN::TypeID(uuid);                                                                             \
        return id;                                                                                                               \
    }                                                                                                                            \
                                                                                                                                 \
    inline static std::string_view UnRTTI_GetSName()                                                                             \
    {                                                                                                                            \
        return ::UN::TypeName<name>();                                                                                           \
    }

    //! \brief Define common RTTI functions for a class.
    //!
    //! This macro is a part of RTTI implementation.
    //! Use it to allow `un_dynamic_cast` and dependency injection.\n
    //!
    //! Example:
    //! \code{.cpp}
    //!     class Foo
    //!     {
    //!     public:
    //!         UN_RTTI_Class(Foo, "12CED1D1-6337-443F-A854-B4624A6133AE");
    //!         // members ...
    //!     };
    //! \endcode
    //!
    //! \note Use UN_RTTI_Struct for `final` structs that don't inherit from any other class.
#define UN_RTTI_Class(name, uuid)                                                                                                \
    UN_RTTI_Struct(name, uuid);                                                                                                  \
    UN_PUSH_CLANG_WARNING("-Winconsistent-missing-override")                                                                     \
    inline virtual std::string_view UnRTTI_GetName() const                                                                       \
    {                                                                                                                            \
        return ::UN::TypeName<name>();                                                                                           \
    }                                                                                                                            \
                                                                                                                                 \
    inline virtual ::UN::TypeID UnRTTI_GetID() const                                                                             \
    {                                                                                                                            \
        return name ::UnRTTI_GetSID();                                                                                           \
    }                                                                                                                            \
                                                                                                                                 \
    template<class UnRTTI_IS_TYPE>                                                                                               \
    inline bool UnRTTI_Is() const                                                                                                \
    {                                                                                                                            \
        return UnRTTI_IS_TYPE::UnRTTI_GetSID() == UnRTTI_GetID();                                                                \
    }                                                                                                                            \
    UN_POP_CLANG_WARNING

    template<class T>
    concept ImplementsStaticRTTI = requires(T& value)
    {
        T::UnRTTI_GetSID();
    };

    template<class T>
    concept ImplementsRTTI = requires(T& value)
    {
        value.UnRTTI_GetID();
    };

    //! \brief Cast a pointer to a base class to a derived class pointer if possible.
    //!
    //! Works just like normal `dynamic_cast<T*>`, except it uses only the classes that provide RTTI
    //! through UN_RTTI_Class.
    //!
    //! \tparam TDstPtr - Type of return value, e.g. `DerivedClass*`, _must_ be a pointer.
    //! \tparam TSrc    - Type of source value, e.g. `BaseClass`, _must not_ be a pointer.
    //! \param [in] src - The source value.
    //!
    //! \return The result pointer if destination type was derived from source type, `nullptr` otherwise.
    template<class TDstPtr, ImplementsRTTI TSrc,
             std::enable_if_t<std::is_base_of_v<TSrc, std::remove_pointer_t<TDstPtr>>, bool> = true>
    inline TDstPtr un_dynamic_cast(TSrc* src)
    {
        if (src->template UnRTTI_Is<std::remove_pointer_t<TDstPtr>>())
            return static_cast<TDstPtr>(src);

        return nullptr;
    }

    //! \brief Assert that a variable can be dynamically casted to another type.
    //!
    //! Works just like un_dynamic_cast<T*>, except it will assert that a type can be casted and won't return
    //! a `nullptr`. Use this when you're certainly sure that you can use `static_cast` here, but want to check it
    //! in debug builds. In release builds, when assertions are disabled, this can lead to undefined behaviour.
    //!
    //! \note The function only works with the classes that provide RTTI through UN_RTTI_Class.
    //!
    //! \tparam TDstPtr - Type of return value, e.g. `DerivedClass*`, _must_ be a pointer.
    //! \tparam TSrc    - Type of source value, e.g. `BaseClass`, _must not_ be a pointer.
    //! \param [in] src - The source value.
    //!
    //! \return The result pointer if destination type was derived from source type.
    template<class TDstPtr, ImplementsRTTI TSrc,
             std::enable_if_t<std::is_base_of_v<TSrc, std::remove_pointer_t<TDstPtr>>, bool> = true>
    inline TDstPtr un_assert_cast(TSrc* src)
    {
        assert(src->template UnRTTI_Is<std::remove_pointer_t<TDstPtr>>() && "Assert cast failed");
        return static_cast<TDstPtr>(src);
    }

    //! \brief Get name of a type.
    //!
    //! This function returns a short name provided in UN_RTTI_Class or UN_RTTI_Struct.\n
    //! Example:
    //! \code{.cpp}
    //!     template<class T>
    //!     class Foo
    //!     {
    //!         UN_RTTI_Class(Foo, "4BDF1868-0E22-48CF-9DBA-8DD10F2A9D0C");
    //!         // members...
    //!     };
    //!
    //!     auto fooName = un_nameof<Foo<int>>(); // "Foo" - not "Foo<int>"!
    //! \endcode
    //!
    //! \note The provided type must implement RTTI through UN_RTTI_Class or UN_RTTI_Struct.
    //!
    //! \tparam T - Type to get the name of.
    //!
    //! \return Short name of type T.
    template<ImplementsStaticRTTI T>
    inline std::string_view un_nameof() noexcept
    {
        return T::UnRTTI_GetSName();
    }

    //! \brief Get name of a type.
    //!
    //! This functions is same as un_nameof(), but can return name of derived class if called from a base class.\n
    //! Returns a short name provided in UN_RTTI_Class or UN_RTTI_Struct.\n
    //! Example:
    //! \code{.cpp}
    //!     class Base
    //!     {
    //!     public:
    //!         UN_RTTI_Class(Base, "AB26B8C7-827F-4212-88B4-F71A5EFD6EEB");
    //!     };
    //!     class Derived : public Base
    //!     {
    //!     public:
    //!         UN_RTTI_Class(Derived, "68CCD7DF-507F-4F3B-9EC3-001EEB33EB55");
    //!     };
    //!
    //!     auto derivedName = un_nameof(*static_cast<Base*>(new Derived)); // "Derived"
    //! \endcode
    //!
    //! For additional information see overload of un_nameof() without parameters.
    //!
    //! \note The provided type must implement RTTI through UN_RTTI_Class or UN_RTTI_Struct.
    //!
    //! \tparam T - Type to get name of.
    //!
    //! \return Short name of type T.
    //!
    //! \see un_nameof()
    template<ImplementsRTTI T>
    inline std::string_view un_nameof(const T& object)
    {
        return object.UnRTTI_GetName();
    }

    template<ImplementsStaticRTTI T>
    inline const UUID& un_typeid() noexcept
    {
        return T::UnRTTI_GetSID();
    }

    template<ImplementsRTTI T>
    inline const UUID& un_typeid(const T& object)
    {
        return object.UnRTTI_GetID();
    }
} // namespace UN
