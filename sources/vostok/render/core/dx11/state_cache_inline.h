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

	state_record record;
	record.crc = crc;
	create_state( desc, &record.state );
	states.push_back( record );
	return record.state;
}

// claude@NOTE: the target keeps the `result != u32( -1 )` compare as a real
// 5-byte `cmp/je` before the found-return (its own line record); our build
// jump-threads it away, so all four `find` instantiations stay one TRGT_ONLY
// statement short. Not reproducible from source without an unfaithful hack.
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
