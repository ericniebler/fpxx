////////////////////////////////////////////////////////////////////////////////////////////////////
// monad.hpp
// Monad stuff.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FPXX_MONAD_HPP_INCLUDED
#define FPXX_MONAD_HPP_INCLUDED

#include <type_traits>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_xxx.hpp>
#include "fpxx/fpxx_fwd.hpp"
#include "fpxx/prologue.hpp"

namespace fp
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(monad_type)

        template<typename T>
        struct monad_type
        {
            using type = typename T::monad_type;
        };
    }

    template<typename T>
    struct monad_type
      : mpl::if_<detail::has_monad_type<T>, detail::monad_type<T>, empty>::type
    {};

    template<typename T>
    struct is_monad_instance
      : mpl::bool_<std::is_base_of<empty, monad_type<T>>::value>
    {};

    struct monad_instance
    {};

    template<typename Monad>
    struct monad
    {
        // (>>=) :: Monad M => M a -> (a -> M b) -> M b

        // return :: Monad M => a -> M a

        // (>>) :: Monad M => M a -> M b -> M b
        // M a >> M b = M a >>= \_ -> M b
        template<typename M_a, typename M_b, typename M = Monad>
        static auto seq(M_a const &m_a, M_b const &m_b)
            RETURN(M::bind(m_a, always(m_b)))
    };

    // (>>=) :: Monad M => M a -> (a -> M b) -> M b
    template<typename M, typename F>
    auto operator >>=(M const &m, F const &f)
        RETURN(monad_type<M>::type::bind(m, f))

    // (>>) :: Monad M => M a -> M b -> M b
    template<typename M_a, typename M_b
      , ENABLE_IF(std::is_same<
            typename monad_type<M_a>::type
          , typename monad_type<M_b>::type
        >::value)
    >
    auto operator >>(M_a const &m_a, M_b const &m_b)
        RETURN(monad_type<M_a>::type::seq(m_a, m_b))

    struct bind
    {
        template<typename M, typename F>
        auto operator ()(M const &m, F const &f) const
            RETURN(monad_type<M>::type::bind(m, f))
    };

    template<typename M>
    struct return_
    {
        // return :: Monad M => a -> M a
        template<typename Val>
        auto operator ()(Val const &val) const
            RETURN(M::return_(val))
    };
}

#endif
