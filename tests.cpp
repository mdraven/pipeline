
#include <pipeline/pipeline.hpp>
#include <pipeline/args.hpp>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE pipeline_tests

#include <boost/test/unit_test.hpp>
#include <type_traits>

using namespace pipeline;

struct Data {
    static int m_mut_call;
    static int m_const_call;

    static int m_def_constructor;
    static int m_copy_constructor;
    static int m_move_constructor;

    static int m_arg_value;

    static void clear() {
        m_mut_call = 0;
        m_const_call = 0;

        m_def_constructor = 0;
        m_copy_constructor = 0;
        m_move_constructor = 0;

        m_arg_value = 0;
    }

    Data() { ++m_def_constructor; }
    Data(const Data&) { ++m_copy_constructor; }
    Data(Data&&) { ++m_move_constructor; }

    const Data& constWithArg(int num = 1) const {
        ++m_const_call;
        m_arg_value = num;
        return *this;
    }

    Data& mutWithArg(int num = 1) {
        ++m_mut_call;
        m_arg_value = num;
        return *this;
    }

    void constWithoutArgs() const {
        ++m_const_call;
    }

    void mutWithoutArgs() {
        ++m_mut_call;
    }
};

int Data::m_mut_call;
int Data::m_const_call;

int Data::m_def_constructor;
int Data::m_copy_constructor;
int Data::m_move_constructor;

int Data::m_arg_value;

static int m_func_call;

void func_const(const Data&) {
    ++m_func_call;
}

void func_mut(Data&) {
    ++m_func_call;
}

template <class T>
void templ_func(T&) {
    ++m_func_call;
}

BOOST_AUTO_TEST_CASE(test_const_func) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op(func_const);
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op(func_const);
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    m_func_call = 0;
    mut_data | mut_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    mut_data | const_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    const_data | mut_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    const_data | const_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_mut_func) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op(func_mut);
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op(func_mut);
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    m_func_call = 0;
    mut_data | mut_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    mut_data | const_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    // TODO: сделать тесты для некомпилирующихся
    /// не должны компилироваться
    // const_data | mut_f
    // const_data | const_f;

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_const_meth_wo_args) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op(&Data::constWithoutArgs);
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op(&Data::constWithoutArgs);
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    Data::m_const_call = 0;
    mut_data | mut_f;
    BOOST_CHECK_EQUAL(Data::m_const_call, 1);

    Data::m_const_call = 0;
    mut_data | const_f;
    BOOST_CHECK_EQUAL(Data::m_const_call, 1);

    Data::m_const_call = 0;
    const_data | mut_f;
    BOOST_CHECK_EQUAL(Data::m_const_call, 1);

    Data::m_const_call = 0;
    const_data | const_f;
    BOOST_CHECK_EQUAL(Data::m_const_call, 1);

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_mut_meth_wo_args) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op(&Data::mutWithoutArgs);
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op(&Data::mutWithoutArgs);
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    Data::m_mut_call = 0;
    mut_data | mut_f;
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);

    Data::m_mut_call = 0;
    mut_data | const_f;
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);

    /// не должны компилироваться
    // const_data | mut_f;
    // const_data | const_f;

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_mut_meth_with_args) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op_factory(&Data::mutWithArg);
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op_factory(&Data::mutWithArg);
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    Data::m_mut_call = 0;
    Data::m_arg_value = 0;
    mut_data | mut_f(3);
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);
    BOOST_CHECK_EQUAL(Data::m_arg_value, 3);

    Data::m_mut_call = 0;
    Data::m_arg_value = 0;
    mut_data | const_f(3);
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);
    BOOST_CHECK_EQUAL(Data::m_arg_value, 3);

    /// не должны компилироваться
    // const_data | mut_f(1);
    // const_data | const_f(1);

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_lambda_with_auto_args) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op_factory([](auto& d, auto num) JUST_RETURN( d.mutWithArg(num) ));
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op_factory([](auto& d, auto num) JUST_RETURN( d.mutWithArg(num) ));
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    Data::m_mut_call = 0;
    Data::m_arg_value = 0;
    mut_data | mut_f(3);
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);
    BOOST_CHECK_EQUAL(Data::m_arg_value, 3);

    Data::m_mut_call = 0;
    Data::m_arg_value = 0;
    mut_data | const_f(3);
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);
    BOOST_CHECK_EQUAL(Data::m_arg_value, 3);

    /// не должны компилироваться
    // const_data | mut_f(1);
    // const_data | const_f(1);

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_args_with_1_arg) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    Data::m_mut_call = 0;
    Data::m_arg_value = 0;
    mut_data | &Data::mutWithArg A(3);
    BOOST_CHECK_EQUAL(Data::m_mut_call, 1);
    BOOST_CHECK_EQUAL(Data::m_arg_value, 3);

    Data::m_const_call = 0;
    Data::m_arg_value = 0;
    const_data | &Data::constWithArg A(3);
    BOOST_CHECK_EQUAL(Data::m_const_call, 1);
    BOOST_CHECK_EQUAL(Data::m_arg_value, 3);

    /// не должны компилироваться
    // const_data | &Data::mutWithArg A(3);

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}

BOOST_AUTO_TEST_CASE(test_template_func) {
    Data::clear();

    Data mut_data;
    const Data const_data;

    auto mut_f = pipe_op(templ_func<const Data>);
    BOOST_CHECK(!std::is_const<decltype(mut_f)>::value);

    const auto const_f = pipe_op(templ_func<const Data>);
    BOOST_CHECK(std::is_const<decltype(const_f)>::value);

    m_func_call = 0;
    mut_data | mut_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    mut_data | const_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    const_data | mut_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    m_func_call = 0;
    const_data | const_f;
    BOOST_CHECK_EQUAL(m_func_call, 1);

    BOOST_CHECK_EQUAL(Data::m_def_constructor, 2);
    BOOST_CHECK_EQUAL(Data::m_copy_constructor, 0);
    BOOST_CHECK_EQUAL(Data::m_move_constructor, 0);
}
