/**
   \file

   Ref -- класс имитирующий ссылку. Он похож на std::reference_wrapper,
   но в отличии от него поддерживает присваивание значения:
   \code
   int val;
   std::ref(val) = 10; // ошибка
   pipeline::ref(val) = 10; // OK
   \endcode

   Этот класс не имеет непосредственного отношения к pipeline, и
   включён в библиотеку только потому что нужен при использовании
   функций возвращающих по ссылке.
*/

#pragma once

namespace pipeline {

    namespace details {

        /**
           Этот класс имитирует ссылку
        */
        template <class T>
        class Ref {
            T* m_t;
        public:
            Ref() = delete;
            Ref(const Ref&) = default;

            explicit Ref(T& t)
                : m_t(&t) {}

            template <class TT>
            Ref& operator=(TT&& t) {
                *m_t = std::forward<TT>(t);
                return *this;
            }

            Ref& operator=(const Ref&) = delete;

            template <class... Args>
            auto operator()(Args&&... args) {
                return (*m_t)(std::forward(args)...);
            }

            T& get() {
                return *m_t;
            }

            const T& get() const {
                return *m_t;
            }

            operator T&() {
                return *m_t;
            }

            operator const T&() const {
                return *m_t;
            }
        };

        /**
           Функция для создания Ref
        */
        template <class T>
        auto ref(T& t) {
            return Ref<T>(t);
        }

        /**
           Функция для создания const Ref
        */
        template <class T>
        auto cref(T& t) {
            return Ref<const T>(t);
        }

    } /* namespace details */

} /* namespace pipeline */
