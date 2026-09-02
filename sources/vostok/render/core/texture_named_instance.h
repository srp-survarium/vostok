// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_TEXTURE_NAMED_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_CORE_TEXTURE_NAMED_INSTANCE_H_INCLUDED
#include <vostok/fixed_string.h>

namespace vostok {
namespace render {

class res_texture;

struct texture_named_instance {
	res_texture* texture;
	fixed_string<260> path;
};

STATIC_SIZE_ASSERT( texture_named_instance, 0x114 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_TEXTURE_NAMED_INSTANCE_H_INCLUDED
