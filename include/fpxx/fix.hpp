////////////////////////////////////////////////////////////////////////////////////////////////////
// fix.hpp
// The fix type constructor
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FPXX_FIX_HPP_INCLUDED
#define FPXX_FIX_HPP_INCLUDED

#include <type_traits>
#include "fpxx/functor.hpp"

namespace fp
{
    // newtype fix f = fx (f (fix f))
    //
    template<template<typename> class F>
    struct fix
      : F<fix<F>>
    {
        constexpr explicit fix(F<fix<F>> const &f)
          : F<fix<F>>(f)
        {}
    };

    template<template<typename> class F>
    constexpr fix<F> fx(F<fix<F>> f)
    {
        return fix<F>{f};
    }

    // This is only needed because template-template parameters have fixed
    // arities, and template aliases do not hide the arity of the underlying
    // template.
    template<template<typename> class F,
             template<typename, typename...> class Fprime,
             typename ...A>
    constexpr fix<F> fx(Fprime<fix<F>, A...> f)
    {
        static_assert(std::is_same<F<fix<F>>, Fprime<fix<F>, A...>>::value,
                      "fx requires F to be an alias for Fprime where all but "
                      "the first template arguments are bound.");
        return fix<F>{f};
    }

    // unfix :: fix f -> f (fix f)
    // unfix (fx x) = x
    template<template<typename> class F>
    F<fix<F>> const &unfix(fix<F> const &f)
    {
        return f;
    }

    // cata :: Functor f => (f a -> a) -> fix f -> a
    // cata alg = alg . fmap (cata alg) . unfix
    template<typename Alg>
    struct cata_
    {
        Alg alg_;

        constexpr explicit cata_(Alg const &alg)
          : alg_(alg)
        {}

        template<template<typename> class F>
        typename Alg::result_type operator ()(fix<F> o) const
        {
            return this->alg_(fp::fmap()(*this, fp::unfix(o)));
        }
    };

    template<typename Alg>
    constexpr cata_<Alg> cata(Alg const &alg)
    {
        return cata_<Alg>{alg};
    }
}

#endif
