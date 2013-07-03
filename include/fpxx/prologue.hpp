////////////////////////////////////////////////////////////////////////////////////////////////////
// prologue.hpp
// Generally useful stuff fp++'s public data types.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FPXX_PROLOGUE_HPP_INCLUDED
#define FPXX_PROLOGUE_HPP_INCLUDED

#include <iosfwd>
#include <utility>
#include <functional>
#include "fpxx/fpxx_fwd.hpp"

namespace fp
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    struct empty
    {};

    template<template<typename...> class T, typename ...B>
    struct bind_rest
    {
        template<typename A>
        using other = T<A, B...>;
    };

    template<typename T>
    struct static_const
    {
        static constexpr T value {};
    };

    template<typename T>
    constexpr T static_const<T>::value;

    struct make_pair
    {
        template<typename First, typename Second>
        auto operator ()(First const &first, Second const &second) const
            RETURN(std::make_pair(first, second))
    };

    template<typename T>
    struct always_
    {
        T value;

        explicit constexpr always_(T const &val)
          : value(val)
        {}

        template<typename U>
        T operator ()(U const &) const
        {
            return value;
        }
    };

    struct always_type
    {
        template<typename T>
        constexpr always_<T> operator ()(T const &val) const
        {
            return always_<T>(val);
        }
    };

    namespace
    {
        constexpr always_type const &always = static_const<always_type>::value;
    }

    template<typename Fun>
    struct uncurry_
    {
        Fun fun;

        constexpr explicit uncurry_(Fun const &f)
          : fun(f)
        {}

        template<typename Pair>
        auto operator ()(Pair const &p) const
            RETURN(fun(std::get<0>(p), std::get<1>(p)))
    };

    struct uncurry_type
    {
        template<typename Fun>
        constexpr uncurry_<Fun> operator ()(Fun const &f) const
        {
            return uncurry_<Fun>{f};
        }
    };

    namespace
    {
        constexpr uncurry_type const &uncurry = static_const<uncurry_type>::value;
    }

    template<typename F0, typename F1>
    struct compose_
    {
        F0 fun0;
        F1 fun1;

        constexpr explicit compose_(F0 const &f0, F1 const &f1)
          : fun0(f0), fun1(f1)
        {}

        template<typename T>
        auto operator ()(T const &t) const
            RETURN(fun0(fun1(t)))
    };

    struct compose_type
    {
        template<typename F0, typename F1>
        constexpr compose_<F0, F1> operator ()(F0 const &f0, F1 const &f1) const
        {
            return compose_<F0, F1>(f0, f1);
        }
    };

    namespace
    {
        constexpr compose_type const &compose = static_const<compose_type>::value;
    }

    struct void_ 
    {
        template<typename Ostream = std::ostream>
        friend Ostream &operator<<(Ostream &sout, void_)
        {
            return sout << "()";
        }
    };
}

#endif
