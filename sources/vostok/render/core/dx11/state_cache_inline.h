// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_STATE_CACHE_INLINE_H_INCLUDED
namespace vostok {
namespace render {

template < typename device_state, typename state_desc >
inline device_state* state_cache< device_state, state_desc >::get_state( state_desc const& desc )
{
	device_state* res;

	u32 crc = state_utils::get_hash( desc );

	res = find( desc, crc );

	if ( !res )
	{
		state_record rec;
		rec.crc = crc;
		create_state( desc, &rec.state );
		res = rec.state;
		states.push_back( rec );
	}

	return res;
}

template < typename device_state, typename state_desc >
inline device_state* state_cache< device_state, state_desc >::find( state_desc const& desc, u32 CRC )
{
	u32 res = 0xffffffff;
	for ( u32 i = 0; i < states.size( ); ++i )
	{
		if ( states[i].crc == CRC )
		{
			state_desc desc_candidate;
			states[i].state->GetDesc( &desc_candidate );
			if ( desc_candidate == desc )
			{
				res = i;
				break;
			}
		}
	}

	if ( res != 0xffffffff )
		return states[res].state;
	else
		return NULL;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_INLINE_H_INCLUDED
