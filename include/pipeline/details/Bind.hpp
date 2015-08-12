/**
   \file

   Bind это упрощённая версия std::bind. Он
   умеет создавать привязки вида std::bind(func, _1, args...),
   т.е. изменяемым является только первый аргумент, остальные
   являются привязанными.

   Bind не аллоцирует сам память и старается использовать семантику
   перемещения. Аллокация может произойти при копировании аргументов
   и объектов для вызова.

   Не имеет смысла создавать Bind напрямую. Для создания нужно
   использовать функцию bind.
*/

#pragma once

#include <pipeline/details/GenSeq.hpp>
#include <pipeline/details/JustReturn.hpp>
#include <pipeline/details/UnpackTuple.hpp>

#include <tuple>

namespace pipeline {

    namespace details {

        /**
           Преобразует полученный функциональный
           объект: первый аргумент остаётся изменяемым,
           к остальным аргументам привязываются значения

           \tparam Func функциональный объект который будет
           вызываться
           \tparam Args типы значений которые будут привязаны
           к аргументам Func
        */
        template <class Func,
                  class... Args>
        class Bind final {
            Func m_func;
            std::tuple<Args...> m_args;
        public:
            template <class TFunc,
                      class... TArgs>
            explicit Bind(TFunc&& func,
                          TArgs&&... args)
                : m_func(std::forward<TFunc>(func)),
                  m_args(std::forward<TArgs>(args)...) {}

            /**
               Вызвать функции с передачей в неё
               единственного непривязанного аргумента
            */
            template <class Arg>
            auto operator()(Arg&& arg)
                JUST_RETURN(
                    UnpackTuple::call(m_func,
                                      m_args,
                                      std::forward<Arg>(arg))
                    );
        };

        /**
           Функция для создания Bind
        */
        template <class Func,
                  class... Args>
        auto bind(Func&& func, Args&&... args) {
            return Bind<std::remove_reference_t<Func>,
                        std::remove_reference_t<Args>...>
                (std::forward<Func>(func),
                 std::forward<Args>(args)...);
        }

    } /* namespace details */

} /* namespace pipeline */
