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

        template<int Index, typename Arg>
        struct Node {
            using Type = Arg;

            using Ref = Arg &;

            using C_Ref = const Arg &;

            Arg value;

            constexpr Node() = default;

            explicit constexpr Node(const Arg &target) : value(target) {};

            explicit constexpr Node(Arg &&target) : value(move(target)) {};

            constexpr Node(const Node &) = default;

            constexpr Node(Node &&) = default;

            static constexpr Ref get_value(Node &target) {
                return target.value;
            }

            static constexpr C_Ref get_value(const Node &target) {
                return target.value;
            }

        };

//----------------------------------------------------------------------------------------------------------------------

        template<int Index, typename Head, typename ...Next>
        struct Tuple_Helper : public Tuple_Helper<Index + 1, Next...>, public Node<Index, Head> {
            using Basic = Tuple_Helper<Index + 1, Next...>;

            using Node = Node<Index, Head>;

            constexpr Tuple_Helper() : Basic(), Node() {};

            template<typename T_Head, typename ...T_Next>
            explicit constexpr Tuple_Helper(T_Head &&now, T_Next &&...next) :
                    Basic(forward<T_Next>(next)...), Node(forward<T_Head>(now)) {};

        };

        template<int Index, typename Head>
        struct Tuple_Helper<Index, Head> : public Node<Index, Head> {
            using Node = Node<Index, Head>;

            constexpr Tuple_Helper() : Node() {};

            template<typename T_Head>
            constexpr explicit Tuple_Helper(T_Head &&now) : Node(forward<T_Head>(now)) {};

        };

//----------------------------------------------------------------------------------------------------------------------

        template<int Index, int Step, typename Head, typename ...Next>
        struct get_traits : public get_traits<Index - 1, Step + 1, Next...> {
        };

        template<int Step, typename Head, typename ...Next>
        struct get_traits<0, Step, Head, Next...> {
            using Type = typename Tuple_Helper<Step, Head, Next...>::Node::Type;

            using Ref = typename Tuple_Helper<Step, Head, Next...>::Node::Ref;

            using C_Ref = typename Tuple_Helper<Step, Head, Next...>::Node::C_Ref;
        };

    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename ...Args>
    struct Tuple : private Detail::Tuple_Helper<0, Args...> {
    private:
        using Self = Tuple<Args...>;

        using Basic = Detail::Tuple_Helper<0, Args...>;

    public:
        constexpr Tuple() : Basic() {};

        template<typename ...T_Next>
        explicit constexpr Tuple(T_Next &&...next) : Basic(forward<T_Next>(next)...) {};

        template<int Index>
        friend auto get(Self &target) -> typename Detail::get_traits<Index, 0, Args...>::Ref {
            static_assert(Index >= 0, "Index outside of Tuple!");
            return Detail::Node<Index,
                    typename Detail::get_traits<Index, 0, Args...>::Type>::get_value(target);
        }

        template<int Index>
        friend auto get(const Self &target) -> typename Detail::get_traits<Index, 0, Args...>::C_Ref {
            static_assert(Index >= 0, "Index outside Tuple!");
            return Detail::Node<Index,
                    typename Detail::get_traits<Index, 0, Args...>::Type>::get_value(target);
        }
    };

}

#endif //TINYSTL_TUPLE_HPP
