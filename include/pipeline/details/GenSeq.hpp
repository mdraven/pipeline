/**
   \file

   Seq -- это тип для хранения последовательности чисел.

   Для того чтобы получить последовательность до определённого
   числа используют вспомогательный класс GenSeq.
*/

#pragma once

namespace pipeline {

    namespace details {

        template <int...>
        struct Seq {};

        template <int N, int... S>
        struct GenSeq : GenSeq<N-1, N-1, S...> {};

        template <int... S>
        struct GenSeq<0, S...> {
            using type = Seq<S...>;
        };

        template <int... S>
        using GenSeq_t = typename GenSeq<S...>::type;

    } /* namespace details */

} /* namespace pipeline */
