//
// Created by 86152 on 2023/9/21.
//

#ifndef TINYSTL_QUEUE_HPP
#define TINYSTL_QUEUE_HPP

#include "Deque.hpp"

namespace STD {

    template<typename Arg, class Container = Deque<Arg>>
    class Queue {
    private:

        Container target = Container();

    public:

        Queue() = default;

        Queue(const Container &target) : target(target) {};

        Queue(Container &&target) : target(move(target)) {};

        Arg &top() const {
            return target.front();
        };

        void pop() {
            target.pop_front();
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

#endif //TINYSTL_QUEUE_HPP
