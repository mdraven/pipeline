
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <cassert>
#include <iostream>

struct Data {
    int m_data = 1;

    Data() {};

    Data& inc(int num = 1) {
        m_data += num;
        return *this;
    }
};

void print(const Data& d) {
    std::cout << d.m_data << std::endl;
}

#define M(METHOD_NAME) [](auto&& klass, auto&&... args) { return klass.METHOD_NAME(args...); }

int main() {
    using namespace pipeline;

    auto inc_ = pipe_op_factory(&Data::inc);
    auto print_ = pipe_op(print);

    print(Data().inc(1).inc(1));

    Data() | inc_(1) | inc_(1) | print_;

    Data() | &Data::inc A(1) | &Data::inc A(1) | print A();

    Data() | M(inc)A(1) | M(inc)A(1) | print A();

    return 0;
}
