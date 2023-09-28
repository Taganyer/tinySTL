//
// Created by 86152 on 2023/9/8.
//

#ifndef TINYSTL_MEMORY_HPP
#define TINYSTL_MEMORY_HPP

#include "Allocater.hpp"

namespace STD {

    template<typename Arg>
    class Shared_ptr;

    template<typename Arg>
    class Weak_ptr;

    template<typename Arg, typename Deleter>
    class unique_ptr;

    struct basic_Value {
        virtual long long *get_weak_ptr_count() { return nullptr; };

        virtual Size *get_count_ptr() { return nullptr; };

        virtual void release() {};

        virtual ~basic_Value() = default;
    };

    template<typename Type>
    struct Value : public basic_Value {
        Type *target;

        long long *weak_ptr_count = Allocate<long long>(0);

        void (*const_deleter)(const Type *) = nullptr;

        void (*deleter)(Type *) = nullptr;

        Size *count = Allocate<Size>(1);

        Value(Type *target, void (*const_deleter)(const Type *)) :
                target(target), const_deleter(const_deleter ? const_deleter : constDeallocate<Type>) {};

        Value(Type *target, void (*deleter)(Type *)) :
                target(target), deleter(deleter ? deleter : Deallocate<Type>) {};

        long long *get_weak_ptr_count() override { return weak_ptr_count; };

        Size *get_count_ptr() override { return count; };

        void release() override {
            deleter ? deleter(target) : const_deleter(target);
            *weak_ptr_count = -(*weak_ptr_count);
            Deallocate(count);
            if (*weak_ptr_count == 0) Deallocate(weak_ptr_count);
        };

        ~Value() override = default;
    };


    //----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    class Shared_ptr {
    private:
        basic_Value *value;

        Arg *target;

        void release_test() {
            Size *count = value->get_count_ptr();
            --(*count);
            if (*count == 0) {
                value->release();
                Deallocate(value);
            }
        }

        Shared_ptr(basic_Value *value, Arg *target) : value(value), target(target) {};

    public:
        explicit Shared_ptr(Arg *target, void (*del)(const Arg *) = nullptr) :
                value(Allocate<Value<Arg>>(target, del)), target(target) {};

        explicit Shared_ptr(Arg *target, void (*del)(Arg *)) :
                value(Allocate<Value<Arg>>(target, del)), target(target) {};

        Shared_ptr(const Shared_ptr<Arg> &other);

        ~Shared_ptr() { release_test(); }

        void reset(Arg *ptr, void (*del)(Arg *arg) = nullptr);

        void reset(Arg *ptr, void (*del)(const Arg *arg));

        void reset(const Shared_ptr<Arg> &other);

        Shared_ptr<Arg> &operator=(const Shared_ptr<Arg> &other);

        Arg &operator*() const {
            if (!target)
                throw runtimeError("You have access to the null pointer\n");
            return *target;
        };

        Arg *operator->() const {
            if (!target)
                throw runtimeError("You have access to the null pointer\n");
            return target;
        }

        Arg *get() const { return target; };

        Size use_count() const { return *(value->get_count_ptr()); };

        bool unique() const { return *(value->get_count_ptr()) == 1; }

        //此模板不会进行安全检查，由于编译器不会自动调用以下四个函数，如需将智能指针转换，请显式调用。
        template<typename Target, typename Object>
        friend Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        //编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的结果。
        template<typename Other>
        operator Shared_ptr<Other>();

        friend class Weak_ptr<Arg>;
    };


    //----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Shared_ptr<Arg>::Shared_ptr(const Shared_ptr<Arg> &other) :
            value(other.value), target(other.target) { ++(*(other.value->get_count_ptr())); }

    template<typename Arg>
    void Shared_ptr<Arg>::reset(Arg *ptr, void (*del)(Arg *arg)) {
        release_test();
        value = Allocate<Value<Arg>>(ptr, del);
        target = ptr;
    }

    template<typename Arg>
    void Shared_ptr<Arg>::reset(Arg *ptr, void (*del)(const Arg *)) {
        release_test();
        value = Allocate<Value<Arg>>(ptr, del);
        target = ptr;
    }

    template<typename Arg>
    void Shared_ptr<Arg>::reset(const Shared_ptr<Arg> &other) {
        release_test();
        ++(*(other.value->get_count_ptr()));
        value = other.value;
        target = other.target;
    }

    template<typename Arg>
    Shared_ptr<Arg> &Shared_ptr<Arg>::operator=(const Shared_ptr<Arg> &other) {
        release_test();
        ++(*(other.value->get_count_ptr()));
        value = other.value;
        target = other.target;
        return *this;
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        auto *ptr = static_cast<Target *>(object.get());
        ++(*(object.value->get_count_ptr()));
        return Shared_ptr<Target>(object.value, ptr);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        auto *ptr = dynamic_cast<Target *>(object.get());
        ++(*(object.value->get_count_ptr()));
        return Shared_ptr<Target>(object.value, ptr);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        auto *ptr = const_cast<Target *>(object.get());
        ++(*(object.value->get_count_ptr()));
        return Shared_ptr<Target>(object.value, ptr);
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        auto *ptr = reinterpret_cast<Target *>(object.get());
        ++(*(object.value->get_count_ptr()));
        return Shared_ptr<Target>(object.value, ptr);
    }

    template<typename Arg>
    template<typename Other>
    Shared_ptr<Arg>::operator Shared_ptr<Other>() {
        return STD::static_pointer_cast<Other, Arg>(*this);
    }


    template<typename Arg>
    Shared_ptr<Arg> make_shared(const Arg &target) {
        auto ptr = new Arg(target);
        return Shared_ptr<Arg>(ptr);
    }

    template<typename Arg>
    Shared_ptr<Arg> make_shared(Arg &&target) {
        auto ptr = new Arg(move(target));
        return Shared_ptr<Arg>(ptr);
    }

    template<typename Arg, typename... Args>
    Shared_ptr<Arg> make_shared(Args &&...args) {
        auto ptr = new Arg(forward<Args>(args)...);
        return Shared_ptr<Arg>(ptr);
    }


    //----------------------------------------------------------------------------------------------------------------------


    template<typename Arg, typename Deleter = decltype(Deallocate<Arg>) *>
    class unique_ptr {
    private:
        Arg *target;

        Deleter deleter;

    public:
        unique_ptr(Arg *target, Deleter deleter = Deallocate<Arg>) : target(target), deleter(deleter) {};

        ~unique_ptr() { deleter(target); };

        Arg &operator*() const {
            if (!target)
                throw runtimeError("You have access to the null pointer\n");
            return *target;
        }

        Arg *operator->() const {
            if (!target)
                throw runtimeError("You have access to the null pointer\n");
            return target;
        }

        Arg *get() const { return target; };

        Arg *release() {
            target = nullptr;
            return target;
        }

        void reset(Arg *other = nullptr) {
            deleter(target);
            target = other;
        };
    };


    //----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    class Weak_ptr {
    private:
        basic_Value *basic;

        long long *counts;

        Arg *target;

        void release_test() {
            if (*counts > 0)
                --(*counts);
            else
                ++(*counts);
            if (*counts == 0)
                Deallocate(counts);
        }

        Weak_ptr(basic_Value *basic, long long *counts, Arg *target) :
                basic(basic), counts(counts), target(target) {};

    public:
        Weak_ptr(const Shared_ptr<Arg> &ptr) : target(ptr.target), counts(ptr.value->get_weak_ptr_count()),
                                               basic(ptr.value) { ++(*counts); };

        Weak_ptr(const Weak_ptr<Arg> &ptr) : target(ptr.target), counts(ptr.counts), basic(ptr.basic) {
            if (*counts > 0)
                ++(*counts);
            else
                --(*counts);
        };

        ~Weak_ptr() { release_test(); };

        bool expired() const { return *counts < 0; };

        Weak_ptr<Arg> &operator=(const Weak_ptr<Arg> &other) {
            release_test();
            counts = other.counts;
            target = other.target;
            if (*counts > 0)
                ++(*counts);
            else
                --(*counts);
            return *this;
        };

        Arg &operator*() const {
            if (expired() || !target)
                throw runtimeError("You have access to the null pointer\n");
            return *target;
        };

        Arg *operator->() const {
            if (expired() || !target)
                throw runtimeError("You have access to the null pointer\n");
            return target;
        };

        Shared_ptr<Arg> lock() const {
            if (expired() != target)
                return Shared_ptr<Arg>(nullptr);
            else
                return Shared_ptr<Arg>(basic, target);
        };

        void reset() {
            release_test();
            basic = counts = target = nullptr;
        }

        //此模板不会进行安全检查，由于编译器不会自动调用以下四个函数，如需将智能指针转换，请显式调用。
        template<typename Target, typename Object>
        friend Weak_ptr<Target> static_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Weak_ptr<Target> dynamic_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Weak_ptr<Target> const_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Weak_ptr<Target> reinterpret_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        //编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的结果。
        template<typename Other>
        operator Weak_ptr<Other>();
    };


    //----------------------------------------------------------------------------------------------------------------------


    template<typename Target, typename Object>
    Weak_ptr<Target> static_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        auto ptr = static_cast<Target *>(object.target);
        if (*object.counts > 0)
            ++(*object.counts);
        else
            --(*object.counts);
        return Weak_ptr<Target>(object.basic, object.counts, ptr);
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> dynamic_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        auto ptr = dynamic_cast<Target *>(object.target);
        if (*object.counts > 0)
            ++(*object.counts);
        else
            --(*object.counts);
        return Weak_ptr<Target>(object.basic, object.counts, ptr);
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> const_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        auto ptr = const_cast<Target *>(object.target);
        if (*object.counts > 0)
            ++(*object.counts);
        else
            --(*object.counts);
        return Weak_ptr<Target>(object.basic, object.counts, ptr);
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> reinterpret_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        auto ptr = reinterpret_cast<Target *>(object.target);
        if (*object.counts > 0)
            ++(*object.counts);
        else
            --(*object.counts);
        return Weak_ptr<Target>(object.basic, object.counts, ptr);
    }

    template<typename Arg>
    template<typename Other>
    Weak_ptr<Arg>::operator Weak_ptr<Other>() {
        return STD::static_pointer_cast<Other, Arg>(*this);
    }
}// namespace STD

#endif//TINYSTL_MEMORY_HPP
