////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED
#define WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED

namespace survarium {

template < typename T >
inline weapon_sound_events_handler_state_cook< T >::weapon_sound_events_handler_state_cook( ) :
	resources::unmanaged_cook( weapon_sound_events_handler_state_cook_class< T >( ), reuse_false, use_current_thread_id, use_current_thread_id )
{
	resources::register_cook( this );
}

template < typename T >
mutable_buffer weapon_sound_events_handler_state_cook< T >::allocate_resource(
	resources::query_result_for_cook&		in_query,
	const_buffer							raw_file_data,
	const bool								file_exist
)
{
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( T ), "weapon_sound_events_handler_state" ), sizeof( T ) );
}

template < typename T >
void weapon_sound_events_handler_state_cook< T >::deallocate_resource( void* buffer )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );
}

// STATE[STUB]
// claude@NOTE: PARKED - large (90-stmt, 0x38d-byte) sound-config parser. Reachable
// + pairs (private EAE) but body unwritten. Recovered shape (target 0x98ad0, show
// instance): parse the sounds config from parent.user_data, build a buffer_vector<
// resources::request> requests over cfg["sounds"][...] entries (class sound_class),
// then resources::query_resources( requests, count, boost::bind(
// &on_subresources_ready, this, _1, in_out_unmanaged_resource_buffer, params,
// config_params{cfg} ), g_allocator, NULL, &parent ); parent.finish_query(
// result_postponed ). NEXT: reconstruct statement-by-statement vs the PDB line
// table (lines 43-134) like weapon_core_state_cook_template::create_resource (its
// sibling, already matched), converging with --view structure-diff. Mirror that
// template's create_resource form (fixed_vector requests + push_back loop + bind).
template < typename T >
void weapon_sound_events_handler_state_cook< T >::create_resource(
	resources::query_result_for_cook&		parent,
	const_buffer							raw_file_data,
	mutable_buffer							in_out_unmanaged_resource_buffer
)
{
}

// STATE[STUB]
// claude@NOTE: PARKED - 38-stmt sound installer (target 0x9f7f0, show instance).
// Recovered shape: build a buffer_vector<managed_resource_ptr> animations from
// data[i].get_managed_resource(); compute total_sounds_count; alloc
// buffer_for_sounds; new_state( buffer, params, animations.begin(),
// animations.size(), buffer_for_sounds, total_sounds_count, config_parameters );
// set_unmanaged_resource + finish_query( result_success ). NEXT: reconstruct vs the
// PDB line table (lines 148-181); calls new_state (also parked). Mirror
// weapon_core_state_cook_template::on_subresources_ready (sibling, matched).
template < typename T >
inline void weapon_sound_events_handler_state_cook< T >::on_subresources_ready(
	resources::queries_result&		data,
	mutable_buffer					buffer,
	weapon_state_creation_params const* const	params,
	config_params					config_parameters
)
{
}

template < typename T >
void weapon_sound_events_handler_state_cook< T >::destroy_resource( resources::unmanaged_resource* const resource )
{
	static_cast< T* >( resource )->~T( );
	VOSTOK_FREE_IMPL( g_allocator, ( pvoid& )static_cast< T* >( resource )->m_buffer_for_sounds );
}

// no standalone target symbol for the primary new_state (only the show/hide
// specializations survive; the other instances inline into on_subresources_ready).
// The 6 surviving specializations live in weapon_sound_events_handler_state_cook_specializations.h.
// STATE[STUB]
template < typename T >
inline T* weapon_sound_events_handler_state_cook< T >::new_state(
	mutable_buffer		buffer,
	weapon_state_creation_params const* const	params,
	resources::managed_resource_ptr const* const	animations,
	const u8			animations_count,
	void* const			sounds,
	const u8			sounds_count,
	config_params const&	config_parameters
)
{
	return NULL;
}

} // namespace survarium

#endif // #ifndef WEAPON_SOUND_EVENTS_HANDLER_STATE_COOK_INLINE_H_INCLUDED
