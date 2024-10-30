#pragma once

#include "base_out_ptr_impl.h"
#include <cstdlib>
#include <memory>
#include <type_traits>
#include <utility>

namespace std {
    namespace out_ptr {
        namespace op_detail {

            template< typename Smart, typename Pointer, typename Args, typename List >
            struct base_inout_ptr_impl
                    : base_out_ptr_impl< Smart, Pointer, inout_ptr_traits< Smart, Pointer >, Args, List >
            {
            private:
                using base_t = base_out_ptr_impl< Smart, Pointer, inout_ptr_traits< Smart, Pointer >, Args, List >;

            public:
                base_inout_ptr_impl( Smart& ptr, Args&& args ) noexcept
                        : base_t( ptr, std::move( args ) )
                {
                    static_assert(
                        is_releasable< Smart >::value || std::is_pointer< Smart >::value,
                        "You cannot use an inout pointer with something that cannot release() its pointer!" );
                }

                base_inout_ptr_impl( base_inout_ptr_impl&& right ) noexcept
                        : base_t( std::move( right ) )
                {
                }

                base_inout_ptr_impl& operator=( base_inout_ptr_impl&& right ) noexcept
                {
                    static_cast< base_t& >( *this ) = std::move( right );
                }
            };

        }
    }
}
