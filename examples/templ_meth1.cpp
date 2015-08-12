
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <cassert>
#include <iostream>

struct Times {
    template <class Func>
    void operator()(std::size_t num, Func func) {
        for(std::size_t i = 0; i < num; ++i)
            func();
    }
};

struct Set {
    template <class Data, class T>
    auto operator()(Data&& data, T& t) {
        t = data;
        return std::forward<Data>(data);
    }
};

int main() {
    using namespace pipeline;

    auto times = pipe_op_factory(Times());

    3U | times([] { std::cout << "Hello" << std::endl; });

    int storage;
    5 | Set() A(ref(storage));
    assert(storage == 5);

    return 0;
}
