//
// Created by 86152 on 2023/10/23.
//

#ifndef TINYSTL_INSERTER_HPP
#define TINYSTL_INSERTER_HPP

#include "Iter.hpp"
#include "../Move.hpp"

namespace STD {
    template<typename Container>
    class Back_Inserter_Iterator : Iterator<Output_iterator_tag, void, void, void, void> {
    private:
        Container *container;

    public:
        Back_Inserter_Iterator(Container &target) : container(&target) {};

        Back_Inserter_Iterator<Container> &operator=(const typename Container::Value_type &val) {
            container->push_back(val);
            return *this;
        }

        Back_Inserter_Iterator<Container> &operator=(typename Container::Value_type &&val) {
            container->push_back(move(val));
            return *this;
        }

        constexpr Back_Inserter_Iterator<Container> &operator*() const {
            return *this;
        }

        constexpr Back_Inserter_Iterator<Container> &operator++() const {
            return *this;
        }

        constexpr Back_Inserter_Iterator<Container> &operator++(int) const {
            return *this;
        }
    };

    template<typename Container>
    inline Back_Inserter_Iterator<Container> Front_Inserter(Container &target) {
        return Back_Inserter_Iterator<Container>(target);
    }

    template<typename Container>
    class Front_Inserter_Iterator : Iterator<Output_iterator_tag, void, void, void, void> {
    private:
        Container *container;

    public:
        Front_Inserter_Iterator(Container &target) : container(&target) {};

        Front_Inserter_Iterator<Container> &operator=(const typename Container::Value_type &val) {
            container->push_front(val);
            return *this;
        }

        Front_Inserter_Iterator<Container> &operator=(typename Container::Value_type &&val) {
            container->push_front(move(val));
            return *this;
        }

        constexpr Front_Inserter_Iterator<Container> &operator*() const {
            return *this;
        }

        constexpr Front_Inserter_Iterator<Container> &operator++() const {
            return *this;
        }

        constexpr Front_Inserter_Iterator<Container> &operator++(int) const {
            return *this;
        }
    };

    template<typename Container>
    inline Front_Inserter_Iterator<Container> Front_Inserter(Container &target) {
        return Front_Inserter_Iterator<Container>(target);
    }

    template<typename Container>
    class Inserter_Iterator : Iterator<Output_iterator_tag, void, void, void, void> {
    private:
        Container *container;

        typename Container::Iterator iterator;

    public:
        Inserter_Iterator(Container &target, typename Container::Iterator iterator) :
                container(&target), iterator(iterator) {};

        Inserter_Iterator<Container> &operator=(const typename Container::Value_type &val) {
            container->insert(iterator, val);
            return *this;
        }

        Inserter_Iterator<Container> &operator=(typename Container::Value_type &&val) {
            container->push_front(iterator, move(val));
            return *this;
        }

        constexpr Inserter_Iterator<Container> &operator*() const {
            return *this;
        }

        constexpr Inserter_Iterator<Container> &operator++() const {
            return *this;
        }

        constexpr Inserter_Iterator<Container> &operator++(int) const {
            return *this;
        }
    };

    template<typename Container>
    inline Inserter_Iterator<Container> Inserter(Container &target, typename Container::Iterator iterator) {
        return Inserter_Iterator<Container>(target, iterator);
    }

}

#endif //TINYSTL_INSERTER_HPP
