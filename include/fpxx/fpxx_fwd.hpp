////////////////////////////////////////////////////////////////////////////////////////////////////
// fpxx_fwd.hpp
// Forward declarations for all of fp++'s public data types.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FPXX_FWD_HPP_INCLUDED
#define FPXX_FWD_HPP_INCLUDED

#include <utility>

#define RETURN(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }
#define ENABLE_IF(...) typename std::enable_if<(__VA_ARGS__)>::type * = nullptr

namespace boost { namespace mpl {} }

namespace fp
{
    namespace detail
    {
        struct no_instance_found {};
    }

    namespace mpl = boost::mpl;

    struct make_pair;

    template<typename T>
    struct always_;

    struct always_type;

    template<typename Fun>
    struct uncurry_;

    struct uncurry_type;

    template<typename F0, typename F1>
    struct compose_;

    struct compose_type;

    struct void_;

    struct empty;

    template<typename T>
    struct static_const;

    template<typename T>
    struct functor_type;

    template<typename T>
    struct is_functor_instance;

    template<typename Functor>
    struct functor;

    struct fmap_type;

    template<typename Fun, typename Functor>
    struct variant_functor_visitor;

    template<typename Functor>
    struct variant_functor;

    template<typename Functor>
    struct monad_functor;

    template<typename T>
    struct is_monad_instance;

    template<typename T>
    struct monad_type;

    template<typename Monad>
    struct monad;

    struct bind_type;

    template<typename M>
    struct return_type;

    struct state_monad;

    template<typename RunState>
    struct state_;

    struct state_type;

    struct run_state;

    struct state_monad;

    struct get_;

    template<typename T>
    struct put_;

    struct put_type;

    struct eval_state_type;

    struct exec_state_type;

    template<template<typename> class F>
    struct fix;

    struct fx_type;

    struct unfix_type;

    template<typename Alg>
    struct cata_;

    struct cata_type;
}

#endif
