//
// Created by 86152 on 2023/10/12.
//

#ifndef TINYSTL_TUPLE_HPP
#define TINYSTL_TUPLE_HPP

#include "Config.hpp"
#include "Type_traits.hpp"
#include "Move.hpp"

namespace STD {

    namespace Detail {

        template <unsigned Index, typename Arg>
        struct Node {
            using Type = Arg;

            using Ref = Arg&;

            using C_Ref = const Arg&;

            Arg value;

            constexpr Node() = default;

            explicit constexpr Node(const Arg& target) : value(target) {};

            explicit constexpr Node(Arg&& target) : value(move(target)) {};

            constexpr Node(const Node&) = default;

            constexpr Node(Node&&) = default;

            static constexpr Ref get_value(Node& target) {
                return target.value;
            }

            static constexpr C_Ref get_value(const Node& target) {
                return target.value;
            }

        };

//----------------------------------------------------------------------------------------------------------------------

        template <unsigned Index, typename Head, typename... Next>
        struct Tuple_Helper : public Tuple_Helper<Index + 1, Next...>, public Node<Index, Head> {
            using Basic = Tuple_Helper<Index + 1, Next...>;

            using NodeT = Node<Index, Head>;

            constexpr Tuple_Helper() : Basic(), NodeT() {};

            template <typename T_Head, typename... T_Next>
            explicit constexpr Tuple_Helper(T_Head&& now, T_Next&&... next) :
                Basic(forward<T_Next>(next)...), NodeT(forward<T_Head>(now)) {};

        };

        template <unsigned Index, typename Head>
        struct Tuple_Helper<Index, Head> : public Node<Index, Head> {
            using NodeT = Node<Index, Head>;

            constexpr Tuple_Helper() : NodeT() {};

            template <typename T_Head>
            constexpr explicit Tuple_Helper(T_Head&& now) : NodeT(forward<T_Head>(now)) {};

        };

//----------------------------------------------------------------------------------------------------------------------

        // template<unsigned Index, unsigned Step, typename Head, typename ...Next>
        // struct get_traits : public get_traits<Index - 1, Step + 1, Next...> {
        // };
        //
        // template<unsigned Step, typename Head, typename ...Next>
        // struct get_traits<0, Step, Head, Next...> {
        //     using Type = typename Tuple_Helper<Step, Head, Next...>::NodeT::Type;
        //
        //     using Ref = typename Tuple_Helper<Step, Head, Next...>::NodeT::Ref;
        //
        //     using C_Ref = typename Tuple_Helper<Step, Head, Next...>::NodeT::C_Ref;
        // };

        template <unsigned Index, typename... Args>
        struct get_traits {
            using Type = typename Tuple_Helper<Index, Args...>::NodeT::Type;

            using Ref = typename Tuple_Helper<Index, Args...>::NodeT::Ref;

            using C_Ref = typename Tuple_Helper<Index, Args...>::NodeT::C_Ref;
        };
    }

//----------------------------------------------------------------------------------------------------------------------

    template <typename... Args>
    struct Tuple : private Detail::Tuple_Helper<0, Args...> {
    private:
        using Self = Tuple<Args...>;

        using Basic = Detail::Tuple_Helper<0, Args...>;

    public:
        constexpr Tuple() : Basic() {};

        template <typename... T_Next>
        explicit constexpr Tuple(T_Next&&... next) : Basic(forward<T_Next>(next)...) {};

        template <unsigned Index>
        friend auto get(Tuple<Args...>& target) -> typename Detail::get_traits<Index, Args...>::Ref {
            return Detail::Node<Index,
                                typename Detail::get_traits<Index, Args...>::Type>::get_value(target);
        }

        template <unsigned Index>
        friend auto get(const Tuple<Args...>& target) -> typename Detail::get_traits<Index, Args...>::C_Ref {
            return Detail::Node<Index,
                                typename Detail::get_traits<Index, Args...>::Type>::get_value(target);
        }

    };

    template <unsigned Index, typename... Args>
    auto get(Tuple<Args...>& target) -> typename Detail::get_traits<Index, Args...>::Ref;

    template <unsigned Index, typename... Args>
    auto get(const Tuple<Args...>& target) -> typename Detail::get_traits<Index, Args...>::C_Ref;

}

#endif //TINYSTL_TUPLE_HPP
