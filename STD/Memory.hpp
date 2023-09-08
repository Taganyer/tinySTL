//
// Created by 86152 on 2023/9/8.
//

#ifndef TINYSTL_MEMORY_HPP
#define TINYSTL_MEMORY_HPP

#include "allocater.hpp"
#include "Move.hpp"

namespace STD {

    struct basic_Value {
        virtual void *get_ptr() { return nullptr; };

        virtual void release() {};

        virtual ~basic_Value() = default;
    };

    template<typename Type>
    struct Value : public basic_Value {
        Type *target;

        void (*deleter)(const Type *);

        Value(Type *target, void (*deleter)(const Type *)) :
                target(target), deleter(deleter ? deleter : constDeallocate<Type>) {};

        void *get_ptr() override { return target; };

        void release() override { deleter(target); };

        ~Value() override = default;
    };

    template<typename Arg>
    class Shared_ptr {
    private:
        Size *count;

        basic_Value *value;

        Arg *target;

        void release_test() {
            --(*count);
            if (!*count) {
                value->release();
                Deallocate(count);
                Deallocate(value);
            }
        }

        Shared_ptr(Size *count, basic_Value *value, Arg *target) : count(count), value(value), target(target) {};

    public:

        Shared_ptr(Arg *target, void (*del)(const Arg *) = nullptr) :
                count(Allocate<Size>(1)), value(new Value<Arg>(target, del)), target(target) {};

        Shared_ptr(const Shared_ptr<Arg> &other);

        ~Shared_ptr() { release_test(); }

        Shared_ptr<Arg> reset(const Shared_ptr<Arg> &other);

        Shared_ptr<Arg> &operator=(const Shared_ptr<Arg> &other);

        Arg &operator*() const { return *target; };

        Arg &operator->() const { return *target; };

        Arg *get() const { return target; };

        Size use_count() const { return *count; };

        void swap(Shared_ptr<Arg> &other);

        template<typename Target, typename Object>
        friend Shared_ptr<Target> static_pointer_cast(Shared_ptr<Object> object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> dynamic_pointer_cast(Shared_ptr<Object> object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> const_pointer_cast(Shared_ptr<Object> object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> reinterpret_pointer_cast(Shared_ptr<Object> object) noexcept;

        template<typename Other>
        operator Shared_ptr<Other>();

    };


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Shared_ptr<Arg>::Shared_ptr(const Shared_ptr<Arg> &other) :
            count(other.count), value(other.value), target(other.target) { ++(*other.count); }

    template<typename Arg>
    Shared_ptr<Arg> Shared_ptr<Arg>::reset(const Shared_ptr<Arg> &other) {
        release_test();
        ++(*other.count);
        count = other.count;
        value = other.value;
        target = other.target;
    }

    template<typename Arg>
    Shared_ptr<Arg> &Shared_ptr<Arg>::operator=(const Shared_ptr<Arg> &other) {
        release_test();
        ++(*other.count);
        count = other.count;
        value = other.value;
        target = other.target;
        return *this;
    }

    template<typename Arg>
    void Shared_ptr<Arg>::swap(Shared_ptr<Arg> &other) {
        if (this == &other) return;
        STD::swap(value, other.value);
        STD::swap(count, other.count);
        STD::swap(target, other.target);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> static_pointer_cast(Shared_ptr<Object> object) noexcept {
        auto *ptr = static_cast<Target*>(object.get());
        ++(*object.count);
        return Shared_ptr<Target>(object.count, object.value, ptr);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> dynamic_pointer_cast(Shared_ptr<Object> object) noexcept {
        auto *ptr = dynamic_cast<Target*>(object.get());
        ++(*object.count);
        return Shared_ptr<Target>(object.count, object.value, ptr);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> const_pointer_cast(Shared_ptr<Object> object) noexcept {
        auto *ptr = const_cast<Target*>(object.get());
        ++(*object.count);
        return Shared_ptr<Target>(object.count, object.value, ptr);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> reinterpret_pointer_cast(Shared_ptr<Object> object) noexcept {
        auto *ptr = reinterpret_cast<Target*>(object.get());
        ++(*object.count);
        return Shared_ptr<Target>(object.count, object.value, ptr);
    }

    template<typename Arg>
    template<typename Other>
    Shared_ptr<Arg>::operator Shared_ptr<Other>() {
        return STD::static_pointer_cast<Other, Arg>(*this);
    }


    template<typename Arg>
    Shared_ptr<Arg> make_shared(const Arg& target) {
        auto ptr = new Arg(target);
        return Shared_ptr<Arg>(ptr);
    }

    template<typename Arg, typename ...Args>
    Shared_ptr<Arg> make_shared(Args ...args) {
        auto ptr = new Arg(args...);
        return Shared_ptr<Arg>(ptr);
    }
}

#endif //TINYSTL_MEMORY_HPP
