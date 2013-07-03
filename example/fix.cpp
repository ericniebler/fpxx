////////////////////////////////////////////////////////////////////////////////////////////////////
// fix.hpp
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <functional>
#include <type_traits>
#include "fpxx/fix.hpp"
#include <iostream>

template<typename A>
struct expr_functor;

// expr_f a = const_ Int
//         | add a a
//         | mul a a
struct _const_
{
    int value;
};

template<typename A>
struct _add_
{
    A left;
    A right;
};

template<typename A>
struct _mul_
{
    A left;
    A right;
};

template<typename A>
using expr_f_ =
    boost::variant<
        _const_
      , boost::recursive_wrapper<_add_<A> >
      , boost::recursive_wrapper<_mul_<A> >
    >;

template<typename A>
struct expr_f
  : expr_f_<A>
{
    using functor_type = expr_functor<A>;
    expr_f(_const_ c) : expr_f_<A>(c) {}
    expr_f(_add_<A> c) : expr_f_<A>(c) {}
    expr_f(_mul_<A> c) : expr_f_<A>(c) {}
};

// type expr = fix expr_f
using expr = fp::fix<expr_f>;

template<typename A = expr>
expr_f<A> const_(int val) {return _const_{val};}

template<typename A>
expr_f<A> add(A a, A b) {return _add_<A>{a, b};}

template<typename A>
expr_f<A> mul(A a, A b) {return _mul_<A>{a, b};}

// instance Functor expr_f where ...
template<typename A>
struct expr_functor
  : fp::variant_functor<expr_functor<A>>
{};

namespace fp
{
    template<typename Fun, typename A>
    struct variant_functor_visitor<Fun, expr_functor<A>>
      : boost::static_visitor<expr_f<typename std::result_of<Fun(A)>::type>>
    {
        typedef typename std::result_of<Fun(A)>::type B;

        explicit variant_functor_visitor(Fun f)
          : f_(f)
        {}

        expr_f<B> operator()(_const_ i) const
        {
            return const_<B>(i.value);
        }

        expr_f<B> operator()(_add_<A> e) const
        {
            return add(f_(e.left), f_(e.right));
        }

        expr_f<B> operator()(_mul_<A> e) const
        {
            return mul(f_(e.left), f_(e.right));
        }
    private:
        Fun f_;
    };
}

// alg :: expr_f Int -> Int
//
// alg (const_ i)   = i
// alg (x `add` y) = x + y
// alg (x `mul` y) = x * y
struct alg_visitor
  : boost::static_visitor<int>
{
    int operator()(_const_ i) const
    {
        return i.value;
    }

    int operator()(_add_<int> e) const
    {
        return e.left + e.right;
    }

    int operator()(_mul_<int> e) const
    {
        return e.left * e.right;
    }
};

struct alg : std::unary_function<expr_f<int>, int>
{
    int operator()(expr_f<int> e) const
    {
        return boost::apply_visitor(alg_visitor{}, e);
    }
};

// Evaluate an expression using "alg"
// eval :: fix expr_f -> Int
// eval = cata alg
int eval(expr e)
{
    return fp::cata(alg{}, e);
}

int main()
{
    // (2+3)*4 == 20
    expr testExpr =
        fp::fx(mul(
            fp::fx(add(
                fp::fx(const_(2)),
                fp::fx(const_(3))
            )),
            fp::fx(const_(4))
        ));
    int z = eval(testExpr);
    std::cout << z << std::endl;

    void done();
    done();
}

void done()
{
    char ch;
    std::cout << "Hit Ctrl+D to exit" << std::endl;
    std::cin >> ch;
}
