////////////////////////////////////////////////////////////////////////////////////////////////////
// functor.hpp
// The functor typeclass and utilities.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FPXX_FUNCTOR_HPP_INCLUDED
#define FPXX_FUNCTOR_HPP_INCLUDED

#include <boost/mpl/if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/recursive_variant.hpp>
#include "fpxx/fpxx_fwd.hpp"
#include "fpxx/monad.hpp"

namespace fp
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(functor_type)

        template<typename T>
        struct functor_type
        {
            using type = typename T::functor_type;
        };
    }

    template<typename T>
    struct functor_type
      : mpl::if_<detail::has_functor_type<T>, detail::functor_type<T>, empty>::type
    {};

    template<typename T>
    struct is_functor_instance
      : mpl::bool_<std::is_base_of<empty, functor_type<T>>::value>
    {};

    struct functor_instance
    {};

    template<typename Functor>
    struct functor
    {
        // fmap :: Functor F => (a -> b) -> F a -> F b
    };

    struct fmap
    {
        // fmap :: Functor F => (a -> b) -> F a -> F b
        template<typename F, typename F_a>
        auto operator ()(F const &f, F_a const &f_a) const
            RETURN(functor_type<F_a>::type::fmap(f, f_a))
    };

    template<typename Fun, typename Functor>
    struct variant_functor_visitor;

    template<typename Functor>
    struct variant_functor : functor<Functor>
    {
        // fmap :: Functor f => (a -> b) -> f a -> f b
        // (Assumes F_a is a boost::variant.)
        template<typename Fun, typename F_a>
        static typename variant_functor_visitor<Fun, Functor>::result_type
        fmap(Fun const &f, F_a const &f_a)
        {
            return boost::apply_visitor(variant_functor_visitor<Fun, Functor>{f}, f_a);
        }
    };

    // Define a functor in terms of the monad operations.
    template<typename Functor>
    struct monad_functor : functor<Functor>
    {
        // fmap f xs  ==  xs >>= return . f
        template<typename Fun, typename M_a, typename M = typename monad_type<M_a>::type>
        static auto fmap(Fun const &f, M_a const &m_a)
            RETURN(m_a >>= compose(return_<M>(), f))
    };
}

#endif
