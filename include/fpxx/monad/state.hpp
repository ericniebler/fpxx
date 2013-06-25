////////////////////////////////////////////////////////////////////////////////////////////////////
// state.hpp
// The state monad.
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef FPXX_MONAD_STATE_HPP_INCLUDED
#define FPXX_MONAD_STATE_HPP_INCLUDED

#include <utility>
#include <functional>
#include "fpxx/fpxx_fwd.hpp"
#include "fpxx/prologue.hpp"
#include "fpxx/monad.hpp"
#include "fpxx/functor.hpp"

namespace fp
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    struct state_monad;

    struct state_functor;

    template<typename RunState>
    struct state_ : monad_instance, functor_instance
    {
        using monad_type = state_monad;
        using functor_type = state_functor;

        RunState run_state;

        explicit constexpr state_(RunState const &fun = RunState())
          : run_state(fun)
        {}
    };

    template<typename RunState>
    constexpr state_<RunState> state(RunState const &fun)
    {
        return state_<RunState>{fun};
    }

    struct run_state
    {
        template<typename RunState, typename St>
        auto operator ()(state_<RunState> const &st, St const &init) const
            RETURN(st.run_state(init))
    };

    struct state_monad : monad<state_monad>
    {
        // monad M => M a -> (a -> M b) -> M b
        template<typename RunState, typename Fun>
        static auto bind(state_<RunState> const &st, Fun const &fun)
          RETURN(
                fp::state(
                    std::bind(
                        fp::uncurry(std::bind(run_state(), std::bind(fun, _1), _2))
                      , std::bind(run_state(), st, _1)
                    )
                )
            )

        template<typename Val>
        static auto return_(Val const &val)
          RETURN(fp::state(std::bind(fp::make_pair(), val, _1)))
    };

    struct state_functor : monad_functor<state_functor>
    {};

    struct get_
    {
        template<typename T>
        std::pair<T, T> operator ()(T const &st) const
        {
            return std::make_pair(st, st);
        }
    };

    namespace
    {
        // The following is an ODR-safe equivalant to:
        // constexpr auto get = fp::state(get_());
        constexpr auto const &get = static_const<state_<get_>>::value;
    }

    template<typename T>
    struct put_
    {
        T st;

        constexpr explicit put_(T const &t) : st(t) {}

        template<typename U>
        std::pair<void_, T> operator ()(U const &) const
        {
            return std::make_pair(void_(), st);
        }
    };

    template<typename T>
    constexpr state_<put_<T>> put(T const &t)
    {
        return fp::state(put_<T>(t));
    }

    struct eval_state
    {
        template<typename GetState, typename St>
        auto operator ()(state_<GetState> const &st, St const &init) const
            RETURN(std::get<0>(run_state()(st, init)))
    };

    struct exec_state
    {
        template<typename GetState, typename St>
        auto operator ()(state_<GetState> const &st, St const &init) const
            RETURN(std::get<1>(run_state()(st, init)))
    };
}

#endif
