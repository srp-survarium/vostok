// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_SLOT_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_TEXTURE_SLOT_H_INCLUDED
#include <vostok/fixed_string.h>
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/res_texture.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

typedef intrusive_ptr<
	res_texture,
	resource_intrusive_base,
	threading::single_threading_policy
> res_texture_ptr;

class texture_slot {
public:
	texture_slot( ) : slot_id( enum_slot_ind_null ), texture( 0 ) { }

	fixed_string<64>	name;
	u32					slot_id;
	res_texture_ptr		texture;
};

STATIC_SIZE_ASSERT( texture_slot, 0x54 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_SLOT_H_INCLUDED
