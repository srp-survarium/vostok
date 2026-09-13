// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HIT_TARGETS_CREATION_UTILS_INLINE_H_INCLUDED
#define HIT_TARGETS_CREATION_UTILS_INLINE_H_INCLUDED

namespace vostok {
namespace collision {

template < class ConfigValueType >
u32 get_bones_count_from_hit_targets_config( ConfigValueType const& config )
{
	ConfigValueType const targets_table			= config["hit_targets"];
	return										targets_table.size();
}

} // namespace collision
} // namespace vostok

#endif // #ifndef HIT_TARGETS_CREATION_UTILS_INLINE_H_INCLUDED
