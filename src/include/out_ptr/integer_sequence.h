#pragma once

#include <utility>

namespace std {
    namespace out_ptr {
        namespace op_detail {

            template< typename T, T... Values >
            using integer_sequence = std::integer_sequence< T, Values... >;

            template< std::size_t... Values >
            using index_sequence = std::index_sequence< Values... >;

            template< typename T, T N >
            using make_integer_sequence = std::make_integer_sequence< T, N >;

            template< std::size_t N >
            using make_index_sequence = std::make_index_sequence< N >;

        }
    }
}
