// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HIT_TARGETS_CREATION_UTILS_H_INCLUDED
#define HIT_TARGETS_CREATION_UTILS_H_INCLUDED

namespace vostok {
namespace collision {

// sushi@TODO: What can this be instantiated but `vostok::configs::binary_config_value`.
template < class ConfigValueType >
u32 get_bones_count_from_hit_targets_config( ConfigValueType const& config );

} // namespace collision
} // namespace vostok

#include <vostok/collision/hit_targets_creation_utils_inline.h>

#endif // #ifndef HIT_TARGETS_CREATION_UTILS_H_INCLUDED
