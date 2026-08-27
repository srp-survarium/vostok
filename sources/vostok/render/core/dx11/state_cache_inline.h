#ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_STATE_CACHE_INLINE_H_INCLUDED

namespace vostok {
namespace render {

template < typename device_state, typename state_desc >
inline device_state* state_cache< device_state, state_desc >::get_state( state_desc const& desc )
{
	u32 const crc = state_utils::get_hash( desc );

	device_state* result = find( desc, crc );

	if ( result )
		return result;

	state_record rec;
	rec.crc = crc;
	create_state( desc, &rec.state );

	states.push_back( rec );

	return rec.state;
}

// claude@NOTE: the target retains the final result check; the base jump-threads it away.
template < typename device_state, typename state_desc >
inline device_state* state_cache< device_state, state_desc >::find( state_desc const& desc, u32 crc )
{
	u32 result = u32( -1 );
	for ( u32 index = 0; index < states.size( ); ++index ) {
		if ( states[index].crc != crc )
			continue;

		state_desc desc_candidate;
		states[index].state->GetDesc( &desc_candidate );
		if ( desc_candidate == desc ) {
			result = index;
			break;
		}
	}

	if ( result != u32( -1 ) )
		return states[result].state;

	return 0;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_STATE_CACHE_INLINE_H_INCLUDED
