//
// Created by 86152 on 2023/9/21.
//

#ifndef TINYSTL_QUEUE_HPP
#define TINYSTL_QUEUE_HPP

#include "Deque.hpp"
#include "Vector.hpp"
#include "Function.hpp"
#include "Algorithms/Algorithms2.hpp"

namespace STD {

    template<typename Arg, class Container = Deque<Arg>>
    class Queue {
    private:
        Container target = Container();

    public:
        Queue() = default;

        Queue(const Container &target) : target(target) {};

        Queue(Container &&target) : target(move(target)) {};

        template<typename Input_iterator>
        Queue(const Input_iterator &begin, const Input_iterator &end) : target(Container(begin, end)) {};

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

        template<typename Input_iterator>
        Priority_queue(const Input_iterator &begin, const Input_iterator &end, Compare compare = Less<Arg>()) : target(
                Container(begin, end)), compare(compare) {
            Sort(target.begin(), target.end(), compare);
        };

        Arg &front() const {
            return target.back();
        };

        void pop() {
            target.pop_back();
        }

        void push(const Arg &val) {
            target.insert(Binary_Search(target.begin(), target.end(), val), val);
        }

        template<typename ...args>
        void emplace(args &&...vals) {
            Arg val = Arg(vals...);
            target.emplace(Binary_Search(target.begin(), target.end(), val), val);
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
