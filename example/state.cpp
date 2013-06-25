////////////////////////////////////////////////////////////////////////////////////////////////////
// state.hpp
//
//  Copyright 2013 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include "fpxx/monad/state.hpp"
#include <iostream>

int main()
{
    using namespace fp;
    using S = fp::state_monad;

    const auto inc =
        fp::state([](int st){return std::make_pair(void_(), st+1);});
    auto inc3sq =
        inc >>
        inc >>
        inc >>
        get >>= [](int s){return
        S::return_(s*s);
    };

    std::cout << eval_state()(inc3sq, 0) << std::endl;

    auto postincrement =
        get      >>= [](int x){return
        put(x+1) >>
        S::return_(x);
    };

    auto p = run_state()(postincrement, 1);
    std::cout << "value = " << p.first << std::endl;
    std::cout << "state = " << p.second << std::endl;

    // fp::state is also a functor:
    auto x = S::return_(42);
    auto fun = [](int x){return "hello";};
    auto y = fp::fmap()(fun, x);

    std::cout << fp::eval_state()(y, 36) << std::endl;

    void done();
    done();
}

void done()
{
    char ch;
    std::cout << "Hit Ctrl+D to exit" << std::endl;
    std::cin >> ch;
}
