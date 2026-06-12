////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state_cook.h>

#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include "game_core_memory.h"

namespace survarium {

// STATE[100%|DONE]
weapon_core_shotgun_reload_state_cook::~weapon_core_shotgun_reload_state_cook( )
{
}

// STATE[41.38%|PARTIAL]: the guarded-eater statement was VOSTOK_UNREFERENCED_PARAMETERS
// (identity(false) guard + variadic helper taking in_query BY VALUE - the 0x258 rep movsd
// copy), not an ASSERT; residual is the mutable_buffer(pvoid,u32) ctor inlined to two
// field stores in target vs the out-of-line uint2::uint2 COMDAT-fold call in base.
mutable_buffer weapon_core_shotgun_reload_state_cook::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	VOSTOK_UNREFERENCED_PARAMETERS( in_query, raw_file_data, file_exist );
	return mutable_buffer( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( weapon_core_shotgun_reload_state ), "weapon_core_shotgun_reload_state" ), sizeof( weapon_core_shotgun_reload_state ) );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts
	// SIZE -0x6 | 25 | return mutable_buffer( VOSTOK_MALLOC_IMPL( ... ), sizeof( ... ) );
	// VERDICT: STRUCTURE MATCH (shape ok) - the eater row now matches byte-for-byte; sole SIZE
	// is the mutable_buffer ctor inline-vs-call, non-steerable LTCG.
}

// STATE[55.64%|PARTIAL]: target calls free_helper<doug_lea_allocator,resource_base>
// OUT-OF-LINE (lea edi,[ebp+8]=&buffer; mov ecx,eax=allocator; call free_helper);
// base INLINES the free_helper wrapper - it inlines the `if(!pointer) return` null
// check and calls free_helper_impl directly, spilling the allocator to an extra
// [ebp-4] temp (sub esp,8 vs target push ecx). Same inline-depth divergence as
// weapon_core_cook::delete_resource; the allocator/type are correct (g_allocator
// pointer, T=resource_base).
void weapon_core_shotgun_reload_state_cook::deallocate_resource( void* buffer )
{
	VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE +0xf | 53 | VOSTOK_FREE_IMPL( g_allocator, (resources::resource_base*&)buffer );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the free_helper wrapper
	// inline-vs-call, non-steerable LTCG.
}

// STATE[49/49 stmts|DONE]
void weapon_core_shotgun_reload_state_cook::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	typedef fixed_vector< resources::request, 24 >	requests_fixed_type;

	weapon_state_creation_params const*	params	= static_cast< weapon_state_creation_params const* >( raw_file_data.c_ptr( ) );

	configs::binary_config_value		cfg;

	if ( !parent.user_data( )->try_get( cfg ) )
	{
		FATAL( "cannot get weapon config" );
		parent.finish_query( cook_base::result_error, assert_on_fail_false );
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION_T( cfg.value_exists( "animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( cfg.value_exists( "user_animations" ) ) );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	requests_fixed_type				requests;

	configs::binary_config_value	start_weapon_anim_cfg			= cfg["animations"]["start_substate"];
	for ( u32 i = 0; i < 4; ++i ) {
		requests.push_back( create_request( pcstr( start_weapon_anim_cfg[ i ] ), resources::animation_class ) );
	}

	configs::binary_config_value	start_user_anim_cfg				= cfg["user_animations"]["start_substate"];
	for ( u32 i = 0; i < 4; ++i ) {
		requests.push_back( create_request( pcstr( start_user_anim_cfg[ i ] ), resources::animation_class ) );
	}

	configs::binary_config_value	reload_one_weapon_anim_cfg		= cfg["animations"]["reload_one_substate"];
	for ( u32 i = 0; i < 4; ++i ) {
		requests.push_back( create_request( pcstr( reload_one_weapon_anim_cfg[ i ] ), resources::animation_class ) );
	}

	configs::binary_config_value	reload_one_user_anim_cfg		= cfg["user_animations"]["reload_one_substate"];
	for ( u32 i = 0; i < 4; ++i ) {
		requests.push_back( create_request( pcstr( reload_one_user_anim_cfg[ i ] ), resources::animation_class ) );
	}

	configs::binary_config_value	finish_weapon_anim_cfg			= cfg["animations"]["finish_substate"];
	for ( u32 i = 0; i < 4; ++i ) {
		requests.push_back( create_request( pcstr( finish_weapon_anim_cfg[ i ] ), resources::animation_class ) );
	}

	configs::binary_config_value	finish_user_anim_cfg			= cfg["user_animations"]["finish_substate"];
	for ( u32 i = 0; i < 4; ++i ) {
		requests.push_back( create_request( pcstr( finish_user_anim_cfg[ i ] ), resources::animation_class ) );
	}

	ASSERT( UNKNOWN_EXPRESSION );

	resources::query_resources(
		requests.begin( ),
		requests.size( ),
		boost::bind(
			&weapon_core_shotgun_reload_state_cook::on_subresources_ready,
			this,
			_1,
			in_out_unmanaged_resource_buffer,
			params
		),
		g_allocator,
		0,
		&parent,
		assert_on_fail_true
	);

	parent.finish_query( cook_base::result_success, assert_on_fail_true );
}

// STATE[65.65%|PARTIAL]: STRUCTURE MATCH (21/21 stmts). All 3 loops now use braced scope.
// Residual SIZE diffs are LTCG inline-vs-call (push_back, VOSTOK_NEW_IMPL, intrusive_ptr
// ctors). Non-steerable.
void weapon_core_shotgun_reload_state_cook::on_subresources_ready( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params )
{
	typedef fixed_vector< resources::managed_resource_ptr, 8 >	start_animations_fixed_type;
	typedef fixed_vector< resources::managed_resource_ptr, 8 >	finish_animations_fixed_type;
	typedef fixed_vector< resources::managed_resource_ptr, 8 >	reload_one_animations_fixed_type;

	ASSERT( UNKNOWN_EXPRESSION_T( data.size( ) == 24 ) );

	u32	resource_index	= 0;

	start_animations_fixed_type		start_animations;

	for ( u32 i = 0; i < 8; ++i ) {
		start_animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ resource_index++ ].get_managed_resource( ) ) );
	}

	finish_animations_fixed_type	finish_animations;

	for ( u32 i = 0; i < 8; ++i ) {
		finish_animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ resource_index++ ].get_managed_resource( ) ) );
	}

	reload_one_animations_fixed_type	reload_one_animations;

	for ( u32 i = 0; i < 8; ++i ) {
		reload_one_animations.push_back( static_cast_resource_ptr< resources::managed_resource_ptr >( data[ resource_index++ ].get_managed_resource( ) ) );
	}

	const float							animations_timescale	= computed_shotgun_reload_animation_time_scale( *start_animations.begin( ), params->weapon.get_magazine_capacity( ), params->reload_time );

	weapon_core_shotgun_reload_start_substate*	reload_start		= VOSTOK_NEW_IMPL( g_allocator, weapon_core_shotgun_reload_start_substate )( params->weapon, animations_timescale, &*start_animations.begin( ), start_animations.size( ) );
	weapon_core_shotgun_reload_one_round_substate*	reload_one_round	= VOSTOK_NEW_IMPL( g_allocator, weapon_core_shotgun_reload_one_round_substate )( params->weapon, animations_timescale, &*finish_animations.begin( ), finish_animations.size( ) );
	weapon_core_shotgun_reload_finish_substate*	reload_finish		= VOSTOK_NEW_IMPL( g_allocator, weapon_core_shotgun_reload_finish_substate )( params->weapon, animations_timescale, &*reload_one_animations.begin( ), reload_one_animations.size( ) );

	weapon_core_shotgun_reload_state*	object_to_cook	= new ( buffer.c_ptr( ) ) weapon_core_shotgun_reload_state( params->weapon, reload_start, reload_one_round, reload_finish );

	resources::query_result_for_cook*	parent	= data.get_parent_query( );
	parent->set_unmanaged_resource(
		resources::unmanaged_resource_ptr( object_to_cook ),
		resources::memory_usage_type( resources::nocache_memory, sizeof( weapon_core_shotgun_reload_state ) )
	);
	// STRUCTURE DIFF: target 21 stmts / base 21 stmts
	// SIZE +0x17 | 163 | start_animations_fixed_type start_animations;
	// SIZE +0x20 | 165 | for ( u32 i = 0; i < 8; ++i ) {
	// SIZE +0x73 | 166 | start_animations.push_back( ... );
	// SIZE +0x14 | 169 | finish_animations_fixed_type finish_animations;
	// SIZE -0x22 | 171 | for ( u32 i = 0; i < 8; ++i ) {
	// SIZE -0x78 | 175 | reload_one_animations_fixed_type reload_one_animations;
	// SIZE -0x67 | 177 | for ( u32 i = 0; i < 8; ++i ) {
	// SIZE +0x1d | 181 | reload_start = VOSTOK_NEW_IMPL( ... );
	// SIZE -0x4e | 187 | parent = data.get_parent_query( );
	// SIZE +0x44 | 188 | parent->set_unmanaged_resource( ... );
	// VERDICT: STRUCTURE MATCH (21/21) - all SIZE diffs are LTCG inline-vs-call: intrusive_ptr
	// ctors on fixed_vector, VOSTOK_NEW_IMPL, get_parent_query(), and set_unmanaged_resource
	// resource_ptr ctor. Non-steerable.
	parent->finish_query( cook_base::result_success, assert_on_fail_true );
}

// STATE[100%|DONE]
void weapon_core_shotgun_reload_state_cook::destroy_resource( resources::unmanaged_resource* resource )
{
	weapon_core_shotgun_reload_state*	wpn_state	= static_cast< weapon_core_shotgun_reload_state* >( resource );
	wpn_state->~weapon_core_shotgun_reload_state( );
}

} // namespace survarium
