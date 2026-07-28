////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the survarium::player carcass.
// The player out-of-line bodies live in player.cpp; nothing in the (still-stub)
// game call graph constructs a player yet, so without this anchor /OPT:REF strips
// every player symbol out of the base EXE and the delinker has nothing to pair.
//
// We pin the methods by laundering their member-function addresses through a
// volatile sink: the linker must keep them callable (via the normal __thiscall
// ABI, which also stops LTCG handing them a custom register convention), but we
// never run the heavy player ctor (~70KB object). anchor_game() calls
// anchor_game_player().
//
// Retire once the real game call graph (game::create_player and friends) reaches
// the player bodies for itself - a later phase.

#include "pch.h"

#include "player.h"
#include "player_creation_params.h"

namespace vostok {
namespace collision {
	class bone_collision_data;
} // namespace collision
} // namespace vostok

namespace survarium {

struct hit_info;
struct hit_initiator;
class bullet;
class ladder;
class player_actions_subscriber;
struct server_player_update;
class player_input_handler;
class stats_graph;
class game_world_ui;

void anchor_game_player( )
{
	static pcvoid volatile s_sink = 0;

	#define ANCHOR_PIN( expr ) do { void const* const p = *( void const* const* )&( expr ); s_sink = p; } while ( 0 )

	// ctor / dtor have no address-of form; reach them through a guarded
	// construction the compiler emits but never runs (the player object is ~70KB
	// and its ctor dereferences params.game_scene, so guard it behind a never-true
	// volatile flag - placeholders are never touched).
	static volatile bool s_run = false;
	if ( s_run )
	{
		static player_creation_params* volatile params_ptr = 0;
		player p( *params_ptr );
		s_sink = &p;
	}

	ANCHOR_PIN( &player::insert );
	ANCHOR_PIN( &player::apply_hit_directly );
	ANCHOR_PIN( &player::hide );
	ANCHOR_PIN( &player::show );
	ANCHOR_PIN( &player::deserialize );
	ANCHOR_PIN( &player::get_physics_world );
	ANCHOR_PIN( &player::reset_fov_factor );
	ANCHOR_PIN( &player::set_near_plane_factor );
	ANCHOR_PIN( static_cast< void ( player::* )( float, float ) >( &player::set_target_fov_factor ) );
	ANCHOR_PIN( &player::fov_factor );
	ANCHOR_PIN( static_cast< void ( player::* )(
		hit_initiator const* const, const u32, pcstr, const float, const float, bullet* const
	) >( &player::hit ) );
	ANCHOR_PIN( static_cast< void ( player::* )(
		hit_initiator const* const, collision::bone_collision_data const&, pcstr, const float, const float, bullet* const
	) >( &player::hit ) );
	ANCHOR_PIN( &player::get_speed );
	ANCHOR_PIN( &player::use_ladder );
	ANCHOR_PIN( &player::set_head_visibility );
	ANCHOR_PIN( &player::skeleton );
	ANCHOR_PIN( &player::subscribe_on_actions );
	ANCHOR_PIN( &player::unsubscribe_from_actions );
	ANCHOR_PIN( &player::on_fire );
	ANCHOR_PIN( &player::jump );
	ANCHOR_PIN( &player::end_jump );
	ANCHOR_PIN( &player::crouch );
	ANCHOR_PIN( &player::stand_up );
	ANCHOR_PIN( &player::set_new_active_item );
	ANCHOR_PIN( &player::take_inventory_item );
	ANCHOR_PIN( &player::get_engine );
	ANCHOR_PIN( static_cast< void ( player::* )(
		pcstr, boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&,
		pcvoid, resources::managed_resource_ptr const&, const u8, pcvoid const
	) >( &player::subscribe_animation_player ) );
	ANCHOR_PIN( static_cast< void ( player::* )(
		animation::reserved_channel_ids_enum, boost::function< enum animation::callback_return_type_enum( animation::animation_callback_params& ) > const&,
		pcvoid, resources::managed_resource_ptr const&, pcvoid const
	) >( &player::subscribe_animation_player ) );
	ANCHOR_PIN( static_cast< void ( player::* )( pcstr, pcvoid ) >( &player::unsubscribe_animation_player ) );
	ANCHOR_PIN( static_cast< void ( player::* )( animation::reserved_channel_ids_enum, pcvoid ) >( &player::unsubscribe_animation_player ) );
	ANCHOR_PIN( &player::select_animations );
	ANCHOR_PIN( &player::render );
	ANCHOR_PIN( &player::render_crosshair_info );
	ANCHOR_PIN( static_cast< void ( player::* )( client_player_state&, float2 const& ) >( &player::apply_input ) );
	ANCHOR_PIN( static_cast< void ( player::* )( client_player_state&, float2 const&, float2 const&, const float ) >( &player::apply_input ) );
	ANCHOR_PIN( &player::get_transform_for_animation_player );
	ANCHOR_PIN( &player::detect_usable_objects );
	ANCHOR_PIN( &player::on_before_active_object_changed );
	ANCHOR_PIN( &player::insert_alive );
	ANCHOR_PIN( &player::compute_bones );

	#undef ANCHOR_PIN
}

} // namespace survarium
