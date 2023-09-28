//
// Created by 86152 on 2023/9/25.
//

#ifndef TINYSTL_ITER_HPP
#define TINYSTL_ITER_HPP
//
// Created by 86152 on 2023/8/19.
//

#include "../Config.hpp"

namespace STD {

    //方法标签，不同的方法标签保证迭代器至少拥有的方法不同。
    struct Input_iterator_tag {
    };

    struct Output_iterator_tag {
    };

    struct Forward_iterator_tag : public Input_iterator_tag {
    };

    struct Bidirectional_iterator_tag : public Forward_iterator_tag {
    };

    struct Random_iterator_tag : public Bidirectional_iterator_tag {
    };

//----------------------------------------------------------------------------------------------------------------------

    // 这五个类只是一个标签的集合，它告诉模板函数传入的迭代器至少拥有哪些方法和可使用的类型，
    // 让作者可以利用其中定义的类型和保证拥有的方法进行泛型编程。
    template<typename T, typename Distance = Difference>
    struct Input_iterator {
        using Iterator_category = Input_iterator_tag;
        using Value_type = T;
        using Difference_type = Distance;
        using Pointer = T *;
        using Reference = T &;
    };

    struct Output_iterator {
        using Iterator_category = Output_iterator_tag;
        using Value_type = void;
        using Difference_type = void;
        using Pointer = void;
        using Reference = void;
    };

    template<typename T, typename Distance = Difference>
    struct Forward_iterator {
        using Iterator_category = Forward_iterator_tag;
        using Value_type = T;
        using Difference_type = Distance;
        using Pointer = T *;
        using Reference = T &;
    };

    template<typename T, typename Distance = Difference>
    struct Bidirectional_iterator {
        using Iterator_category = Bidirectional_iterator_tag;
        using Value_type = T;
        using Difference_type = Distance;
        using Pointer = T *;
        using Reference = T &;
    };

    template<typename T, typename Distance = Difference>
    struct Random_iterator {
        using Iterator_category = Random_iterator_tag;
        using Value_type = T;
        using Difference_type = Distance;
        using Pointer = T *;
        using Reference = T &;
    };

//----------------------------------------------------------------------------------------------------------------------

    // 继承该类可以保证规范，免去了代码遗漏定义所需的各种类型的风险。
    template<typename Category, typename T, typename Distance = Ptrdiff,
            typename Ptr = T *, typename Ref = T &>
    struct Iterator {
        using Iterator_category = Category;
        using Value_type = T;
        using Difference_type = Distance;
        using Pointer = Ptr;
        using Reference = Ref;
    };

//----------------------------------------------------------------------------------------------------------------------

    // 萃取器，作用是让泛型函数得到传入的迭代器的标签集合、迭代器所包含的元素的各种类型。
    template<typename Iterator>
    struct Iterator_traits {
        using Iterator_category = typename Iterator::Iterator_category;
        using Value_type = typename Iterator::Value_type;
        using Difference_type = typename Iterator::Difference_type;
        using Pointer = typename Iterator::Pointer;
        using Reference = typename Iterator::Reference;
    };

    // 针对指针的萃取器偏特化
    template<typename T>
    struct Iterator_traits<T *> {
        using Iterator_category = Random_iterator_tag;
        using Value_type = T;
        using Difference_type = Ptrdiff;
        using Pointer = T *;
        using Reference = T &;
    };

    // 针对const指针的萃取器偏特化
    template<typename T>
    struct Iterator_traits<const T *> {
        using Iterator_category = Random_iterator_tag;
        using Value_type = T;
        using Difference_type = Ptrdiff;
        using Pointer = const T *;
        using Reference = const T &;
    };

//----------------------------------------------------------------------------------------------------------------------

    // 这个函数模板的作用是为了能够在编译期间确定给定迭代器的迭代器类别。
    template<class Iterator>
    inline typename Iterator_traits<Iterator>::Iterator_category
    Iterator_category(const Iterator &) {
        using category = typename Iterator_traits<Iterator>::Iterator_category;
        return category(); //返回一个Iterator_category的匿名对象
    }

    template<class Iterator>
    inline typename Iterator_traits<Iterator>::Value_type *
    Value_type(const Iterator &) {
        return static_cast<typename Iterator_traits<Iterator>::Value_type *>(nullptr); //返回一个的Value_type空指针
    }

    template<class Iterator>
    inline typename Iterator_traits<Iterator>::Difference_type *
    Difference_type(const Iterator &) {
        return static_cast<typename Iterator_traits<Iterator>::Difference_type *>(nullptr); //返回一个的Value_type的const空指针
    }

//----------------------------------------------------------------------------------------------------------------------


}// namespace Detail

#endif //TINYSTL_ITER_HPP
