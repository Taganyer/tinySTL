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

    template<typename Target, typename Object>
    Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, static_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, dynamic_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, const_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, reinterpret_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> static_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, static_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> dynamic_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, dynamic_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> const_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, const_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    Weak_ptr<Target> reinterpret_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, reinterpret_cast<Target *>(object.value));
    }

    namespace Detail {

        struct basic_Value {
            unsigned weak_ptr_count = 0;

            unsigned shared_ptr_count = 0;

            virtual bool release(const void *target, bool is_Shared_ptr) = 0;

            virtual ~basic_Value() = default;
        };

        template<typename Type, typename Delete>
        struct Value : public basic_Value {
            Delete deleter;

            explicit Value(Delete deleter) : deleter(deleter) {};

            bool release(const void *target, bool is_Shared_ptr) override {
                if (is_Shared_ptr && !shared_ptr_count)
                    deleter(const_cast<Type *>(static_cast<const Type *>(target)));
                if (!shared_ptr_count && !weak_ptr_count) return true;
                else return false;
            };

            ~Value() override = default;
        };

    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Shared_ptr {
    private:
        Detail::basic_Value *basic;

        Arg *value;

        explicit Shared_ptr(Detail::basic_Value *value, Arg *val) : basic(value), value(val) {
            if (basic) ++basic->shared_ptr_count;
        };

        void release() const {
            if (!basic) return;
            --basic->shared_ptr_count;
            if (basic->release(value, true)) Deallocate(basic);
        };

    public:
        template<typename Deleter = Default_delete<Arg>>
        Shared_ptr(Arg *target = nullptr, Deleter del = Deleter()) : value(target) {
            if (target) {
                basic = Allocate<Detail::Value<Arg, Deleter>>(del);
                ++basic->shared_ptr_count;
            } else basic = nullptr;
        };

        Shared_ptr(const Shared_ptr<Arg> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        ~Shared_ptr() { release(); }

        template<typename Del = Default_delete<Arg>>
        void reset(Arg *ptr, Del del = Del()) {
            release();
            basic = Allocate<Detail::Value<Arg, Del>>(del);
            value = ptr;
        };

        void reset(const Shared_ptr<Arg> &other) {
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
        };

        Shared_ptr<Arg> &operator=(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        Arg &operator*() const {
            return *value;
        };

        Arg *operator->() const {
            return value;
        }

        Arg *get() const { return value; };

        unsigned use_count() const { return basic->shared_ptr_count; };

        bool unique() const { return basic->shared_ptr_count == 1; }

        // 由于编译器不会自动调用以下四个函数，如需将智能指针转换，请显式调用。
        template<typename Target, typename Object>
        friend Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        // 编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的转化结果。
        template<typename Other>
        operator Shared_ptr<Other>() const {
            return STD::static_pointer_cast<Other, Arg>(*this);
        };

        operator bool() const {
            return basic;
        }

        friend bool operator==(const Shared_ptr<Arg> &left, const Shared_ptr<Arg> &right) {
            return left.basic == right.basic;
        }

        friend bool operator!=(const Shared_ptr<Arg> &left, const Shared_ptr<Arg> &right) {
            return left.basic != right.basic;
        }

        friend class Weak_ptr<Arg>;
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, typename Deleter = Default_delete<Arg>>
    Shared_ptr<Arg> make_shared(const Arg &target, Deleter deleter = Deleter()) {
        auto ptr = new Arg(target);
        return Shared_ptr<Arg>(ptr, deleter);
    }

    template<typename Arg, typename Deleter = Default_delete<Arg>>
    Shared_ptr<Arg> make_shared(Arg &&target, Deleter deleter = Deleter()) {
        auto ptr = new Arg(move(target));
        return Shared_ptr<Arg>(ptr, deleter);
    }

    template<typename Arg, typename... Args>
    Shared_ptr<Arg> make_shared(Args &&...args) {
        auto ptr = new Arg(forward<Args>(args)...);
        return Shared_ptr<Arg>(ptr);
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, typename Deleter = Default_delete<Arg>>
    class unique_ptr {
    private:
        Arg *target;

        Deleter deleter;

    public:
        unique_ptr(Arg *target, Deleter deleter = Deleter()) : target(target), deleter(deleter) {};

        ~unique_ptr() { deleter(target); };

        Arg &operator*() const {
            return *target;
        }

        Arg *operator->() const {
            return target;
        }

        Arg *get() const { return target; };

        Arg *release() {
            Arg *temp = target;
            target = nullptr;
            return temp;
        }

        void reset(Arg *other = nullptr) {
            deleter(target);
            target = other;
        };

        operator bool() const {
            return target;
        }

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Weak_ptr {
    private:
        Detail::basic_Value *basic = nullptr;

        Arg *value = nullptr;

        void release() {
            if (!basic) return;
            --basic->weak_ptr_count;
            if (basic->release(value, false)) Deallocate(basic);
        }

        explicit Weak_ptr(Detail::basic_Value *basic, Arg *val) : basic(basic), value(val) {
            if (basic) ++basic->weak_ptr_count;
        };

    public:
        Weak_ptr() = default;

        Weak_ptr(const Shared_ptr<Arg> &ptr) : basic(ptr.basic), value(ptr.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        Weak_ptr(const Weak_ptr<Arg> &ptr) : basic(ptr.basic), value(ptr.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        ~Weak_ptr() { release(); };

        bool expired() const { return !basic->shared_ptr_count; };

        Weak_ptr<Arg> &operator=(const Weak_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        Arg &operator*() const {
            return *value;
        };

        Arg *operator->() const {
            return value;
        };

        Shared_ptr<Arg> lock() const {
            if (expired()) return Shared_ptr<Arg>(nullptr, nullptr);
            else return Shared_ptr<Arg>(basic, value);
        };

        void reset() {
            release();
            basic = nullptr;
            value = nullptr;
        }

        void reset(const Weak_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
        }

        void reset(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
        }

        // 由于编译器不会自动调用以下四个函数，如需将智能指针转换，请显式调用。
        template<typename Target, typename Object>
        friend Weak_ptr<Target> static_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Weak_ptr<Target> dynamic_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Weak_ptr<Target> const_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        friend Weak_ptr<Target> reinterpret_pointer_cast(const Weak_ptr<Object> &object) noexcept;

        // 编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的转化结果。
        template<typename Other>
        operator Weak_ptr<Other>() const {
            return STD::static_pointer_cast<Other, Arg>(*this);
        };

        operator bool() const {
            return basic;
        }

        friend bool operator==(const Weak_ptr<Arg> &left, const Weak_ptr<Arg> &right) {
            return left.basic == right.basic;
        }

        friend bool operator!=(const Weak_ptr<Arg> &left, const Weak_ptr<Arg> &right) {
            return left.basic != right.basic;
        }

    };

}// namespace STD

#endif//TINYSTL_MEMORY_HPP
