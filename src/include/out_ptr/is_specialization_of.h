#pragma once

#include <type_traits>

namespace std {
    namespace out_ptr {
        namespace op_detail {

            namespace meta_detail {
                template< typename T, template< typename... > class Templ >
                struct is_specialization_of : std::false_type
                {
                };
                template< typename... T, template< typename... > class Templ >
                struct is_specialization_of< Templ< T... >, Templ > : std::true_type
                {
                };

            }

            template< typename T, template< typename... > class Templ >
            using is_specialization_of = meta_detail::is_specialization_of< typename std::remove_cv< T >::type, Templ >;

        }
    }
}
