
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <cassert>
#include <iostream>

struct Sum {
    int operator()(int a, int b) const {
        return a + b;
    }
};

void print(int n) {
    std::cout << n << std::endl;
}

int main() {
    using namespace pipeline;

    auto sum = pipe_op_factory(Sum());

    print(Sum()(1, 2));
    print(sum(2)(1));

    1 | sum(2) | print A();
    1 | Sum() A(2) | print A();

    return 0;
}
