#pragma once

#include "out_ptr_traits.h"
#include "pointer_of.h"
#include <type_traits>

namespace std {
    namespace out_ptr {

        namespace op_detail {
            template< typename Smart >
            void call_release( std::true_type, Smart& s ) noexcept( noexcept( std::declval< Smart& >().release() ) )
            {
                s.release();
            }

            template< typename Smart >
            void call_release( std::false_type, Smart& ) noexcept
            {
                static_assert( std::is_pointer< Smart >::value,
                               "the type that does not have release called on it must be a pointer type" );
            }
        }

        template< typename Smart, typename Pointer >
        class inout_ptr_traits
        {
        public:
            using pointer = Pointer;

        private:
            using OUT_PTR_DETAIL_UNSPECIALIZED_MARKER_ = int;
            using defer_t = out_ptr_traits< Smart, Pointer >;

            template< typename... Args >
            static pointer construct( std::true_type, Smart& s, Args&&... ) noexcept
            {
                return static_cast< pointer >( s );
            }

            template< typename... Args >
            static pointer construct( std::false_type, Smart& s, Args&&... ) noexcept
            {
                return static_cast< pointer >( s.get() );
            }

        public:
            template< typename... Args >
            static pointer construct( Smart& s, Args&&... args ) noexcept
            {
                return construct( std::is_pointer< Smart >(), s, std::forward< Args >( args )... );
            }

            static typename std::add_pointer< pointer >::type get( Smart&, pointer& p ) noexcept
            {
                return std::addressof( p );
            }

            template< typename... Args >
            static void reset( Smart& s, pointer& p, Args&&... args ) noexcept
            {
                op_detail::call_release( op_detail::is_releasable< Smart >(), s );
                defer_t::reset( s, p, std::forward< Args >( args )... );
            }
        };

    }
}
