
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <cassert>
#include <iostream>

struct Decorate {
    std::string m_str;

    explicit Decorate(const std::string& str)
        : m_str(str) {}

    template <class Type>
    void print(const Type& data) const {
        std::cout << m_str << data << std::endl;
    }
};

int main() {
    using namespace pipeline;

    Decorate decor(">> ");

    auto print1 = pipe_op_factory(&Decorate::print<std::string>);

    decor | print1(std::string("Hello"));
    decor | &Decorate::print<std::string> A(std::string("Hello"));

    return 0;
}
