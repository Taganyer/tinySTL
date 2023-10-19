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
    class Unique_ptr;

    template<typename Target, typename Object>
    inline Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, static_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, dynamic_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, const_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept {
        return Shared_ptr<Target>(object.basic, reinterpret_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Weak_ptr<Target> static_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, static_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Weak_ptr<Target> dynamic_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, dynamic_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Weak_ptr<Target> const_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, const_cast<Target *>(object.value));
    }

    template<typename Target, typename Object>
    inline Weak_ptr<Target> reinterpret_pointer_cast(const Weak_ptr<Object> &object) noexcept {
        return Weak_ptr<Target>(object.basic, reinterpret_cast<Target *>(object.value));
    }

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
            if (basic->release(value, true))
                Deallocate(basic);
        };

    public:
        template<typename Deleter = Default_delete<Arg>>
        Shared_ptr(Arg *target = nullptr, Deleter del = Deleter()) : value(target) {
            if (target) {
                basic = Allocate<Detail::Value<Arg, Deleter>>(del);
                ++basic->shared_ptr_count;
            } else basic = nullptr;
        };

        //注意，这个函数必不可少，在复制和自身相同的类时，下面的万能模板构造函数并不会被调用。
        Shared_ptr(const Shared_ptr<Arg> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        template<typename Type>
        explicit Shared_ptr(const Shared_ptr<Type> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        ~Shared_ptr() { release(); }

        template<typename Del = Default_delete<Arg>>
        void reset(Arg *ptr, Del del = Del()) {
            release();
            basic = Allocate<Detail::Value<Arg, Del>>(del);
            value = ptr;
            ++basic->shared_ptr_count;
        };

        template<typename Type>
        void reset(const Shared_ptr<Type> &other) {
            if (basic == other.basic) return;
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

        template<typename Type>
        Shared_ptr<Arg> &operator=(const Shared_ptr<Type> &other) {
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

        Arg &operator[](Step index) const {
            return *(value + index);
        }

        Arg *get() const { return value; };

        unsigned use_count() const { return basic->shared_ptr_count; };

        bool unique() const { return basic->shared_ptr_count == 1; }

        // 由于编译器不会自动调用以下四个函数，如需将智能指针转换，请显式调用。
        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        operator bool() const {
            return basic;
        }

        template<typename Arg1, typename Arg2>
        inline friend bool operator==(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator!=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        inline friend bool operator-(const Shared_ptr<Arg> &left, const Shared_ptr<Arg> &right) {
            return left.value - right.value;
        }

        friend class Weak_ptr<Arg>;

        template<typename Type>
        friend class Shared_ptr;

    };

    template<>
    class Shared_ptr<void> {
    private:
        Detail::basic_Value *basic = nullptr;

        void *value = nullptr;

        explicit Shared_ptr(Detail::basic_Value *value, void *val) : basic(value), value(val) {
            if (basic) ++basic->shared_ptr_count;
        };

        void release() const {
            if (!basic) return;
            --basic->shared_ptr_count;
            if (basic->release(value, true))
                Deallocate(basic);
        };

    public:
        Shared_ptr() = default;

        Shared_ptr(const Shared_ptr<void> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        template<typename Arg>
        explicit Shared_ptr(const Shared_ptr<Arg> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        ~Shared_ptr() { release(); }

        template<typename Arg, typename Del = Default_delete<Arg>>
        void reset(void *ptr, Del del = Del()) {
            release();
            basic = Allocate<Detail::Value<Arg, Del>>(del);
            value = ptr;
            ++basic->shared_ptr_count;
        };

        template<typename Arg>
        void reset(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
        };

        Shared_ptr<void> &operator=(const Shared_ptr<void> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        template<typename Arg>
        Shared_ptr<void> &operator=(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        void *get() const { return value; };

        unsigned use_count() const { return basic->shared_ptr_count; };

        bool unique() const { return basic->shared_ptr_count == 1; }

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        // 编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的转化结果。
        template<typename Other>
        operator Shared_ptr<Other>() const {
            return STD::static_pointer_cast<Other, void>(*this);
        };

        operator bool() const {
            return basic;
        }

        template<typename Arg1, typename Arg2>
        inline friend bool operator==(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator!=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg>
        friend class Weak_ptr;

        template<typename Arg>
        friend class Shared_ptr;

    };

    template<>
    class Shared_ptr<const void> {
    private:
        Detail::basic_Value *basic = nullptr;

        const void *value = nullptr;

        explicit Shared_ptr(Detail::basic_Value *value, const void *val) : basic(value), value(val) {
            if (basic) ++basic->shared_ptr_count;
        };

        void release() const {
            if (!basic) return;
            --basic->shared_ptr_count;
            if (basic->release(value, true)) Deallocate(basic);
        };

    public:
        Shared_ptr() = default;

        Shared_ptr(const Shared_ptr<const void> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        template<typename Arg>
        explicit Shared_ptr(const Shared_ptr<Arg> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->shared_ptr_count;
        };

        ~Shared_ptr() { release(); }

        template<typename Arg, typename Del = Default_delete<Arg>>
        void reset(Arg *ptr, Del del = Del()) {
            release();
            basic = Allocate<Detail::Value<Arg, Del>>(del);
            value = ptr;
            ++basic->shared_ptr_count;
        };

        template<typename Arg>
        void reset(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
        };

        Shared_ptr<const void> &operator=(const Shared_ptr<const void> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        template<typename Arg>
        Shared_ptr<const void> &operator=(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->shared_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        const void *get() const { return value; };

        unsigned use_count() const { return basic->shared_ptr_count; };

        bool unique() const { return basic->shared_ptr_count == 1; }

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> static_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> dynamic_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> const_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        template<typename Target, typename Object>
        inline friend Shared_ptr<Target> reinterpret_pointer_cast(const Shared_ptr<Object> &object) noexcept;

        // 编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的转化结果。
        template<typename Other>
        operator Shared_ptr<Other>() const {
            return STD::static_pointer_cast<Other, const void>(*this);
        };

        operator bool() const {
            return basic;
        }

        template<typename Arg1, typename Arg2>
        inline friend bool operator==(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator!=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right);

        template<typename Arg>
        friend class Weak_ptr;

        template<typename Arg>
        friend class Shared_ptr;

    };

    template<typename Arg1, typename Arg2>
    inline bool operator==(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right) {
        return left.basic == right.basic;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator!=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right) {
        return left.basic != right.basic;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator<(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right) {
        return left.value < right.value;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator<=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right) {
        return left.value <= right.value;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator>(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right) {
        return left.value > right.value;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator>=(const Shared_ptr<Arg1> &left, const Shared_ptr<Arg2> &right) {
        return left.basic >= right.basic;
    }

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

        template<typename Type>
        explicit Weak_ptr(const Shared_ptr<Type> &ptr) : basic(ptr.basic), value(ptr.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        //注意，这个函数必不可少，在复制和自身相同的类时，下面的万能模板构造函数并不会被调用。
        Weak_ptr(const Weak_ptr<Arg> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        template<typename Type>
        explicit Weak_ptr(const Weak_ptr<Type> &other) : basic(other.basic), value(other.value) {
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

        template<typename Type>
        Weak_ptr<Arg> &operator=(const Weak_ptr<Type> &other) {
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

        Arg &operator[](Step index) const {
            return *(value + index);
        }

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
            if (basic == other.basic) return;
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

        operator bool() const {
            return basic;
        }

        template<typename Arg1, typename Arg2>
        friend inline bool operator==(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend inline bool operator!=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend inline bool operator<(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend inline bool operator<=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend inline bool operator>(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend inline bool operator>=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend inline Ptrdiff operator-(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
            return left.value - right.value;
        }

    };

    template<>
    class Weak_ptr<void> {
    private:
        Detail::basic_Value *basic = nullptr;

        void *value = nullptr;

        void release() {
            if (!basic) return;
            --basic->weak_ptr_count;
            if (basic->release(value, false)) Deallocate(basic);
        }

        explicit Weak_ptr(Detail::basic_Value *basic, void *val) : basic(basic), value(val) {
            if (basic) ++basic->weak_ptr_count;
        };

    public:
        Weak_ptr() = default;

        template<typename Type>
        explicit Weak_ptr(const Shared_ptr<Type> &ptr) : basic(ptr.basic), value(ptr.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        Weak_ptr(const Weak_ptr<void> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        template<typename Type>
        explicit Weak_ptr(const Weak_ptr<Type> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        ~Weak_ptr() { release(); };

        bool expired() const { return !basic->shared_ptr_count; };

        Weak_ptr<void> &operator=(const Weak_ptr<void> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        template<typename Arg>
        Weak_ptr<void> &operator=(const Weak_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        Shared_ptr<void> lock() const {
            if (expired()) return Shared_ptr<void>(nullptr, nullptr);
            else return Shared_ptr<void>(basic, value);
        };

        void reset() {
            release();
            basic = nullptr;
            value = nullptr;
        }

        template<typename Arg>
        void reset(const Weak_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
        }

        template<typename Arg>
        void reset(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
        }

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
            return STD::static_pointer_cast<Other, void>(*this);
        }

        operator bool() const {
            return basic;
        }

        template<typename Arg1, typename Arg2>
        friend bool operator==(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend bool operator!=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend bool operator<(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend bool operator<=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend bool operator>(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        friend bool operator>=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

    };

    template<>
    class Weak_ptr<const void> {
    private:
        Detail::basic_Value *basic = nullptr;

        const void *value = nullptr;

        void release() {
            if (!basic) return;
            --basic->weak_ptr_count;
            if (basic->release(value, false)) Deallocate(basic);
        }

        explicit Weak_ptr(Detail::basic_Value *basic, const void *val) : basic(basic), value(val) {
            if (basic) ++basic->weak_ptr_count;
        };

    public:
        Weak_ptr() = default;

        template<typename Type>
        explicit Weak_ptr(const Shared_ptr<Type> &ptr) : basic(ptr.basic), value(ptr.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        Weak_ptr(const Weak_ptr<const void> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        template<typename Type>
        explicit Weak_ptr(const Weak_ptr<Type> &other) : basic(other.basic), value(other.value) {
            if (basic) ++basic->weak_ptr_count;
        };

        ~Weak_ptr() { release(); };

        bool expired() const { return !basic->shared_ptr_count; };

        Weak_ptr<const void> &operator=(const Weak_ptr<const void> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        template<typename Arg>
        Weak_ptr<const void> &operator=(const Weak_ptr<Arg> &other) {
            if (basic == other.basic) return *this;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
            return *this;
        };

        Shared_ptr<const void> lock() const {
            if (expired()) return Shared_ptr<void>(nullptr, nullptr);
            else return Shared_ptr<const void>(basic, value);
        };

        void reset() {
            release();
            basic = nullptr;
            value = nullptr;
        }

        template<typename Arg>
        void reset(const Weak_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
        }

        template<typename Arg>
        void reset(const Shared_ptr<Arg> &other) {
            if (basic == other.basic) return;
            release();
            if (other.basic) ++other.basic->weak_ptr_count;
            basic = other.basic;
            value = other.value;
        }

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
            return STD::static_pointer_cast<Other, const void>(*this);
        }

        operator bool() const {
            return basic;
        }

        template<typename Arg1, typename Arg2>
        inline friend bool operator==(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator!=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator<=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

        template<typename Arg1, typename Arg2>
        inline friend bool operator>=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right);

    };

    template<typename Arg1, typename Arg2>
    inline bool operator==(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
        return left.basic == right.basic;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator!=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
        return left.basic != right.basic;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator<(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
        return left.value < right.value;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator<=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
        return left.value <= right.value;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator>(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
        return left.value > right.value;
    }

    template<typename Arg1, typename Arg2>
    inline bool operator>=(const Weak_ptr<Arg1> &left, const Weak_ptr<Arg2> &right) {
        return left.value >= right.value;
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, typename Deleter = Default_delete<Arg>>
    class Unique_ptr {
    private:
        Arg *target = nullptr;

        Deleter deleter;

    public:
        Unique_ptr(Arg *target = nullptr, Deleter deleter = Deleter()) : target(target), deleter(deleter) {};

        Unique_ptr(const Arg &value, Deleter deleter = Deleter()) :
                target(new Arg(value)), deleter(deleter) {};

        Unique_ptr(Arg &&value, Deleter deleter = Deleter()) :
                target(new Arg(move(value))), deleter(deleter) {};

        Unique_ptr(const Unique_ptr<Arg, Deleter> &) = delete;

        Unique_ptr(Unique_ptr<Arg, Deleter> &&other) noexcept:
                target(other.target), deleter(other.deleter) {
            other.target = nullptr;
        };

        template<typename Type, typename T_Deleter>
        Unique_ptr(Unique_ptr<Type, T_Deleter> &&other, Deleter deleter = Deleter()) noexcept :
                target(other.target), deleter(deleter) {
            other.target = nullptr;
        }

        ~Unique_ptr() { deleter(target); };

        Arg &operator*() const {
            return *target;
        }

        Arg *operator->() const {
            return target;
        }

        Arg &operator[](Step index) const {
            return *(target + index);
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

        Unique_ptr<Arg, Deleter>
        &operator=(const Unique_ptr<Arg, Deleter> &) = delete;

        Unique_ptr<Arg, Deleter>
        &operator=(Unique_ptr<Arg, Deleter> &&other) noexcept {
            deleter(target);
            target = other.target;
            other.target = nullptr;
            return *this;
        }

        template<typename Type, typename T_Deleter>
        Unique_ptr<Arg, Deleter>
        &operator=(Unique_ptr<Type, T_Deleter> &&other) noexcept {
            target = other.target;
            other.target = nullptr;
            return *this;
        }

        operator bool() const {
            return target;
        }

        template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
        friend inline bool operator==(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right);

        template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
        friend inline bool operator!=(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right);

        template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
        friend inline bool operator<(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right);

        template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
        friend inline bool operator<=(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right);

        template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
        friend inline bool operator>(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right);

        template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
        friend inline bool operator>=(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right);

        template<typename Deleter1, typename Deleter2>
        friend Ptrdiff operator-(const Unique_ptr<Arg, Deleter1> &left, const Unique_ptr<Arg, Deleter2> &right) {
            return left.target - right.target;
        }

        template<typename Type, typename T_Deleter>
        friend
        class Unique_ptr;

    };

    template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
    inline bool operator==(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right) {
        return left.target == right.target;
    }

    template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
    inline bool operator!=(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right) {
        return left.target != right.target;
    }

    template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
    inline bool operator<(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right) {
        return left.target < right.target;
    }

    template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
    inline bool operator<=(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right) {
        return left.target <= right.target;
    }

    template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
    inline bool operator>(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right) {
        return left.target > right.target;
    }

    template<typename Arg1, typename Arg2, typename Deleter1, typename Deleter2>
    inline bool operator>=(const Unique_ptr<Arg1, Deleter1> &left, const Unique_ptr<Arg2, Deleter2> &right) {
        return left.target >= right.target;
    }


}// namespace STD

#endif//TINYSTL_MEMORY_HPP
