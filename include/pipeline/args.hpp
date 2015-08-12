#pragma once

#include <pipeline/details/Arguments.hpp>

/**
   Этот макрос нужен для того чтобы дать
   ему синоним и использовать вместо
   функции args: 4 | sum A(2);
*/
#define PIPELINE_ARGS_MACRO(...) << ::pipeline::details::args(__VA_ARGS__)

#ifndef PIPELINE_WITHOUT_ARGS_MACRO_FLAG
#define A PIPELINE_ARGS_MACRO
#endif /* PIPELINE_WITHOUT_ARGS_MACRO_FLAG */

namespace pipeline {

    using pipeline::details::args;

} /* namespace pipeline */
