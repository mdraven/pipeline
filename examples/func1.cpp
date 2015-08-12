
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <cassert>
#include <iostream>

int func_arg1(int n) {
    return n + 1;
}

int func_arg2(int n, int k) {
    return n + k;
}

template <int k>
int templ_func(int n) {
    return k + n;
}

void print(int n) {
    std::cout << n << std::endl;
}

int main() {
    using namespace pipeline;

    auto arg1_1 = pipe_op(func_arg1);
    auto arg1_2 = pipe_op_factory(func_arg1);

    print(arg1_1(1));
    print(arg1_2()(1));

    1 | arg1_1 | print A();
    1 | arg1_2() | print A();

    auto arg2 = pipe_op_factory(func_arg2);

    1 | arg2(1) | print A();
    1 | func_arg2 A(1) | print A();

    auto tmpl_f = pipe_op(templ_func<1>);

    1 | tmpl_f | print A();
    1 | templ_func<1> A() | print A();

    return 0;
}
