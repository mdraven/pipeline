
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <cassert>
#include <iostream>

int main() {
    using namespace pipeline;

    auto times = pipe_op_factory([](unsigned int num, auto func) {
            for(unsigned int i = 0; i < num; ++i)
                func();
        });

    times([] { std::cout << "Hello" << std::endl; })(2U);

    3U | times([] { std::cout << "World" << std::endl; });

    return 0;
}
