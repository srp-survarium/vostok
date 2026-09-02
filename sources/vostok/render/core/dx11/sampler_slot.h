// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SAMPLER_SLOT_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SAMPLER_SLOT_H_INCLUDED
#include <vostok/fixed_string.h>
#include <vostok/render/core/render_include.h>

namespace vostok {
namespace render {

class sampler_slot {
public:
	sampler_slot( ) :
		slot_id( enum_slot_ind_null ),
		state( 0 )
	{
	}

	sampler_slot( fixed_string<64> const& name, u32 slot_id, ID3D11SamplerState* state ) :
		name( name ),
		slot_id( slot_id ),
		state( state )
	{
	}

	fixed_string<64> name;
	u32 slot_id;
	ID3D11SamplerState* state;
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SAMPLER_SLOT_H_INCLUDED
