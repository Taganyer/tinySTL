//
// Created by 86152 on 2023/9/26.
//

#ifndef TINYSTL_TYPE_TRAITS_HPP
#define TINYSTL_TYPE_TRAITS_HPP

namespace STD {

    struct True_type {
    };

    struct False_type {
    };

    //这个模板的作用在于推断传入的的类型是否有效，如果有效Void_t将为void
    template<typename...> using Void_t = void;

//----------------------------------------------------------------------------------------------------------------------

    template<typename T>
    struct Type_traits {
        using Has_trivial_default_constructor = False_type;
        using Has_trivial_copy_constructor = False_type;
        using Has_trivial_assignment_operator = False_type;
        using Has_trivial_destructor = False_type;
        using Is_POD_type = False_type;
    };
    //对于内置类型的偏特化
    template<>
    struct Type_traits<bool> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<char> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<unsigned char> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<signed char> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<wchar_t> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<short> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<unsigned short> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<int> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<unsigned int> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<long> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<unsigned long> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<long long> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<unsigned long long> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<float> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<double> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

    template<>
    struct Type_traits<long double> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };
    //对于原生指针的偏特化。
    template<class T>
    struct Type_traits<T *> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };
    //对于原生const指针的偏特化。
    template<class T>
    struct Type_traits<const T *> {
        using Has_trivial_default_constructor = True_type;
        using Has_trivial_copy_constructor = True_type;
        using Has_trivial_assignment_operator = True_type;
        using Has_trivial_destructor = True_type;
        using Is_POD_type = True_type;
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    struct Remove_reference {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_reference<Arg &> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_reference<Arg &&> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_pointer {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_pointer<Arg *> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_const {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_const<Arg const> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_volatile {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_volatile<Arg volatile> {
        using Type = Arg;
    };

//----------------------------------------------------------------------------------------------------------------------

    //这个模板是为了支持SFINAE技术，在类型推断失败时不会导致编译错误。
    template<typename Arg, typename = void>
    struct Can_left_reference : public False_type {
    };

    template<typename Arg>
    struct Can_left_reference<Arg, Void_t<Arg &>> : public True_type {
    };

    template<typename Arg, typename = void>
    struct Can_right_reference : public False_type {
    };

    template<typename Arg>
    struct Can_right_reference<Arg, Void_t<Arg &&>> : public True_type {
    };

    template<typename Arg, typename T>
    struct Add_lvalue_reference_Helper {
        using Type = Arg;
    };

    template<typename Arg>
    struct Add_lvalue_reference_Helper<Arg, True_type> {
        using Type = Arg &;
    };

    template<typename Arg>
    struct Add_lvalue_reference {
        using Type = typename Add_lvalue_reference_Helper<Arg, Can_left_reference<Arg>>::Type;
    };

    template<typename Arg, typename T>
    struct Add_rvalue_reference_Helper {
        using Type = Arg;
    };

    template<typename Arg>
    struct Add_rvalue_reference_Helper<Arg, True_type> {
        using Type = Arg &&;
    };

    template<typename Arg>
    struct Add_rvalue_reference {
        using Type = typename Add_rvalue_reference_Helper<Arg, Can_right_reference<Arg>>::Type;
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    struct Remove_cv {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_cv<const Arg> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_cv<volatile Arg> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Remove_cv<const volatile Arg> {
        using Type = Arg;
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    struct Add_const {
        using Type = Arg const;
    };

    template<typename Arg>
    struct Add_const<Arg &> {
        using Type = const Arg &;
    };

    template<typename Arg>
    struct Add_volatile {
        using Type = Arg volatile;
    };

    template<typename Arg>
    struct Add_cv {
        using Type = Arg const volatile;
    };

    template<typename Arg, typename = void>
    struct Can_add_pointer : public False_type {
    };

    template<typename Arg>
    struct Can_add_pointer<Arg, Void_t<Arg *>> : public True_type {
    };

    template<typename Arg, typename T>
    struct Add_pointer_Helper {
        using Type = Arg;
    };

    template<typename Arg>
    struct Add_pointer_Helper<Arg, True_type> {
        using Type = Arg *;
    };

    template<typename Arg>
    struct Add_pointer {
        using Type = typename Add_pointer_Helper<Arg, Can_add_pointer<Arg>>::Type;
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    struct Is_integral_Helper {
        using Type = False_type;
    };

    template<>
    struct Is_integral_Helper<bool> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<char> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<signed char> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<unsigned char> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<wchar_t> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<char16_t> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<char32_t> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<short> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<unsigned short> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<int> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<unsigned int> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<long> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<unsigned long> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<long long> {
        using Type = True_type;
    };

    template<>
    struct Is_integral_Helper<unsigned long long> {
        using Type = True_type;
    };

    template<typename Arg>
    struct Is_integral : public Is_integral_Helper<typename Remove_cv<Arg>::Type>::Type {
    };

//----------------------------------------------------------------------------------------------------------------------

    template<bool, typename = void>
    struct Enable_if {
    };

    template<typename Arg>
    struct Enable_if<true, Arg> {
        using Type = Arg;
    };

    template<typename Arg>
    struct Decay {
        using Type = typename Remove_reference<typename Remove_cv<Arg>::Type>::Type;
    };

    template<typename Temp>
    struct Always_False {
        static constexpr bool Value = false;
    };

    template<typename Temp>
    struct Always_True {
        static constexpr bool value = true;
    };

    template<typename ...Args>
    struct Common_Type;

    template<typename Arg>
    struct Common_Type<Arg> {
        using Type = Arg;
    };

    template<typename Arg1, typename Arg2>
    struct Common_Type<Arg1, Arg2> {
        using Type = decltype(Always_False<Arg1>::Value ? Decay<Arg1>() : Decay<Arg2>());
    };

    template<typename Arg1, typename Arg2, typename ...Args>
    struct Common_Type<Arg1, Arg2, Args...> {
        using Type = typename Common_Type<typename Common_Type<Arg1, Arg2>::Type, Args...>::Type;
    };

    template<typename Type1, typename Type2>
    struct Is_Same_Type {
        static constexpr bool Value = false;
    };

    template<typename Type>
    struct Is_Same_Type<Type, Type> {
        static constexpr bool Value = true;
    };

    template<typename Type1, typename Type2>
    constexpr bool Is_Same_Type_V = Is_Same_Type < Type1, Type2
    >::Value;

}

#endif //TINYSTL_TYPE_TRAITS_HPP
