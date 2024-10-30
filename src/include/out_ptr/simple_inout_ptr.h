#pragma once

#include "base_inout_ptr_impl.h"
#include "marker.h"
#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>

namespace std {
    namespace out_ptr {
        namespace op_detail {

            template< typename Smart, typename Pointer, typename... Args >
            class simple_inout_ptr_t
                    : public base_inout_ptr_impl< Smart,
                                                  Pointer,
                                                  std::tuple< Args... >,
                                                  std::out_ptr::op_detail::make_index_sequence<
                                                      std::tuple_size< std::tuple< Args... > >::value > >
            {
                using list_t =
                    std::out_ptr::op_detail::make_index_sequence< std::tuple_size< std::tuple< Args... > >::value >;
                using core_t = base_inout_ptr_impl< Smart, Pointer, std::tuple< Args... >, list_t >;

            public:
                simple_inout_ptr_t( Smart& s, Args... args ) noexcept
                        : core_t( s, std::forward_as_tuple( std::forward< Args >( args )... ) )
                {
                }
            };

            template< typename Pointer, typename Smart, typename... Args >
            simple_inout_ptr_t< Smart, Pointer, Args... >
                simple_inout_ptr_tagged( std::false_type, Smart& s, Args&&... args ) noexcept(
                    ::std::is_nothrow_constructible< simple_inout_ptr_t< Smart, pointer_of_t< Smart >, Args... >,
                                                     Smart&,
                                                     Args... >::value )
            {
                using P = simple_inout_ptr_t< Smart, Pointer, Args... >;
                return P( s, std::forward< Args >( args )... );
            }

            template< typename, typename Smart, typename... Args >
            simple_inout_ptr_t< Smart, pointer_of_t< Smart >, Args... >
                simple_inout_ptr_tagged( std::true_type, Smart& s, Args&&... args ) noexcept(
                    ::std::is_nothrow_constructible< simple_inout_ptr_t< Smart, pointer_of_t< Smart >, Args... >,
                                                     Smart&,
                                                     Args... >::value )
            {
                using Pointer = pointer_of_t< Smart >;
                using P = simple_inout_ptr_t< Smart, Pointer, Args... >;
                return P( s, std::forward< Args >( args )... );
            }

            template< typename Pointer = marker, typename Smart, typename... Args >
            auto simple_inout_ptr( Smart& s, Args&&... args ) noexcept( noexcept( simple_inout_ptr_tagged< Pointer >(
                std::is_same< Pointer, op_detail::marker >(), s, std::forward< Args >( args )... ) ) )
                -> decltype( simple_inout_ptr_tagged< Pointer >( std::is_same< Pointer, op_detail::marker >(),
                                                                 s,
                                                                 std::forward< Args >( args )... ) )
            {
                return simple_inout_ptr_tagged< Pointer >(
                    std::is_same< Pointer, op_detail::marker >(), s, std::forward< Args >( args )... );
            }

        }
    }
}
