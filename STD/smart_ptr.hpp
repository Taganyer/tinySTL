////
//// Created by 86152 on 2023/8/25.
////
//
//#ifndef TINYSTL_SMART_PTR_HPP
//#define TINYSTL_SMART_PTR_HPP
//
//#include "allocater.hpp"
//#include "Move.hpp"
//
//namespace STD {
//    template<typename Arg>
//    class Shared_ptr {
//    private:
//        Size *count;
//
//        Arg *target;
//
//        void (*deleter) (Arg*);
//
//        inline explicit Shared_ptr(Arg *target, Size *count, void (*del) (Arg*) = Deallocate<Arg>);
//
//    public:
//        explicit Shared_ptr(const Arg &val, void (*del) (Arg*) = Deallocate<Arg>);
//
//        explicit Shared_ptr(Arg &&val, void (*del) (Arg*) = Deallocate<Arg>);
//
//        Shared_ptr(const Shared_ptr<Arg> &other);
//
//        ~Shared_ptr();
//
//        Shared_ptr<Arg> reset(const Shared_ptr<Arg> &other);
//
//        Shared_ptr<Arg> &operator=(const Shared_ptr<Arg> &other);
//
//        Arg &operator*() const { return *target; };
//
//        Arg &operator->() const { return *target; };
//
//        Arg *get() const { return target; };
//
//        Size use_count() { return *count; };
//
//        void swap(Shared_ptr<Arg> &other);
//
//        //此模板不会进行安全检查，由于编译器不会自动调用以下四个函数，如需将智能指针转换，请显式调用。
//        template<typename Target, typename Object>
//        friend Shared_ptr<Target> static_pointer_cast(Shared_ptr<Object> object) noexcept;
//
//        template<typename Target, typename Object>
//        friend Shared_ptr<Target> dynamic_pointer_cast(Shared_ptr<Object> object) noexcept;
//
//        template<typename Target, typename Object>
//        friend Shared_ptr<Target> const_pointer_cast(Shared_ptr<Object> object) noexcept;
//
//        template<typename Target, typename Object>
//        friend Shared_ptr<Target> reinterpret_pointer_cast(Shared_ptr<Object> object) noexcept;
//
//        //编译器会自动调用该类型转换（默认使用static_pointer_cast实现），可能会有意外的结果。
//        template<typename Other>
//        operator Shared_ptr<Other>();
//    };
//
////----------------------------------------------------------------------------------------------------------------------
//
//    template<typename Target, typename Object>
//    Shared_ptr<Target> static_pointer_cast(Shared_ptr<Object> object) noexcept {
//        auto *ptr = static_cast<Target*>(object.get());
//        ++(*object.count);
//        return Shared_ptr<Target>(ptr, object.count, Deallocate);
//    }
//
//    template<typename Target, typename Object>
//    Shared_ptr<Target> dynamic_pointer_cast(Shared_ptr<Object> object) noexcept {
//        auto *ptr = static_cast<Target*>(object.get());
//        ++(*object.count);
//        return Shared_ptr<Target>(ptr, object.count, Deallocate);
//    }
//
//    template<typename Target, typename Object>
//    Shared_ptr<Target> const_pointer_cast(Shared_ptr<Object> object) noexcept {
//        auto *ptr = const_cast<Target*>(object.get());
//        ++(*object.count);
//        return Shared_ptr<Target>(ptr, object.count, Deallocate);
//    }
//
//    template<typename Target, typename Object>
//    Shared_ptr<Target> reinterpret_pointer_cast(Shared_ptr<Object> object) noexcept {
//        auto *ptr = reinterpret_cast<Target*>(object.get());
//        *(++object.count);
//        return Shared_ptr<Target>(ptr, object.count, Deallocate);
//    }
//
//    template<typename Arg>
//    template<typename Other>
//    Shared_ptr<Arg>::operator Shared_ptr<Other>() {
//        return STD::static_pointer_cast<Other, Arg>(*this);
//    }
//
//    template<typename Arg>
//    Shared_ptr<Arg>::Shared_ptr(Arg *target, Size *count, void (*del) (Arg*)) : target(target), count(count), deleter(del) {}
//
//    template<typename Arg>
//    Shared_ptr<Arg>::Shared_ptr(const Arg &val, void (*del) (Arg*)) : target(Allocate(val)), count(Allocate(Size(1))), deleter(del) {}
//
//    template<typename Arg>
//    Shared_ptr<Arg>::Shared_ptr(Arg &&val, void (*del) (Arg*)) : target(Allocate(move(val))), count(Allocate(Size(1))), deleter(del) {}
//
//    template<typename Arg>
//    Shared_ptr<Arg>::Shared_ptr(const Shared_ptr<Arg> &other) : count(other.count), target(other.target), deleter(other.deleter) {
//        ++(*count);
//    }
//
//    template<typename Arg>
//    Shared_ptr<Arg>::~Shared_ptr() {
//        --(*count);
//        if (!*count) {
//            deleter(target);
//            Deallocate(count);
//        }
//    }
//
//    template<typename Arg>
//    Shared_ptr<Arg> Shared_ptr<Arg>::reset(const Shared_ptr<Arg> &other) {
//        Shared_ptr<Arg> temp(*this);
//        --(*count);
//        count = other.count;
//        target = other.target;
//        deleter = other.deleter;
//        ++(*count);
//        return temp;
//    }
//
//    template<typename Arg>
//    Shared_ptr<Arg> &Shared_ptr<Arg>::operator=(const Shared_ptr<Arg> &other) {
//        if (count == other.count) return *this;
//        --(*count);
//        if (!*count) {
//            deleter(target);
//            Deallocate(count);
//        }
//        count = other.count;
//        target = other.target;
//        deleter = other.deleter;
//        ++(*count);
//        return *this;
//    }
//
//    template<typename Arg>
//    void Shared_ptr<Arg>::swap(Shared_ptr<Arg> &other) {
//        if (count == other.count) return;
//        STD::swap(target, other.target);
//        STD::swap(count, other.count);
//        STD::swap(deleter, other.deleter);
//    }
//
////    template<typename Arg, typename ...args>
////    Shared_ptr<Arg> make_shared(args &&...vals) {
////        return Shared_ptr<Arg>(Arg(forward(vals)...));
////    }
//}
//
//#endif //TINYSTL_SMART_PTR_HPP
