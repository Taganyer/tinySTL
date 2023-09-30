//
// Created by 86152 on 2023/9/21.
//

#ifndef TINYSTL_STACK_HPP
#define TINYSTL_STACK_HPP

#include "Vector.hpp"

namespace STD {
    template<typename Arg, class Container = Vector<Arg>>
    class Stack {
    private:
        Container target = Container();

    public:
        Stack() = default;

        Stack(const Container &target) : target(target) {};

        Stack(Container &&target) : target(move(target)) {};

        template<typename Input_iterator>
        Stack(const Input_iterator &begin, const Input_iterator &end) : target(Container(begin, end)) {};

        Arg &top() const {
            return target.back();
        };

        void pop() {
            target.pop_back();
        }

        void push(const Arg &val) {
            target.push_back(val);
        }

        void push(Arg &&val) {
            target.push_back(move(val));
        }

        template<typename ...args>
        void emplace(args &&...vals) {
            target.emplace_back(move(vals)...);
        }

        bool empty() const {
            return target.empty();
        }

        Size size() const {
            return target.size();
        }

    };
}
#endif //TINYSTL_STACK_HPP
