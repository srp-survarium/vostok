#include "pch.h"

// Game-module /OPT:REF reachability anchor for the WEAPONS/COOKS cluster:
//   weapon, weapon_cook, object_weapon, sound_player_cook, project_cooker_simple,
//   key_binder.
//
// These carcass TUs compile into game.lib but no reachable engine call graph
// touches them, so /OPT:REF strips them from the EXE (objdiff then reports their
// target symbols as "unpaired"). use_game_weapons() address-takes each public/
// virtual/private method through a volatile sink so the linker keeps the symbol.
// Address-of-member is an UNCONDITIONAL reference that survives /OPT:REF (a method
// CALL inside the never-taken self-guard branch does NOT - the optimizer elides it),
// so everything here is a member-pointer take rather than a constructed call.
//
// Dispatched from anchor_game() (anchor_game.cpp) via a single call line. Retire
// once the real game call graph (the cook registry + human_npc weapon spawning)
// reaches these for itself.

#include "weapon.h"
#include "weapon_cook.h"
#include "shotgun_weapon_reload_state_cook.h"
#include "object_weapon.h"
#include "sound_player_cook.h"
#include "project_cooker_simple.h"
#include "key_binder.h"
#include "weapon_sound_effect.h"
#include "weapon_sound_events_handler_state_cook.h"
#include "damage_zone.h"
#include "game.h"

#include <vostok/game_core/weapon_state_creation_params.h>

#include <vostok/math_float4x4.h>
#include <vostok/resources_query_result.h>

namespace survarium {
	class game_object_;
	class simple_game_project;
	struct base_player;
	// file-local free helpers in weapon.cpp (no public header)
	bool is_dead( base_player*& user );
	bool is_alive( base_player*& user );
	float freeze_at_end_time_calculator( float, float, u32, u32, u32, float );
	// free helper in weapon_sound_effect.cpp (no public header)
	void on_sound_finished( buffer_vector< sound::sound_instance_proxy_ptr >& instances, sound::sound_instance_proxy const& instance );
} // namespace survarium

namespace vostok
{
	static pcvoid volatile s_weapon_sink = 0;

	template < typename T >
	static void keep( T m ) { s_weapon_sink = *( pcvoid const* )&m; }

	void use_game_weapons( )
	{
		// guarded construction keeps the out-of-line ctors/dtors (a method CALL here
		// would be stripped, but the full-object construction reference survives).
		// object_weapon is abstract in our engine headers (ai::game_object::
		// get_collision_object is pure here but absent from the shipped base) so it
		// cannot be constructed - its ctor/dtor stay unreachable until that resolves.
		static volatile bool s_run = false;
		if( s_run )
		{
			static survarium::game* volatile			s_game	= 0;
			static sound::world* volatile				s_snd	= 0;
			survarium::weapon					wpn( 0u, 0u, 0u );
			survarium::sound_player_cook		scook( s_snd, resources::class_id_enum( 0 ) );
			survarium::project_cooker_simple	pcook( false );

			// play_weapon_shell_pfx / play_weapon_fire_pfx receive `this` in esi
			// under LTCG (no push/pop of the this-reg); a guarded DIRECT CALL
			// through a volatile this-ptr reproduces that convention, where the
			// address-take sink below would force a stock __thiscall (this in ecx).
			static survarium::weapon* volatile			s_wpn_pfx	= 0;
			s_wpn_pfx->play_weapon_shell_pfx( );
			s_wpn_pfx->play_weapon_fire_pfx( );

			// Construct each handler state so its VTABLE (carrying the real
			// initialize/finalize bodies, now un-walled) is emitted; the cook's
			// new_state is the real construction site but is still a STUB. Volatile
			// args prevent constant-folding the construction away.
			static survarium::weapon* volatile			s_h_wpn		= 0;
			static float volatile						s_h_ts		= 0.f;
			static resources::managed_resource_ptr* volatile	s_h_anims	= 0;
			static u8 volatile							s_h_u8		= 0;
			static void* volatile						s_h_buf		= 0;
			static bool volatile						s_h_flag	= false;
			#define MAKE_HANDLER( T )	survarium::weapon_sound_events_handler_state< survarium::T > h_##T ( \
				*s_h_wpn, s_h_ts, s_h_anims, s_h_u8, s_h_buf, s_h_u8, s_h_flag, s_h_u8 )
			MAKE_HANDLER( weapon_core_reload_state );
			MAKE_HANDLER( weapon_core_chamber_a_round_state );
			MAKE_HANDLER( weapon_core_chamber_a_round_aimed_state );
			MAKE_HANDLER( weapon_core_fire_state );
			MAKE_HANDLER( weapon_core_aimed_fire_state );
			MAKE_HANDLER( weapon_core_shotgun_reload_start_substate );
			MAKE_HANDLER( weapon_core_shotgun_reload_one_round_substate );
			MAKE_HANDLER( weapon_core_shotgun_reload_finish_substate );
			MAKE_HANDLER( double_barreled_weapon_core_reload_state );
			MAKE_HANDLER( double_barreled_weapon_core_fire_state );
			MAKE_HANDLER( double_barreled_weapon_core_aimed_fire_state );
			MAKE_HANDLER( pistol_weapon_core_reload_state );
			MAKE_HANDLER( pistol_weapon_core_fire_state );
			MAKE_HANDLER( pistol_weapon_core_aimed_fire_state );
			#undef MAKE_HANDLER
			// show/hide instantiations take the 9-arg ctor (bool& shown)
			static bool volatile						s_h_shown	= false;
			#define MAKE_HANDLER_SHOWN( T )	survarium::weapon_sound_events_handler_state< survarium::T > h_##T ( \
				*s_h_wpn, s_h_ts, s_h_anims, s_h_u8, s_h_buf, s_h_u8, s_h_flag, s_h_u8, (bool&)s_h_shown )
			MAKE_HANDLER_SHOWN( weapon_core_show_state );
			MAKE_HANDLER_SHOWN( weapon_core_hide_state );
			MAKE_HANDLER_SHOWN( double_barreled_weapon_core_show_state );
			MAKE_HANDLER_SHOWN( double_barreled_weapon_core_hide_state );
			MAKE_HANDLER_SHOWN( pistol_weapon_core_show_state );
			MAKE_HANDLER_SHOWN( pistol_weapon_core_hide_state );
			#undef MAKE_HANDLER_SHOWN
		}

		// ---- weapon ----------------------------------------------------------
		typedef survarium::weapon w;
		keep( &w::load_weapon );
		keep( &w::set_fire_bullet_transform );
		keep( &w::tick );
		keep( &w::set_transform );
		keep( &w::set_target );
		keep( &w::set_next_fire_queue_type );
		keep( &w::set_next_ammo_type );
		keep( &w::on_ammo_empty );
		keep( &w::show_crosshair );
		keep( &w::hide_crosshair );
		keep( &w::update_pfx_transform );
		keep( &w::show_laser_pointer );
		keep( &w::activate );
		keep( &w::deactivate );
		keep( &w::on_before_fire );
		keep( &w::on_after_fire );
		keep( &w::on_reload );
		keep( &w::on_chamber_a_round );
		keep( &w::on_reload_started );
		keep( &w::on_show );
		keep( &w::on_hide );
		keep( &w::on_unload_chambered_round );
		keep( &w::instant_aim_start );
		keep( &w::instant_aim_end );
		keep( &w::on_skeleton_matrices_changed );
		keep( &w::process_finger_correction );
		keep( &w::on_foot_step );
		keep( &w::on_shell_extraction_event );
		keep( &w::calculate_locator );
		keep( &w::on_hand_correction_event );
		keep( &w::on_user_sprint );

		// ---- weapon_cook -----------------------------------------------------
		typedef survarium::weapon_cook wc;
		keep( &wc::on_weapon_subresources_ready );
		keep( &wc::on_weapon_config_loaded );
		keep( &wc::delete_resource );
		keep( &wc::cooked_object_size );
		// ---- shotgun_weapon_reload_state_cook --------------------------------
		typedef survarium::shotgun_weapon_reload_state_cook src;
		keep( &src::allocate_resource );
		keep( &src::deallocate_resource );
		keep( &src::create_resource );
		keep( &src::destroy_resource );

		// ---- object_weapon ---------------------------------------------------
		typedef survarium::object_weapon ow;
		keep( &ow::cast_game_object );
		keep( &ow::is_loaded );
		keep( &ow::get_name );
		keep( static_cast< vostok::ai::npc* ( ow::* )( ) >( &ow::cast_npc ) );
		keep( static_cast< vostok::ai::weapon* ( ow::* )( ) >( &ow::cast_weapon ) );
		keep( &ow::get_velocity );
		keep( &ow::get_luminosity );
		keep( &ow::get_random_surface_point );
		keep( &ow::local_to_cell );

		// ---- sound_player_cook ----------------------------------------------
		// on_sounds_loaded / on_config_loaded are private (AAE); reached
		// transitively from the public translate_query bind chain, so /OPT:REF
		// keeps them without an (inaccessible) explicit keep().
		typedef survarium::sound_player_cook spc;
		keep( &spc::translate_query );
		keep( &spc::delete_resource );

		// ---- project_cooker_simple ------------------------------------------
		// on_game_project_loaded / create_game_objects / on_damage_zones_loaded /
		// on_ladders_loaded / on_collision_and_visuals_loaded are private (AAE);
		// reached transitively from the public translate_query bind chain, so
		// /OPT:REF keeps them. on_object_loaded has a matched body but its sole
		// bind site is in the still-stubbed create_game_objects, so address-take it
		// directly (use_game_weapons is a friend of project_cooker_simple).
		typedef survarium::project_cooker_simple pcs;
		keep( &pcs::translate_query );
		keep( &pcs::delete_resource );
		keep( &pcs::on_object_loaded );

		// ---- key_binder ------------------------------------------------------
		typedef survarium::key_binder kb;
		keep( &kb::unbind_key );
		keep( &kb::dik_to_keyname );
		keep( &kb::get_binding_group );
		// keyname_to_ptr / action_name_to_id / action_name_to_ptr / remap_keys are
		// private (AAE) and reached transitively from the public bind_key/unbind_key
		// chain, so /OPT:REF keeps them without an explicit (and inaccessible) keep().

		// weapon.cpp file-local free helpers
		keep( &survarium::is_dead );
		keep( &survarium::is_alive );
		keep( &survarium::freeze_at_end_time_calculator );

		// ---- weapon_sound_effect --------------------------------------------
		typedef survarium::weapon_sound_effect wse;
		keep( &wse::initialize );
		keep( &wse::finalize );
		keep( &wse::on_sound_event );
		keep( &survarium::on_sound_finished );
		// the handler-state virtuals initialize/finalize are now bodied (the
		// weapon_core accessor map is recovered); the guarded construction block above
		// emits each handler vtable so /OPT:REF keeps them until the cook new_state
		// (the real instantiation site, still a STUB) reaches them.
		if( s_run )
		{
			// volatile args so the inlined construction is not constant-folded
			// (which would shadow the real out-of-line ctor in objdiff)
			static survarium::weapon* volatile			s_wpn	= 0;
			static sound::sound_emitter_ptr* volatile		s_emit	= 0;
			static sound::sound_instance_proxy_ptr* volatile	s_inst	= 0;
			static u8 volatile							s_count	= 0;
			static bool volatile						s_flag	= false;
			survarium::weapon_sound_effect		wse_inst( *s_wpn, s_emit, s_count, s_inst, s_count, s_emit, s_count, s_inst, s_count, s_flag, s_count );
		}

		// ---- animation time-scale calculators (cdecl free functions) --------
		keep( &survarium::reload_animation_time_scale_calculator );
		keep( &survarium::fire_animation_time_scale_calculator );
		keep( &survarium::shotgun_reload_timescale_calculator );
		keep( &survarium::always_unit_timescale_calculator );

		// ---- damage_zone -----------------------------------------------------
		// the ctor/dtor reach via damage_zone_cook, but load / play_particles /
		// stop_particles / deactivate have no reachable call site yet (zone_group
		// activation is still a carcass), so /OPT:REF would strip them.
		typedef survarium::damage_zone dz;
		keep( &dz::load );
		keep( &dz::play_particles );
		keep( &dz::deactivate );
	}
}
