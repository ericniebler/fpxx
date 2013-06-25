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
    namespace mpl = boost::mpl;

    struct make_pair;

    template<typename T>
    struct always_;

    template<typename T>
    constexpr always_<T> always(T const &val);

    template<typename Fun>
    struct uncurry_;

    template<typename Fun>
    constexpr uncurry_<Fun> uncurry(Fun const &f);

    template<typename F0, typename F1>
    struct compose_;

    template<typename F0, typename F1>
    constexpr compose_<F0, F1> compose(F0 const &f0, F1 const &f1);

    struct void_;

    struct empty;

    template<typename T>
    struct static_const;

    template<typename T>
    struct functor_type;

    template<typename T>
    struct is_functor_instance;

    struct functor_instance;

    template<typename Functor>
    struct functor;

    struct fmap;

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

    struct monad_instance;

    template<typename Monad>
    struct monad;

    struct bind;

    template<typename M>
    struct return_;

    struct state_monad;

    template<typename RunState>
    struct state_;

    template<typename RunState>
    constexpr state_<RunState> state(RunState const &fun);

    struct run_state;

    struct state_monad;

    struct get_;

    template<typename T>
    struct put_;

    template<typename T>
    constexpr state_<put_<T>> put(T const &t);

    struct eval_state;

    struct exec_state;

    template<template<typename> class F>
    struct fix;

    template<template<typename> class F>
    constexpr fix<F> fx(F<fix<F>> f);

    template<template<typename> class F,
             template<typename, typename...> class Fprime,
             typename ...A>
    constexpr fix<F> fx(Fprime<fix<F>, A...> f);

    template<template<typename> class F>
    F<fix<F>> const &unfix(fix<F> const &f);

    template<typename Alg>
    struct cata_;

    template<typename Alg>
    constexpr cata_<Alg> cata(Alg const &alg);
}

#endif
