
#include <pipeline/args.hpp>
#include <pipeline/ref.hpp>
#include <pipeline/pipeline.hpp>

#include <boost/optional.hpp>
#include <cassert>
#include <iostream>

using namespace pipeline;
using namespace pipeline::details;

template <class T, class Callable>
auto operator|(boost::optional<T>&& t, PipeOp<Callable>& op)
    JUST_RETURN(
        t ? op(std::move(t.get())) : decltype(op(std::move(t.get())))()
        );

template <class T, class Callable>
auto operator|(boost::optional<T>&& t, const PipeOp<Callable>& op)
    JUST_RETURN(
        t ? op(std::move(t.get())) : decltype(op(std::move(t.get())))()
        );

template <class T, class Callable>
auto operator|(boost::optional<T>&& t, PipeOp<Callable>&& op)
    JUST_RETURN(
        t ? op(std::move(t.get())) : decltype(op(std::move(t.get())))()
        );

boost::optional<int> ok(int i) {
    return i + 1;
}

boost::optional<int> fail(int i) {
    return boost::optional<int>();
}

void print(int n) {
    std::cout << n << std::endl;
}

int main() {

    auto ok_ = pipe_op(ok);
    auto fail_ = pipe_op(fail);
    auto print_ = pipe_op(print);

    boost::optional<int>(1) | ok_ | ok_ | print_;
    boost::optional<int>(1) | ok_ | fail_ | print_;

    return 0;
}
