//
// Created by 86152 on 2023/9/21.
//

#ifndef TINYSTL_QUEUE_HPP
#define TINYSTL_QUEUE_HPP

#include "Deque.hpp"
#include "Vector.hpp"
#include "Function.hpp"

namespace STD {

    template<typename Arg, class Container = Deque<Arg>>
    class Queue {
    private:

        Container target = Container();

    public:

        Queue() = default;

        Queue(const Container &target) : target(target) {};

        Queue(Container &&target) : target(move(target)) {};

        Queue(const Iter<Arg> &begin, const Iter<Arg> &end) : target(Container(begin, end)) {};

        Queue(const cIter<Arg> &begin, const cIter<Arg> &end) : target(Container(begin, end)) {};

        Arg &front() const {
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

    template<typename Arg, class Container = Vector<Arg>, typename Compare = Less<Arg>>
    class Priority_queue {
    private:

        Container target = Container();

        Compare compare;

    public:

        Priority_queue() : compare(Less<Arg>()) {};

        Priority_queue(Compare compare) : compare(compare) {};

        Priority_queue(const Container &target, Compare compare = Less<Arg>()) : target(target), compare(compare) {};

        Priority_queue(Container &&target, Compare compare = Less<Arg>()) : target(move(target)), compare(compare) {};

        Priority_queue(const Iter<Arg> &begin, const Iter<Arg> &end, Compare compare = Less<Arg>()) : target(
                Container(begin, end)), compare(compare) {};

        Priority_queue(const cIter<Arg> &begin, const cIter<Arg> &end, Compare compare = Less<Arg>()) : target(
                Container(begin, end)), compare(compare) {};

        Arg &front() const {
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
