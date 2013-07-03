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
    struct state_monad;

    struct state_functor;

    template<typename RunState>
    struct state_
    {
        RunState run_state;

        explicit constexpr state_(RunState const &fun = RunState())
          : run_state(fun)
        {}
    };

    struct state_type
    {
        template<typename RunState>
        constexpr state_<RunState> operator ()(RunState const &fun) const
        {
            return state_<RunState>{fun};
        }
    };

    namespace
    {
        constexpr state_type const &state = static_const<state_type>::value;
    }

    struct run_state
    {
        template<typename RunState, typename St>
        auto operator ()(state_<RunState> const &st, St const &init) const
            RETURN(st.run_state(init))
    };

    struct state_monad : monad<state_monad>
    {
        struct bind_type
        {
            // monad M => M a -> (a -> M b) -> M b
            template<typename RunState, typename Fun>
            auto operator ()(state_<RunState> const &st, Fun const &fun) const
                RETURN(
                    fp::state(
                        std::bind(
                            fp::uncurry(std::bind(run_state(), std::bind(fun, _1), _2))
                          , std::bind(run_state(), st, _1)
                        )
                    )
                )
        };

        static constexpr bind_type const &bind = static_const<bind_type>::value;

        struct return_type
        {
            template<typename Val>
            auto operator ()(Val const &val) const
                RETURN(fp::state(std::bind(fp::make_pair(), val, _1)))
        };

        static constexpr return_type const &return_ = static_const<return_type>::value;
    };

    constexpr state_monad::bind_type const &state_monad::bind;
    constexpr state_monad::return_type const &state_monad::return_;

    struct state_functor : monad_functor<state_functor>
    {};

    template<typename RunState>
    struct monad_type<state_<RunState>>
    {
        using type = state_monad;
    };

    template<typename RunState>
    struct functor_type<state_<RunState>>
    {
        using type = state_functor;
    };

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

    struct put_type
    {
        template<typename T>
        constexpr state_<put_<T>> operator ()(T const &t) const
        {
            return fp::state(put_<T>(t));
        }
    };

    namespace
    {
        constexpr put_type const &put = static_const<put_type>::value;
    }

    struct eval_state_type
    {
        template<typename GetState, typename St>
        auto operator ()(state_<GetState> const &st, St const &init) const
            RETURN(std::get<0>(run_state()(st, init)))
    };

    struct exec_state_type
    {
        template<typename GetState, typename St>
        auto operator ()(state_<GetState> const &st, St const &init) const
            RETURN(std::get<1>(run_state()(st, init)))
    };

    namespace
    {
        constexpr eval_state_type const &eval_state = static_const<eval_state_type>::value;
        constexpr exec_state_type const &exec_state = static_const<exec_state_type>::value;
    }
}

#endif
