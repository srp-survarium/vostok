#include "pch.h"
#include "temp_include_all.h"

#include <vostok/physics/sources/bullet_include.h>

#include "../../collision/sources/loose_oct_tree.h"

#include <vostok/ai/npc_statistics.h>

#include <vostok/collision/animated_object.h>
#include <vostok/collision/bone_collision_data.h>
#include <vostok/collision/api.h>

#include <vostok/configs_binary_config_value.h>
#include <vostok/managed_allocator.h>

#include <vostok/render/facade/one_way_render_channel.h>
#include <vostok/render/world.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/debug_renderer.h>

#include <vostok/physics/animated_rigid_body.h>
#include <vostok/physics/api.h>
#include <vostok/physics/character_controller.h>
#include <vostok/physics/collision_shapes.h>
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/physics/engine.h>
#include <vostok/physics/ghost_object.h>
#include <vostok/physics/sources/bullet_character_controller.h>
#include <vostok/physics/static_rigid_body.h>

// #include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <vostok/network_core/http_client.h>
#include <vostok/network_core/tcp_packet.h>

#include <vostok/animation/skeleton.h>

#include <vostok/game_core/affects_threshold.h>
#include <vostok/game_core/breath_vibration_calculator.h>
#include <vostok/game_core/breath_holding_params.h>
#include <vostok/game_core/bullet.h>
#include <vostok/game_core/bullet_manager.h>
#include <vostok/game_core/client_player_update.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_sensor.h>
#include <vostok/game_core/damage_model_cook.h>
#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/inventory_item_props.h>
#include <vostok/game_core/interactive_object.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/weapon_user_animations_container_cook.h>
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/ladder.h>
#include <vostok/game_core/medkit.h>
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/legs_ik_processor.h>
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/game_core/player_logic_base_state.h>
#include "player_logic_jump_state.h"
#include "jump_logic_state_inactive.h"
#include "jump_logic_state_landing.h"
#include "jump_logic_state_start.h"
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/player_stealth.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_state.h>
#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_core_animation_end_aware_state.h>
#include <vostok/game_core/weapon_core_idle_state_base.h>
#include <vostok/game_core/weapon_core_aimed_state_base.h>
#include <vostok/game_core/weapon_core_idle_state.h>
#include <vostok/game_core/weapon_core_aimed_state.h>
#include <vostok/game_core/pistol_weapon_core_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_show_state.h>
#include <vostok/game_core/weapon_core_hide_state.h>
#include <vostok/game_core/double_barreled_weapon_core_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/double_barreled_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_aimed_fire_state.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_show_state_base.h>
#include <vostok/game_core/weapon_core_hide_state_base.h>
#include <vostok/game_core/weapon_core_reload_state_base.h>
#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>
#include <vostok/game_core/weapon_core_fire_state_base.h>
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/pistol_weapon_core_fire_state.h>
#include <vostok/game_core/weapon_core.h>

#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/recoil_calculator.h>
#include <vostok/game_core/character_dispersion_calculator.h>
#include <vostok/game_core/weapon_dispersion_calculator.h>
#include <vostok/game_core/weapon_recoil_params.h>
#include <vostok/game_core/character_dispersion_params.h>
#include <vostok/game_core/weapon_dispersion_params.h>

#include <vostok/game_core/victory_item_core_cook.h>
#include <vostok/game_core/victory_item_core.h>
#include <vostok/game_core/weapon_core_cook.h>
#include <vostok/game_core/artefact_lifebone_core.h>
#include <vostok/game_core/hit_affects_type_enum.h>
#include <vostok/game_core/artefact_container_core.h>
#include <vostok/game_core/generic_anomaly_core.h>
#include <vostok/game_core/anomaly_state.h>
#include <vostok/game_core/zone_group.h>
#include <vostok/game_core/damage_zone_core.h>
#include <vostok/game_core/hit_receiver.h>

#include <vostok/game_core/respawn_point_core.h>
#include <vostok/game_core/booby_trap_core.h>
#include <vostok/game_core/booby_trap_core_cook.h>
#include <vostok/game_core/booby_trap_set_core_cook.h>
#include <vostok/game_core/weapon_core_shotgun_reload_state_cook.h>
#include <vostok/game_core/weapon_core_inactive_state_cook.h>
#include <vostok/game_core/victory_items_container_core.h>
#include <vostok/game_core/inventory.h>
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/items_dictionary.h>
#include <vostok/game_core/player_parameters_modifyer.h>

namespace survarium
{
	void game_core_initialize( );

	float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices );
	float4x4 get_bone_matrix_in_object_space_impl( animation::skeleton_bone const& bone, float4x4 const* matrices, animation::skeleton_bone const* first_non_root_bone );

	float get_additional_length( float3 const& upleg_dir, float3 const& leg_dir, float knee_len );
}

// fsm_state has a pure-virtual dtor (= 0) with no body in our sources; the
// concrete weapon_core_base_state anchor below needs it to LINK (vtable slot).
// Define it here in the (non-target) anchor TU so it can't regress a matched obj.
// Link stopgaps in this non-target TU (cannot regress a matched obj): both dtors
// are declared but undefined in our sources. fsm_state's dtor is the documented
// foundational gap (game_core/README.md); ~player_logic_base_state is the analogous
// gap one level down, pulled in via the abstract class's vtable once the obj is
// kept by the movement_animation_index anchor. claude@NOTE: real bodies belong to
// the ai module / a future ~player_logic_base_state unit; these are just stopgaps.
namespace vostok { namespace ai {
	fsm_state::~fsm_state( ) { }
} }

namespace survarium
{
	player_logic_base_state::~player_logic_base_state( ) { }
}

namespace vostok
{
	// claude@NOTE: game_core_initialize has an empty body (push ebp; mov ebp,esp;
	// pop ebp; ret - byte-identical to target). The LTCG linker ICF-folds this
	// 5-byte empty function into another identical one, so no standalone symbol
	// survives into the base EXE for the delinker to score - it stays on the
	// dummy.obj base regardless of how it is referenced (direct call, address
	// taken, or volatile global pointer were all tried). Body is correct; the
	// gap is purely linker ICF visibility, not a source mismatch.
	void example_callback( const char* name );

	void use_game_core_breath_vibration_calculator( )
	{
		survarium::breath_vibration_calculator	calc;
		survarium::breath_holding_params		params;

		calc.set_breath_holding_params( &params );
		calc.tick( 0, 0.0f );

		// Escape &calc so LTCG observes the ctor's member stores
		// (otherwise the constant-only stores are dead-store-eliminated).
		example_callback( reinterpret_cast< pcstr >( &calc ) );
	}

	void use_game_core_initialize( )
	{
		survarium::game_core_initialize( );
	}

	// claude@MATCH: use_game_core_legs_ik_processor_leg_params removed as redundant.
	// Every leg_params member is reached transitively through the real `processor`
	// instance in use_game_core_legs_ik_processor below: ctor via the processor
	// ctor; set_*_transition_time/set_heel_on_ground/set_toe_on_ground via the
	// private set_*_on_ground(leg_params&,bool) helpers (reached from the public
	// processor setters); activate via processor::activate; tick via processor::tick.
	// (Verified: removing it left every leg_params % byte-for-byte unchanged.)

	// The four public on_ground setters delegate to the private
	// set_heel_on_ground/set_toe_on_ground(leg_params&, bool) helpers; calling them
	// on a live instance keeps all six bodies (the private helpers survive
	// transitively). Escape &processor so LTCG observes the member stores.
	void use_game_core_legs_ik_processor( )
	{
		survarium::legs_ik_processor processor;
		processor.set_left_heel_on_ground( true );
		processor.set_left_toe_on_ground( false );
		processor.set_right_heel_on_ground( true );
		processor.set_right_toe_on_ground( false );

		// anchor the public activate / process / tick (ctor + dtor anchored by the
		// local instance). process() reaches the private process_leg/get_foot_fixed.
		processor.activate( *reinterpret_cast< vostok::animation::skeleton const* >( NULL ) );
		processor.process( reinterpret_cast< float4x4* >( NULL ), *reinterpret_cast< float4x4 const* >( NULL ) );
		processor.tick( 0u );

		example_callback( reinterpret_cast< pcstr >( &processor ) );
	}

	// claude@MATCH: legs_ik_drawer is a thin forwarder over render::debug::renderer.
	// Construct one over NULL renderer/scene refs (never run) and call every public
	// draw_* so the linker keeps all five bodies.
	void use_game_core_legs_ik_drawer( )
	{
		survarium::legs_ik_drawer drawer(
			*reinterpret_cast< render::debug::renderer* >( NULL ),
			render::scene_ptr( )
		);
		float4x4 const&			m	= *reinterpret_cast< float4x4 const* >( NULL );
		float3 const&			v	= *reinterpret_cast< float3 const* >( NULL );
		math::color const&		c	= *reinterpret_cast< math::color const* >( NULL );
		drawer.draw_leg( m, m, m, m, c, c, c, c, 0.0f );
		drawer.draw_cross( v, 0.0f, c, false );
		drawer.draw_origin( m, 0.0f, false );
		drawer.draw_line_capsule( m, v, c, false );
		drawer.draw_solid_capsule( m, v, c, false );
		example_callback( reinterpret_cast< pcstr >( &drawer ) );
	}

	// claude@MATCH: get_foot_fixed_transform / process_leg / get_additional_length
	// are reached transitively from process() (anchored via the real `processor`
	// instance in use_game_core_legs_ik_processor above), so their former direct
	// NULL-cast-observation anchors were redundant and are removed. (Verified: the
	// real call chain keeps all three scored, and each % is byte-for-byte unchanged -
	// the fake observation was clutter, not a codegen distortion; residuals are genuine.)

	// ik_processor's ctor + activate are protected, so reach them through a
	// trivial concrete subclass; escape the object so LTCG keeps the member stores.
	struct concrete_ik_processor : survarium::ik_processor
	{
		void do_activate( animation::skeleton const& skeleton ) { activate( skeleton ); }
	};

	void use_game_core_ik_processor( animation::skeleton const* skeleton, animation::skeleton_bone const* bone, float4x4 const* matrices )
	{
		concrete_ik_processor proc;
		proc.do_activate( *skeleton );
		example_callback( reinterpret_cast< pcstr >( &proc ) );

		// escape the returned float4x4 so LTCG keeps the (observed) body
		float4x4 result	= survarium::get_bone_matrix_in_object_space( *bone, *skeleton, matrices );
		example_callback( reinterpret_cast< pcstr >( &result ) );

		// keep the impl standalone too (also reachable via the wrapper)
		float4x4 ( *fn )( animation::skeleton_bone const&, float4x4 const*, animation::skeleton_bone const* ) =
			&survarium::get_bone_matrix_in_object_space_impl;
		example_callback( reinterpret_cast< pcstr >( fn ) );
	}

	void use_medkit( )
	{
		survarium::player_parameters_modifyer ppm;
		ppm.apply( NULL );
	}

	void use_inventory_2( )
	{
		survarium::inventory inventory;
		survarium::player_profile profile;
		survarium::items_dictionary items_dictionary;
		survarium::inventory_item_ptr item_ptr( NULL );

		inventory.set_holder( NULL );
		inventory.unset_holder( );
		inventory.set_item( survarium::quick_slot1, item_ptr );
		inventory.action( survarium::helmet_slot, false );

		inventory.setup_demo_profile( );
		inventory.setup_from_profile( profile, items_dictionary );
		inventory.unload_to_profile( profile, items_dictionary );

		inventory.remove( );
		inventory.set_victory_item( NULL );
	}

	void use_victory_items_container_core( survarium::victory_items_container_core* victory_items_container_core )
	{
		configs::binary_config_value	config;

		victory_items_container_core->load( config );
		victory_items_container_core->use_initialize( NULL );
		victory_items_container_core->use_execute( NULL );
		victory_items_container_core->use_finalize( NULL );
		victory_items_container_core->use_info( NULL );

		victory_items_container_core->put_item( NULL );
		victory_items_container_core->take_item( );
	}

	void use_booby_trap_cook( )
	{
		class booby_trap_set_cook : public survarium::booby_trap_set_core_cook
		{
			virtual	survarium::booby_trap_set_core*	new_derived_resource		( ) override { return NULL; }
			virtual	u32								get_derived_resource_size	( ) override { return 0; }

			virtual void							delete_resource				( resources::resource_base* resource ) { }

		};

		class booby_trap_cook : public survarium::booby_trap_core_cook
		{
		};

		static booby_trap_set_cook	s_booby_trap_set_cook;
		static booby_trap_cook		s_booby_trap_cook;

		vostok::resources::register_cook( &s_booby_trap_set_cook );
		vostok::resources::register_cook( &s_booby_trap_cook );

	}

	void use_hittable_object( survarium::hittable_object* hittable_object )
	{
		configs::binary_config_value	config;

		hittable_object->load( config );
		hittable_object->set_transform( float4x4( ) ),
		hittable_object->insert( NULL );
		hittable_object->remove( );
	}

	void use_respawn_point_core( )
	{
		survarium::respawn_point_core	respawn_point_core;
		configs::binary_config_value	config;

		respawn_point_core.load( config );
	}

	void use_damage_zone_core( )
	{
		survarium::damage_zone_core		damage_zone;
		configs::binary_config_value	config;

		damage_zone.load( config );

		survarium::hit_receiver_info hit_receiver_info( NULL, NULL );
		hit_receiver_info == hit_receiver_info;
	}

	void use_generic_anomaly_core( )
	{
		survarium::generic_anomaly_core core;
		survarium::scheduler			scheduler( NULL );
		configs::binary_config_value	config;

		core.load( config );
		core.resolve_links( NULL, config );

		core.activate( NULL, scheduler );
		core.deactivate( );
		core.tick( 0, 0 );

		core.on_player_action( NULL, survarium::player_actions_subscriber::walk, 10.0f );

		core.on_zone_act( NULL, NULL );
		core.on_hit_receiver_enter( NULL, NULL );
		core.on_hit_receiver_leave( NULL, NULL );
		core.on_artefact_container_use( NULL );
	}

	void use_artefact_container_core( )
	{
		survarium::artefact_container_core core;

		configs::binary_config_value config;
		core.load( config );
		core.use_initialize( NULL );
		core.use_execute( NULL );
		core.use_finalize( NULL );
		// Qualified (devirtualized) call keeps the standalone `use_info` body
		// (returns the "st_use_artefact_container" literal) for scoring.
		core.artefact_container_core::use_info( NULL );

		survarium::scheduler scheduler( NULL );
		core.activate		( NULL, NULL, scheduler );
		core.deactivate		( );
		core.has_artefact	( );
		core.spawn_artefact	( );
	}

	void use_artefact_lifebone_core( )
	{
		survarium::artefact_lifebone_core lifebone;
		lifebone.holder_assigned( );
		lifebone.holder_removed( );
		lifebone.action( true );
		lifebone.reduce_damage( "body_part_name", "damage_type",  10.0f, 10.0f );

		configs::binary_config_value config;
		lifebone.load_core( config );
	}

	void use_victory_item_core( )
	{
		static survarium::victory_item_core_cook s_victory_item_core_cook;
		vostok::resources::register_cook( &s_victory_item_core_cook );
	}

	void use_weapon_core_cook( )
	{
		static survarium::weapon_core_cook s_weapon_core_cook;
		vostok::resources::register_cook( &s_weapon_core_cook );
	}

	void use_weapon_core_shotgun_reload_state_cook( )
	{
		static survarium::weapon_core_shotgun_reload_state_cook s_weapon_core_shotgun_reload_state_cook;
		vostok::resources::register_cook( &s_weapon_core_shotgun_reload_state_cook );
	}

	void use_weapon_core_inactive_state_cook( )
	{
		static survarium::weapon_core_inactive_state_cook s_weapon_core_inactive_state_cook;
		vostok::resources::register_cook( &s_weapon_core_inactive_state_cook );
	}

	void use_dispersion_calculator( )
	{
		survarium::dispersion_calculator calc;
		calc.get_dispersion( );
		calc.set_weapon( NULL );

		// Escape &calc so LTCG observes the ctor's constant member stores
		// (m_weapon=NULL, m_shooting_skill_coeff=1.0f, m_aiming_speed_coeff=1.0f);
		// get_dispersion() returns early on m_weapon==NULL and never reads the
		// coeffs, so without this the stores are dead-store-eliminated.
		example_callback( reinterpret_cast< pcstr >( &calc ) );
	}

	void use_character_dispersion_calculator( )
	{
		survarium::character_dispersion_calculator calc;
		calc.set_character_dispersion_params( NULL );
		calc.tick( survarium::type_stand, true, false, 0, false, 0 );

		// Escape &calc so LTCG observes the ctor's constant member stores
		// (m_value_smoothing_speed=5.0f, m_aiming_speed=1.0f, etc.); the early
		// returns in tick can leave them otherwise dead-store-eliminated.
		example_callback( reinterpret_cast< pcstr >( &calc ) );
	}

	void use_game_core_weapon_recoil_params( )
	{
		// Anchor both ctors so their member stores are OBSERVED (escape &obj
		// through the opaque sink); otherwise LTCG dead-store-eliminates them
		// and the ctor compiles empty (the #107 18% trap).
		survarium::weapon_recoil_params params;

		configs::binary_config_value cfg;
		survarium::weapon_recoil_params config_params( cfg );

		example_callback( reinterpret_cast< pcstr >( &params ) );
		example_callback( reinterpret_cast< pcstr >( &config_params ) );
	}

	void use_game_core_character_dispersion_params( )
	{
		// Escape &params so the constant-only ctor stores are OBSERVED (else
		// LTCG dead-store-eliminates them and the ctor compiles empty).
		survarium::character_dispersion_params params;

		configs::binary_config_value cfg;
		params.load( cfg );

		example_callback( reinterpret_cast< pcstr >( &params ) );
	}

	void use_game_core_weapon_dispersion_params( )
	{
		// Both ctors store constants; escape &obj through the opaque sink so
		// LTCG observes the stores (else they are dead-store-eliminated and the
		// ctor compiles empty - the #107 18% trap).
		configs::binary_config_value cfg;

		survarium::weapon_dispersion_params default_params;
		survarium::weapon_dispersion_params config_params( cfg );

		example_callback( reinterpret_cast< pcstr >( &default_params ) );
		example_callback( reinterpret_cast< pcstr >( &config_params ) );
	}

	void use_recoil_calculator( )
	{
		survarium::recoil_calculator calc;
		calc.get_horizontal_coeff( );
		calc.get_vertical_coeff( );
		calc.get_back_coeff( );

		calc.tick( survarium::type_stand, true, 10, 10.f );

		calc.set_weapon( NULL );

		calc.reload( );
		calc.chamber_a_round( );
		calc.fire( );
	}

	void use_game_material_manager( )
	{
		survarium::game_material_manager manager;
		manager.clear_resources( ); // game_world::clear_resources
	}

	void use_weapon_dispersion_calculator( )
	{
		survarium::weapon_dispersion_calculator calc;

		// The setters write members that get_value() does not read, so the
		// object must escape or LTCG dead-store-eliminates each store (the
		// same elision that empties the constant-only ctor - see README).
		// Take each setter's address to force its standalone body to be kept,
		// then escape &calc through the opaque example_callback so the stores
		// it performs are observed.
		typedef void ( survarium::weapon_dispersion_calculator::*setter_t )( const float );
		setter_t setters[ 3 ] =
		{
			&survarium::weapon_dispersion_calculator::set_reload_dispersion_amount,
			&survarium::weapon_dispersion_calculator::set_one_shoot_dispersion_amount,
			&survarium::weapon_dispersion_calculator::set_aiming_speed,
		};
		example_callback( reinterpret_cast< pcstr >( &setters ) );

		calc.set_reload_dispersion_amount( 10.0f );
		calc.set_one_shoot_dispersion_amount( 20.0f );
		calc.set_aiming_speed( 30.0f );
		calc.get_value( );

		example_callback( reinterpret_cast< pcstr >( &calc ) );
		calc.get_value( );
	}

	void use_game_core_weapon_core_base_state( )
	{
		// weapon_core_base_state is abstract (pure weapon_and_hands_expression) and its
		// ctor is protected; a concrete derived stub gives us a constructible instance.
		struct concrete_state : survarium::weapon_core_base_state
		{
			concrete_state( survarium::weapon_core& weapon )
				: survarium::weapon_core_base_state( weapon, false ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}

			// deserializing() is protected on the base; expose it through the
			// derived type so the anchor keeps the symbol alive.
			bool call_deserializing( ) const { return deserializing( ); }
		};

		survarium::weapon_core		weapon;
		concrete_state				state( weapon );

		// Real call keeps the out-of-line deserializing() symbol; escape the result
		// so it is observed.
		volatile bool d = state.call_deserializing( );
		example_callback( reinterpret_cast< pcstr >( const_cast< bool* >( &d ) ) );

		// Escape &state so LTCG observes the ctor's member stores (loop_performance.md).
		example_callback( reinterpret_cast< pcstr >( &state ) );

		// claude@NOTE: anchor the non-virtual recoil-time-calculator getters so the
		// linker keeps their out-of-line bodies for scoring.
		survarium::weapon_core::calculator_functor bc = weapon.backward_recoil_time_calculator( );
		survarium::weapon_core::calculator_functor hc = weapon.horizontal_recoil_time_calculator( );
		survarium::weapon_core::calculator_functor vc = weapon.vertical_recoil_time_calculator( );
		example_callback( reinterpret_cast< pcstr >( &bc ) );
		example_callback( reinterpret_cast< pcstr >( &hc ) );
		example_callback( reinterpret_cast< pcstr >( &vc ) );
	}

	void use_game_core_weapon_core_animation_end_aware_state( )
	{
		// weapon_core_animation_end_aware_state is abstract (pure
		// weapon_and_hands_expression from the base + pure on_animation_end_impl);
		// a concrete derived stub overriding both gives a constructible instance.
		struct concrete_aware_state : survarium::weapon_core_animation_end_aware_state
		{
			concrete_aware_state( survarium::weapon_core& weapon )
				: survarium::weapon_core_animation_end_aware_state( weapon, true ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}

			virtual void on_animation_end_impl( bool& ) override { }
		};

		survarium::weapon_core						weapon;
		concrete_aware_state						state( weapon );
		vostok::resources::managed_resource_ptr		anim;
		vostok::animation::animation_callback_params
			params( NULL, anim, NULL, 0, 0, 0, 0 );

		// Qualified, non-virtual calls keep the out-of-line symbols (not the stub
		// vtable slots) for initialize/finalize/set_animation_to_wait/on_animation_end.
		state.survarium::weapon_core_animation_end_aware_state::initialize( );
		state.survarium::weapon_core_animation_end_aware_state::finalize( );
		state.survarium::weapon_core_animation_end_aware_state::set_animation_to_wait( anim );
		state.survarium::weapon_core_animation_end_aware_state::on_animation_end( params );

		// Escape pointers so LTCG observes the stores.
		example_callback( reinterpret_cast< pcstr >( &state ) );
		example_callback( reinterpret_cast< pcstr >( &anim ) );
		example_callback( reinterpret_cast< pcstr >( &params ) );
	}

	void use_game_core_weapon_core_idle_state_base( )
	{
		// weapon_core_idle_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the vtable stores.
		struct concrete_idle_state : survarium::weapon_core_idle_state_base
		{
			concrete_idle_state( survarium::weapon_core& weapon )
				: survarium::weapon_core_idle_state_base( weapon ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core		weapon;
		concrete_idle_state			state( weapon );

		// Qualified, non-virtual calls keep the idle-base out-of-line
		// initialize()/finalize() symbols (not the stub's vtable slot).
		state.survarium::weapon_core_idle_state_base::initialize( );
		state.survarium::weapon_core_idle_state_base::finalize( );

		// Escape &state so LTCG observes the ctor's vtable stores.
		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_aimed_state_base( )
	{
		// weapon_core_aimed_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the vtable stores.
		struct concrete_aimed_state : survarium::weapon_core_aimed_state_base
		{
			concrete_aimed_state( survarium::weapon_core& weapon )
				: survarium::weapon_core_aimed_state_base( weapon ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core		weapon;
		concrete_aimed_state		state( weapon );

		// Qualified, non-virtual calls keep the aimed-base out-of-line
		// initialize()/finalize() symbols (not the stub's vtable slot).
		state.survarium::weapon_core_aimed_state_base::initialize( );
		state.survarium::weapon_core_aimed_state_base::finalize( );

		// Escape &state so LTCG observes the ctor's vtable stores.
		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_idle_state( )
	{
		// All of weapon_core_idle_state's ctor / weapon_and_hands_expression /
		// get_weapon_lexeme_pair and the cook's new_object are PRIVATE (target mangling
		// AAE/EBE/ABE); this anchor is befriended by both the state class and the cook
		// template, so it can reach them directly. The anchor never runs.
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 4 ];
		survarium::weapon_core_idle_state			state( weapon, animations, 4 );

		// Qualified, non-virtual calls keep the out-of-line private virtual /
		// private const bodies (not the vtable slot).
		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_idle_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_idle_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );

		// new_object is a private member of the cook template; reach it via a qualified
		// call on a fabricated null pointer so its body is ODR-used WITHOUT constructing
		// a cook (constructing would emit the cook vtable and force codegen of the still-
		// STUB create_resource/allocate_resource virtuals -> C4716). The anchor is
		// befriended on the template, so the private new_object is accessible.
		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_idle_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_idle_state*			object	= cook->cook_type::new_object( buffer, params, animations, 4 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_aimed_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 4 ];
		survarium::weapon_core_aimed_state			state( weapon, animations, 4 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_aimed_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_aimed_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_aimed_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_aimed_state*			object	= cook->cook_type::new_object( buffer, params, animations, 4 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_pistol_weapon_core_idle_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		survarium::pistol_weapon_core_idle_state	state( weapon, animations, 8 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_idle_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_idle_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_idle_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_idle_state*	object	= cook->cook_type::new_object( buffer, params, animations, 8 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_pistol_weapon_core_fire_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		survarium::pistol_weapon_core_fire_state	state( weapon, 1.0f, animations, 12 );

		state.survarium::pistol_weapon_core_fire_state::initialize( );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_fire_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_fire_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::pistol_weapon_core_fire_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_fire_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_fire_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
	}

	void use_game_core_pistol_weapon_core_show_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		bool										is_shown	= false;
		survarium::pistol_weapon_core_show_state	state( weapon, 1.0f, animations, 12, is_shown );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_show_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_show_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::pistol_weapon_core_show_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_show_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_show_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_hide_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		bool										is_shown	= false;
		survarium::weapon_core_hide_state			state( weapon, 1.0f, animations, 8, is_shown );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_hide_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_hide_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_hide_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_hide_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_hide_state*			object	= cook->cook_type::new_object( buffer, params, animations, 8 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_double_barreled_weapon_core_idle_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		survarium::double_barreled_weapon_core_idle_state	state( weapon, animations, 12 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_idle_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_idle_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_idle_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_idle_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_pistol_weapon_core_aimed_idle_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		survarium::pistol_weapon_core_aimed_idle_state	state( weapon, animations, 8 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_aimed_idle_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_aimed_idle_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_aimed_idle_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_aimed_idle_state*	object	= cook->cook_type::new_object( buffer, params, animations, 8 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_pistol_weapon_core_aimed_fire_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		survarium::pistol_weapon_core_aimed_fire_state	state( weapon, 1.0f, animations, 12 );

		state.survarium::pistol_weapon_core_aimed_fire_state::initialize( );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_aimed_fire_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::pistol_weapon_core_aimed_fire_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_aimed_fire_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_aimed_fire_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_double_barreled_weapon_core_aimed_idle_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		survarium::double_barreled_weapon_core_aimed_idle_state	state( weapon, animations, 12 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_aimed_idle_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_aimed_idle_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_aimed_idle_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_aimed_idle_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_show_state_base( )
	{
		// weapon_core_show_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the stores.
		struct concrete_show_state : survarium::weapon_core_show_state_base
		{
			concrete_show_state( survarium::weapon_core& weapon, bool& is_shown )
				: survarium::weapon_core_show_state_base( weapon, is_shown ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core		weapon;
		bool						is_shown	= false;
		concrete_show_state			state( weapon, is_shown );
		bool						tick_result	= false;

		// Qualified, non-virtual calls keep the show-base out-of-line
		// initialize()/finalize()/on_animation_end_impl() symbols.
		state.survarium::weapon_core_show_state_base::initialize( );
		state.survarium::weapon_core_show_state_base::finalize( );
		state.survarium::weapon_core_show_state_base::on_animation_end_impl( tick_result );

		// Escape &state / &is_shown so LTCG observes the stores.
		example_callback( reinterpret_cast< pcstr >( &state ) );
		example_callback( reinterpret_cast< pcstr >( &is_shown ) );
		example_callback( reinterpret_cast< pcstr >( &tick_result ) );
	}

	void use_game_core_weapon_core_hide_state_base( )
	{
		// weapon_core_hide_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the stores.
		struct concrete_hide_state : survarium::weapon_core_hide_state_base
		{
			concrete_hide_state( survarium::weapon_core& weapon, bool& is_shown )
				: survarium::weapon_core_hide_state_base( weapon, is_shown ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core		weapon;
		bool						is_shown	= false;
		concrete_hide_state			state( weapon, is_shown );
		bool						tick_result	= false;

		// Qualified, non-virtual calls keep the hide-base out-of-line
		// initialize()/finalize()/on_animation_end_impl() symbols.
		state.survarium::weapon_core_hide_state_base::initialize( );
		state.survarium::weapon_core_hide_state_base::finalize( );
		state.survarium::weapon_core_hide_state_base::on_animation_end_impl( tick_result );

		// Escape &state / &is_shown so LTCG observes the stores.
		example_callback( reinterpret_cast< pcstr >( &state ) );
		example_callback( reinterpret_cast< pcstr >( &is_shown ) );
		example_callback( reinterpret_cast< pcstr >( &tick_result ) );
	}

	void use_game_core_weapon_core_reload_state_base( )
	{
		// weapon_core_reload_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the stores.
		struct concrete_reload_state : survarium::weapon_core_reload_state_base
		{
			concrete_reload_state( survarium::weapon_core& weapon, float animation_time_scale )
				: survarium::weapon_core_reload_state_base( weapon, animation_time_scale ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core		weapon;
		concrete_reload_state		state( weapon, 1.0f );
		bool						tick_result	= false;

		state.survarium::weapon_core_reload_state_base::initialize( );
		state.survarium::weapon_core_reload_state_base::on_animation_end_impl( tick_result );

		example_callback( reinterpret_cast< pcstr >( &state ) );
		example_callback( reinterpret_cast< pcstr >( &tick_result ) );
	}

	void use_game_core_weapon_core_chamber_a_round_state_base( )
	{
		// weapon_core_chamber_a_round_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the stores.
		struct concrete_chamber_state : survarium::weapon_core_chamber_a_round_state_base
		{
			concrete_chamber_state( survarium::weapon_core& weapon, float animation_time_scale )
				: survarium::weapon_core_chamber_a_round_state_base( weapon, animation_time_scale ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core		weapon;
		concrete_chamber_state		state( weapon, 1.0f );
		bool						tick_result	= false;

		state.survarium::weapon_core_chamber_a_round_state_base::initialize( );
		state.survarium::weapon_core_chamber_a_round_state_base::on_animation_end_impl( tick_result );

		example_callback( reinterpret_cast< pcstr >( &state ) );
		example_callback( reinterpret_cast< pcstr >( &tick_result ) );
	}

	void use_game_core_weapon_core_fire_state_base( )
	{
		// weapon_core_fire_state_base does not override the pure
		// weapon_and_hands_expression, so it is still abstract; a concrete derived
		// stub gives a constructible instance whose ctor observes the stores.
		struct concrete_fire_state : survarium::weapon_core_fire_state_base
		{
			concrete_fire_state( survarium::weapon_core& weapon, float animation_timescale )
				: survarium::weapon_core_fire_state_base( weapon, animation_timescale ) {}

			virtual animation::mixing::expression weapon_and_hands_expression(
				mutable_buffer&,
				bool,
				survarium::weapon_user_state_enum,
				animation::mixing::animation_lexeme& ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_core						weapon;
		concrete_fire_state							state( weapon, 1.0f );
		vostok::resources::managed_resource_ptr		anim;
		vostok::animation::animation_callback_params
			params( NULL, anim, NULL, 0, 0, 0, 0 );
		bool										tick_result	= false;

		state.survarium::weapon_core_fire_state_base::initialize( );
		state.survarium::weapon_core_fire_state_base::execute( );
		state.survarium::weapon_core_fire_state_base::finalize( );
		state.survarium::weapon_core_fire_state_base::on_animation_end_impl( tick_result );
		state.survarium::weapon_core_fire_state_base::on_shot_event( params );

		example_callback( reinterpret_cast< pcstr >( &state ) );
		example_callback( reinterpret_cast< pcstr >( &anim ) );
		example_callback( reinterpret_cast< pcstr >( &params ) );
		example_callback( reinterpret_cast< pcstr >( &tick_result ) );
	}

	void use_game_core_weapon_core_shotgun_reload_state( )
	{
		// claude@NOTE: anchor the matched methods. We construct an instance (ctor + dtor
		// reachable) AND escape it through the opaque sink. weapon_and_hands_expression is
		// now matched (its real body calls the base_substate override, which is matched too),
		// so the qualified call anchors the whole reachable chain (shotgun_reload_state ->
		// weapon_core_shotgun_reload_base_substate::weapon_and_hands_expression ->
		// get_weapon_lexeme_pair -> get_weapon_lexeme_pair_impl).
		typedef survarium::weapon_core_shotgun_reload_state state_t;

		survarium::weapon_core	weapon;
		state_t					state( weapon, NULL, NULL, NULL );

		state.state_t::execute( );
		state.state_t::initialize( );
		state.state_t::finalize( );

		// Anchor weapon_and_hands_expression (and, transitively, the base_substate override +
		// get_weapon_lexeme_pair it calls) via a member-function pointer escaped through the
		// opaque sink - this keeps it without having to construct an animation_lexeme arg.
		vostok::animation::mixing::expression ( state_t::*waahe )(
			vostok::mutable_buffer&,
			bool,
			survarium::weapon_user_state_enum,
			vostok::animation::mixing::animation_lexeme&
		) const = &state_t::weapon_and_hands_expression;
		example_callback( reinterpret_cast< pcstr >( &waahe ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_bullet( )
	{
		survarium::bullet_manager			bullet_manager( NULL, NULL, NULL );
		survarium::weapon_ammunition_ptr	wa( NULL );
		survarium::weapon_core				wc;

		bullet_manager.fire( float3(), float3(), wa, wc, 10, NULL, NULL, true );
	}

	void use_game_core_weapon_core_initialize_weapon_logic( )
	{
		survarium::weapon_core				wc;
		survarium::weapon_core_base_state_ptr	s( NULL );

		wc.initialize_weapon_logic( s, s, s, s, s, s, s, s, s, s );

		// Anchor weapon_core's public/private members defined in weapon_core.cpp so
		// they are emitted and scored (this fn is friended; reaches privates).
		volatile bool b = wc.is_ready_to_shoot( );
		b = wc.maximum_ammo_in_weapon( ) != 0;
		survarium::weapon_ammunition_ptr		ammo( NULL );
		wc.set_ammunition( ammo );
		wc.unload_chambered_round( );
		vostok::resources::managed_resource_ptr	anim;
		vostok::animation::animation_callback_params
			params( NULL, anim, NULL, 0, 0, 0, 0 );
		b = wc.on_hand_ik_event( params, survarium::hand_to_weapon_ik_processor::left ) == 0;

		example_callback( reinterpret_cast< pcstr >( const_cast< bool* >( &b ) ) );
		example_callback( reinterpret_cast< pcstr >( &wc ) );
	}

	// claude@NOTE: anchor for the small weapon_core setters/accessors batch.
	void use_game_core_weapon_core_small_setters( )
	{
		survarium::weapon_core				wc;
		vostok::animation::skeleton_ptr	skeleton( NULL );

		wc.set_magazine_capacity( 0 );
		wc.set_skeleton( skeleton );
		wc.get_body_part_mask_for_user( );
		wc.update_recoil( 0, 0.0f );
		wc.update_breath_vibration( false, 0, 0.0f );
		wc.instant_show( );
		wc.instant_hide( );
	}


	void use_inventory( )
	{
	/*
		survarium::medkit item;

		survarium::inventory_item_props props;
		item.get_item_props( props );

		item.remove( );

		item.reduce_damage( NULL, NULL, 0.0f, 0.0f );
	*/
	}

	void use_damage_model_cook( )
	{
		survarium::damage_model_cook cook;
		cook.delete_resource( NULL );
	}

	void use_ladder( survarium::ladder* ladder )
	{
		ladder->activate( NULL );
		ladder->deactivate( );

		ladder->add_landing_point( NULL );
		ladder->pop_landing_point( );
	}

	void use_game_core_affects_threshold()
	{
		survarium::affects_threshold( 10.0f, 10, NULL );
	}

	void use_game_core_player_stamina()
	{
		survarium::player_stamina stamina;

		configs::binary_config_value cfg;
		stamina.load( cfg );

		stamina.reset( );
		stamina.set_regeneration_speed( 10.0f );
		stamina.set_regeneration_speed_factor( 10.0f );
		stamina.tick( 10, true );
		stamina.spend( 10.0f );
		stamina.can_be_spent();
		stamina.subscribe_on_depletion( NULL );
		stamina.unsubscribe_from_depletion( NULL );

		survarium::player_stamina stamina2( stamina );
	}


	void use_game_core_player_stealth()
	{
		survarium::player_stealth stealth;

		// load() reads config into the members (keeps the load body reachable)
		configs::binary_config_value cfg;
		stealth.load( cfg );

		// copy ctor (delegates to operator=) + direct operator= call
		survarium::player_stealth stealth2( stealth );
		stealth2 = stealth;

		// escape both objects so LTCG keeps the member stores observable
		example_callback( reinterpret_cast< pcstr >( &stealth ) );
		example_callback( reinterpret_cast< pcstr >( &stealth2 ) );
	}


	void use_game_core_player_input()
	{
		survarium::player_input input;
		input.is_sprinting();
		input.is_empty();
	}

	void use_client_player_update( network_core::udp_match_packet* packet )
	{
		survarium::client_player_update update;
		update.serialize( *packet );
	}

	void use_game_core_weapon_state()
	{
		// Anchor weapon_state::operator= so its member stores are OBSERVED
		// (escape both objects through the opaque sink) and LTCG cannot DSE them.
		survarium::weapon_state a;
		survarium::weapon_state b;

		b = a;

		example_callback( reinterpret_cast< pcstr >( &a ) );
		example_callback( reinterpret_cast< pcstr >( &b ) );
	}

	void use_game_core_player_logic_base_state()
	{
		survarium::player_input input;
		// Escape the computed index so LTCG does not elide the static body.
		u32 index = survarium::player_logic_base_state::movement_animation_index( input );
		example_callback( reinterpret_cast< pcstr >( &index ) );

		// player_logic_base_state is abstract (pure selected_animations); a concrete
		// derived stub gives a constructible instance whose ctor observes the member
		// stores. The anchor never runs, so fabricated references are fine.
		struct concrete_logic_state : survarium::player_logic_base_state
		{
			concrete_logic_state( survarium::weapon_user_animations_selector& owner )
				: survarium::player_logic_base_state( owner, survarium::type_stand ) {}

			// player_logic_base_state leaves fsm_state's initialize/execute/finalize
			// pure; override them all so the stub is concrete.
			virtual void initialize( ) override { }
			virtual void execute( ) override { }
			virtual void finalize( ) override { }

			virtual std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
				selected_animations(
					mutable_buffer&,
					survarium::weapon_animation_parameters const&,
					bool ) const override
			{
				VOSTOK_UNREACHABLE_CODE( );
			}
		};

		survarium::weapon_user_animations_selector&	owner	= *reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );
		survarium::base_player&						user	= *reinterpret_cast< survarium::base_player* >( NULL );

		concrete_logic_state	state( owner );
		state.survarium::player_logic_base_state::set_user( user );
		// claude@NOTE: is_ready_for_transition is `return true;` which /OPT:ICF folds
		// to the `mov al,1;ret` fold @0x12700; unscorable None but byte-correct,
		// marked None|DONE (a folded body cannot be made scorable by anchoring).

		// Escape &state so LTCG observes the ctor's member stores.
		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	// claude@MATCH: anchor for player_logic_jump_state (jump_state unit). The anchor
	// never runs, so fabricated null refs are fine; calling each override keeps the
	// COMDAT and lets objdiff pair them.
	void use_game_core_player_logic_jump_state()
	{
		survarium::weapon_user_animations_selector&	owner	= *reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );
		survarium::base_player&						user	= *reinterpret_cast< survarium::base_player* >( NULL );

		survarium::player_logic_jump_state	state( owner );

		// overrides are private in jump_state; reach them virtually through the
		// base interfaces where they are public/protected.
		survarium::player_logic_base_state&	base	= state;
		vostok::ai::fsm_state&				fsm		= state;

		base.set_user( user );
		fsm.initialize( );
		fsm.execute( );
		fsm.finalize( );

		bool ready = base.is_ready_for_transition( );
		example_callback( reinterpret_cast< pcstr >( &ready ) );

		// claude@TODO: selected_animations is INPROGRESS (blocked on jump_logic::
		// selected_animations stub); not anchored until that callee returns a value.

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}


	void use_game_core_jump_logic_state_inactive( )
	{
		// initialize()/is_ready_for_transition() are header inline overrides. Take
		// their member-fn addresses to ODR-use them and force a standalone
		// (un-inlined) out-of-line body for each, then escape the pointers so the
		// uses are observed. Do NOT construct an instance: instantiating the class
		// would emit its vtable and force codegen of the still-STUB
		// selected_animations (no return -> C4716/LNK1257). Address-of touches only
		// these two members, which is all this unit needs.
		void ( survarium::jump_logic_state_inactive::*init )( )        = &survarium::jump_logic_state_inactive::initialize;
		bool ( survarium::jump_logic_state_inactive::*ready )( ) const = &survarium::jump_logic_state_inactive::is_ready_for_transition;
		example_callback( reinterpret_cast< pcstr >( &init ) );
		example_callback( reinterpret_cast< pcstr >( &ready ) );

		// claude@MATCH: selected_animations now has a (returning) body, so a qualified
		// devirtualized call ODR-uses its out-of-line body without emitting the vtable.
		survarium::jump_logic_state_inactive& s = *reinterpret_cast< survarium::jump_logic_state_inactive* >( NULL );
		survarium::jump_logic_base_state::animation_delegate const& d =
			*reinterpret_cast< survarium::jump_logic_base_state::animation_delegate const* >( NULL );
		vostok::mutable_buffer&                    buf = *reinterpret_cast< vostok::mutable_buffer* >( NULL );
		survarium::weapon_animation_parameters const& wp =
			*reinterpret_cast< survarium::weapon_animation_parameters const* >( NULL );
		std::pair< vostok::animation::mixing::expression, vostok::animation::mixing::animation_lexeme > p =
			s.survarium::jump_logic_state_inactive::selected_animations( buf, false, d, wp );
		example_callback( reinterpret_cast< pcstr >( &p ) );
	}

	void use_game_core_jump_logic_state_landing( )
	{
		// selected_animations now returns (UNREACHABLE), so the class can be instantiated
		// without the #148 C4716 trap. Construct on a fabricated owner (never runs): this
		// emits the ctor + the full vtable (all virtual overrides codegen'd). Then call the
		// private non-virtual helpers (anchor befriended) so their bodies are emitted too.
		survarium::jump_logic&					owner	= *reinterpret_cast< survarium::jump_logic* >( NULL );
		survarium::jump_logic_state_landing		s( owner );
		s.survarium::jump_logic_state_landing::execute( );
		bool r = s.survarium::jump_logic_state_landing::is_ready_for_transition( );
		example_callback( reinterpret_cast< pcstr >( &r ) );

		vostok::mutable_buffer&					buf	= *reinterpret_cast< vostok::mutable_buffer* >( NULL );
		vostok::animation::animation_callback_params& cbp =
			*reinterpret_cast< vostok::animation::animation_callback_params* >( NULL );
		survarium::jump_logic_base_state::animation_delegate const& d =
			*reinterpret_cast< survarium::jump_logic_base_state::animation_delegate const* >( NULL );
		vostok::animation::mixing::animation_lexeme& lx =
			*reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		vostok::animation::mixing::animation_lexeme m = s.get_main_lexeme( buf, false, vostok::animation::body_part_whole_body );
		vostok::animation::mixing::animation_lexeme l = s.get_look_lexeme( buf, false, d, lx );
		vostok::animation::callback_return_type_enum ie = s.on_interval_end( cbp );
		example_callback( reinterpret_cast< pcstr >( &ie ) );
		example_callback( reinterpret_cast< pcstr >( &s ) );
	}

	void use_game_core_jump_logic_state_start( )
	{
		// Construct (see landing): emits ctor + vtable; befriended anchor calls the private
		// non-virtual helpers so their bodies are emitted.
		survarium::jump_logic&					owner	= *reinterpret_cast< survarium::jump_logic* >( NULL );
		survarium::jump_logic_state_start		s( owner );
		s.survarium::jump_logic_state_start::execute( );
		bool r = s.survarium::jump_logic_state_start::is_ready_for_transition( );
		example_callback( reinterpret_cast< pcstr >( &r ) );

		vostok::mutable_buffer&					buf	= *reinterpret_cast< vostok::mutable_buffer* >( NULL );
		vostok::animation::animation_callback_params& cbp =
			*reinterpret_cast< vostok::animation::animation_callback_params* >( NULL );
		survarium::jump_logic_base_state::animation_delegate const& d =
			*reinterpret_cast< survarium::jump_logic_base_state::animation_delegate const* >( NULL );
		vostok::animation::mixing::animation_lexeme& lx =
			*reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		vostok::animation::mixing::animation_lexeme m = s.get_main_lexeme( buf, false, vostok::animation::body_part_whole_body );
		vostok::animation::mixing::animation_lexeme l = s.get_look_lexeme( buf, false, d, lx );
		vostok::animation::callback_return_type_enum ie = s.on_interval_end( cbp );
		vostok::animation::callback_return_type_enum je = s.on_jump_event( cbp );
		example_callback( reinterpret_cast< pcstr >( &ie ) );
		example_callback( reinterpret_cast< pcstr >( &je ) );
		example_callback( reinterpret_cast< pcstr >( &s ) );
	}

	// claude@MATCH: anchor for jump_logic non-virtual leaf methods (landing_predicate,
	// tick, look_time_factor, is_jump_finished). Qualified calls on a fabricated
	// reference ODR-use the bodies without constructing an instance (the dtor/vtable
	// of derived states stays untouched). The anchor never runs.
	void use_game_core_jump_logic( )
	{
		survarium::jump_logic&	jl	= *reinterpret_cast< survarium::jump_logic* >( NULL );
		bool	lp	= jl.landing_predicate( );
		jl.tick( );
		float	lt	= jl.look_time_factor( );
		bool	jf	= jl.is_jump_finished( );
		example_callback( reinterpret_cast< pcstr >( &lp ) );
		example_callback( reinterpret_cast< pcstr >( &lt ) );
		example_callback( reinterpret_cast< pcstr >( &jf ) );

		// ODR-use the out-of-line jump_logic method bodies via member-fn pointers so
		// the linker keeps them (no instance constructed -> no vtable/STUB codegen).
		std::pair< vostok::animation::mixing::expression, vostok::animation::mixing::animation_lexeme >
			( survarium::jump_logic::*sel )( vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, bool ) const
				= &survarium::jump_logic::selected_animations;
		vostok::resources::managed_resource_ptr ( survarium::jump_logic::*get_anim )( survarium::jump_animation_parts, bool ) const
			= &survarium::jump_logic::get_animation;
		void ( survarium::jump_logic::*act )( ) = &survarium::jump_logic::activate;
		example_callback( reinterpret_cast< pcstr >( &sel ) );
		example_callback( reinterpret_cast< pcstr >( &get_anim ) );
		example_callback( reinterpret_cast< pcstr >( &act ) );

		// claude@NOTE: anchor the matched free fns + non-virtual members only. Do NOT
		// construct a jump_logic: its ctor calls initialize_logic, which builds the
		// jump_logic_state_* subclasses and force-codegens their STUB selected_animations
		// (no return) -> C4716/LNK1257. So ctor/dtor/initialize_logic stay BLOCKED here.
		survarium::player_input const& input = *reinterpret_cast< survarium::player_input const* >( NULL );
		survarium::move_direction_enum d = survarium::get_move_direction( input );
		example_callback( reinterpret_cast< pcstr >( &d ) );

		u32 idx = survarium::get_jump_animation_index( d, true, survarium::jump_animations_part_start );
		example_callback( reinterpret_cast< pcstr >( &idx ) );

		// Address-of non-virtual members to ODR-use their bodies WITHOUT constructing a
		// jump_logic (which would emit the vtable and force still-STUB members to codegen).
		void ( survarium::jump_logic::*su )( survarium::base_player& ) = &survarium::jump_logic::set_user;
		void ( survarium::jump_logic::*de )( )                         = &survarium::jump_logic::deactivate;
		bool ( survarium::jump_logic::*dn )( ) const                   = &survarium::jump_logic::does_need_land_and_run;
		example_callback( reinterpret_cast< pcstr >( &su ) );
		example_callback( reinterpret_cast< pcstr >( &de ) );
		example_callback( reinterpret_cast< pcstr >( &dn ) );
	}

	struct ghost_predicate : physics::contact_test_predicate {
	virtual	float		add_single_result		(
							void*				arg_0,
							collision::primitive_type		arg_1,
							float4x4 const&		arg_2,
							float3 const&		arg_3,
							collision::primitive_type		arg_4,
							float4x4 const&		arg_5,
							float3 const&		arg_6
						) override { return 0.0;}
	};


	void use_game_core_collision_sensor()
	{
		survarium::collision_sensor sensor;
		configs::binary_config_value cfg;
		sensor.load( cfg );
		sensor.resolve_links( NULL, cfg );
		sensor.tick( 0, 0 );
		sensor.is_filter_passed( NULL );

		ghost_predicate gp;
		sensor.contact_test( NULL, gp );
		sensor.contact_test( NULL );
		sensor.insert( NULL );
		sensor.remove( );
		sensor.get_collision_geometry( 10 );
		// claude@NOTE: the four protected on_* overrides are trivial empty bodies
		// that /OPT:ICF-fold (on_inside/on_leave/on_objetcs_loosed -> `ret 4`
		// @0x12c50, on_enter -> @0xd2070); no anchor can make a folded body scorable,
		// so they are proven byte-correct and marked None|DONE, not anchored here.
	}


	void use_game_core_collision_geometry()
	{
		survarium::collision_geometry gm;
		gm.destroy_ghost_object( );

		configs::binary_config_value cfg;
		gm.load( cfg );

		gm.get_overlapping_objects_count( );

		physics::base_physics_objects_type physics_objects( NULL, 10 );
		gm.get_overlapping_objects( physics_objects );

		ghost_predicate predicate;
		gm.contact_test( NULL, predicate );
		gm.contact_test( );

		vostok::vectora<float3> centers_results( NULL );
		gm.get_shapes_centers( centers_results );

		gm.subscribe( NULL, NULL );
		gm.unsubscribe( NULL );

		gm.set_transform( float4x4() );
		gm.get_transform( );
		// claude@NOTE: cast_to_collision_geometry is `return this;` which /OPT:ICF
		// folds to the empty-frame fold @0x17600; unscorable None but byte-correct,
		// marked None|DONE (a folded body cannot be made scorable by anchoring).
	}

	void use_game_core_scheduler()
	{
		survarium::scheduler sc( NULL );
		survarium::scheduler::callback callback;
		sc.register_on_frame( NULL, callback, true );
		sc.register_for_update( NULL, callback, true, 10, 10, 10 );
		sc.on_frame( 10, 10 );
		sc.unregister( NULL );
	}

	// claude@NOTE: interactive_object::assign_game_ui (`ret 4` empty @0x12c50) and
	// cast_weapon_core (`xor eax,eax;ret` @0x327c0, const + non-const) are trivial
	// overrides that /OPT:ICF folds; unscorable None but byte-correct, marked
	// None|DONE (a folded body cannot be made scorable by anchoring).

	// inventory_holder is abstract; reach the three non-virtual getters via a trivial
	// concrete derived stub (overrides every pure virtual). Qualified calls keep the
	// standalone getter bodies; escape the results.
	struct concrete_inventory_holder : survarium::inventory_holder
	{
		concrete_inventory_holder( survarium::scheduler& s, survarium::inventory_ptr inv )
			: survarium::inventory_holder( s, inv ) { }

		virtual bool					set_new_active_item	( survarium::inventory_item_ptr const& ) override { return false; }
		virtual void					take_inventory_item	( survarium::inventory_item_ptr const& ) override { }
		virtual survarium::damage_model_ptr const&	damage_model( ) const override { return *reinterpret_cast< survarium::damage_model_ptr* >( NULL ); }
		virtual survarium::base_player*	cast_to_base_player	( ) override { return NULL; }
		virtual physics::world*			get_physics_world	( ) override { return NULL; }
		virtual void					insert_game_world_object( survarium::game_world_object& ) override { }
		virtual void					remove_game_world_object( survarium::game_world_object& ) override { }

		void touch( )
		{
			survarium::inventory const&	ci	= inventory_holder::inventory( );
			survarium::inventory&		mi	= inventory_holder::inventory( );
			survarium::scheduler&		sc	= inventory_holder::scheduler( );
			example_callback( reinterpret_cast< pcstr >( &ci ) );
			example_callback( reinterpret_cast< pcstr >( &mi ) );
			example_callback( reinterpret_cast< pcstr >( &sc ) );
		}
	};

	void use_game_core_inventory_holder( )
	{
		survarium::scheduler&	sched	= *reinterpret_cast< survarium::scheduler* >( NULL );
		concrete_inventory_holder	holder( sched, survarium::inventory_ptr( NULL ) );
		holder.touch( );
	}

	void use_game_core_weapon_user_animations_selector( )
	{
		survarium::weapon_user_animations_selector&	sel	= *reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );
		sel.set_animations( survarium::weapon_user_animations_container_ptr( NULL ) );
		example_callback( reinterpret_cast< pcstr >( &sel ) );

		// claude@MATCH: keep the state accessors as standalone COMDATs via member-fn
		// pointers (a direct call would LTCG-inline them into this anchor, emitting no
		// body). current_state is private but is reached transitively from these three.
		typedef survarium::weapon_user_animations_selector self;
		survarium::weapon_user_state_enum	( self::*p0 )( ) const = &self::get_current_state_id;
		bool								( self::*p1 )( ) const = &self::is_in_jump;
		bool								( self::*p2 )( ) const = &self::is_sprinting;
		bool								( self::*p3 )( ) const = &self::is_ready_to_be_deactivated;
		example_callback( reinterpret_cast< pcstr >( &p0 ) );
		example_callback( reinterpret_cast< pcstr >( &p1 ) );
		example_callback( reinterpret_cast< pcstr >( &p2 ) );
		example_callback( reinterpret_cast< pcstr >( &p3 ) );

		// claude@MATCH: keep tick/deactivate/selected_animations as standalone COMDATs.
		void					( self::*p4 )( )												= &self::tick;
		void					( self::*p5 )( )												= &self::deactivate;
		std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >
								( self::*p6 )( mutable_buffer&, survarium::weapon_animation_parameters const&, bool ) const = &self::selected_animations;
		example_callback( reinterpret_cast< pcstr >( &p4 ) );
		example_callback( reinterpret_cast< pcstr >( &p5 ) );
		example_callback( reinterpret_cast< pcstr >( &p6 ) );
	}

	// base_project: register_named_object / register_object_to_resolve are public
	// non-virtual. base_project has a pure-ish vtable (get_object_by_name/resolve_links
	// are non-pure); reach the two registrars via a trivial concrete derived stub.
	struct concrete_base_project : survarium::base_project
	{
		void touch( )
		{
			register_named_object( "name", NULL );
			register_object_to_resolve( NULL, configs::binary_config_value( ) );
		}
	};

	void use_game_core_base_project( )
	{
		concrete_base_project	p;
		p.touch( );
		example_callback( reinterpret_cast< pcstr >( &p ) );
	}

	void use_game_core_weapon_user_animations_container_cook( )
	{
		// Constructing + registering keeps the vtable (translate_query, delete_resource).
		static survarium::weapon_user_animations_container_cook	s_cook;
		vostok::resources::register_cook( &s_cook );
		example_callback( reinterpret_cast< pcstr >( &s_cook ) );

		// claude@NOTE: create_requests_for_animations (the .cpp-local free helper) and the
		// private on_config_loaded/on_animations_loaded callbacks read None: taking the free
		// function's address through a fn-pointer does NOT force EXE emission under LTCG (the
		// pointer is provably dead), and the on_* callbacks are only reachable through the
		// still-stubbed translate_query/on_config_loaded boost::bind chain. They will be kept
		// (and scorable) once translate_query/on_config_loaded are matched with real bodies.
	}

	// booby_trap_core::get_speed is a PRIVATE virtual; befriended above so a
	// qualified (devirtualized) call on a null ref ODR-uses its standalone body
	// (`fldz; ret`) without emitting a vtable / codegen-ing the other stubs.
	void use_game_core_booby_trap_core_get_speed( )
	{
		survarium::booby_trap_core&	trap	= *reinterpret_cast< survarium::booby_trap_core* >( NULL );
		float speed = trap.survarium::booby_trap_core::get_speed( );
		example_callback( reinterpret_cast< pcstr >( &speed ) );
	}

	void use_physics_api()
	{
		physics::engine engine;
		physics::create_world_bt( NULL, engine );
		physics::destroy_world( NULL, NULL );
		physics::set_memory_allocator( NULL );
	}

	void use_log()
	{
		LOG_WARNING("ERROR %d", 10);
		logging::append(
			logging::log_callback_boost( core::g_log_callback ),
			(void*)0,
			logging::format_specifier( logging::format_specifier_time_brief ),
			"file",
			41,
			"use_log",
			"game_core:",
			logging::warning,
			"%s",
			"Hello!"
		);

		logging::log_format lf;

		logging::append(
			logging::log_callback_boost( core::g_log_callback ),
			(void*)0,
			&lf,
			"file",
			41,
			"use_log",
			"game_core:",
			logging::warning,
			"%s",
			"Hello!"
		);

	}

	void use_bt_character_controller()
	{
		memory::stack_allocator stack_allocator;
		physics::bt_character_controller* cc2 = physics::create_character_controller( stack_allocator, NULL );
		(void)cc2;
		physics::bt_character_controller cc(NULL);
		cc.allocator();
		cc.initialize();

		float4x4 t;
		cc.activate(t);
		cc.deactivate();
		cc.get_transform();
		cc.set_transform(t);
		cc.set_walk_direction(float3());
		cc.has_updates();
		cc.jump();
		cc.end_jump();
		cc.adjust_foot_transform( float3(), float3(), float3(), 1.0, 1.0, t);
		cc.update_action(10);
		cc.can_jump();
		cc.on_ground();
		cc.set_crouch(true);
		cc.can_crouch();
		cc.can_stand();
	}

	void use_bullet_character_controller()
	{
		physics::bullet_character_controller controller( NULL, float2(), float2(), 10, 20 );
		controller.set_transform( btTransform( ) );
		controller.set_crouch( true );
		controller.can_stand( );
		controller.get_transform( );
		controller.on_ground( );
		controller.can_jump( );
		controller.end_jump( );
		controller.jump( );
		controller.set_desired_walk_vector( btVector3( ) );
		// controller.setup_shape_dim( float2( ) );
		controller.insert( NULL );
		controller.remove( NULL );
		controller.updateAction( NULL, 10.0 );


		physics::computeReflectionDirection( btVector3( ), btVector3( ) );
		physics::perpindicularComponent( btVector3( ), btVector3( ) );
		physics::parallelComponent( btVector3( ), btVector3( ) );
		physics::setup_game_material_groups( NULL, 10 );
	}

	void example_callback(const char *name)
	{
		printf("%s\n", name);
	}

	void use_network_core_tcp_packet()
	{
		memory::stack_allocator stack_allocator;
		network_core::tcp_packet packet( stack_allocator );

		network_core::buffer_to_send( packet );
		network_core::buffer_to_receive_into( packet );
	}


	void use_network_core_http_client()
	{
		boost::asio::io_service io_service( 10 );
		network_core::http_client http_client( io_service );
		http_client.set_on_error( boost::bind( use_network_core_http_client ) );
		http_client.get( "server", "path", boost::bind(&example_callback, "hello" ) );

		boost::asio::streambuf buff;
		network_core::read_lines_from_stream( "prefix", buff );
	}

	void use_static_rigid_body()
	{
		physics::bt_collision_shape_ptr shape(NULL);

		physics::bt_static_rigid_body body( shape, NULL );
		body.get_rigid_body( );
		body.get_triangle_material( 10, true );
		body.apply_impulse( float3(), float3() );
		body.set_transform( float4x4() );
		body.get_transform( );
		body.get_collision_group( );
		body.get_bt_collision_obect( );

		physics::create_static_rigid_body( physics::bt_rigid_body_construction_info( ) );
		physics::destroy_static_rigid_body( NULL );
	}

	void use_collision_shape()
	{
		physics::bt_collision_shape shape(NULL);
		shape.get_triangle_material( 10, true );

		configs::binary_config_value bcv = configs::binary_config_value();

		physics::destroy_bt_shape		( NULL );
		physics::destroy_shape			( NULL );
		physics::create_bt_primitive	( collision::primitive_box, float3(), float3() );
		physics::create_primitive_shape	( collision::primitive_box, float3(), float3() );
		physics::create_compound_shape	( bcv, float3(), "model_path" );

		physics::geometry_resource_ptr resource_ptr	( NULL );
		physics::create_btBvhTriangleMeshShape		( NULL, NULL, 10, 10, NULL, float3(), resource_ptr, resource_ptr );
		physics::create_static_triangle_mesh_shape	( NULL, NULL, 10, 10, NULL, float3(), resource_ptr, resource_ptr );
	}

	void use_loose_oct_tree()
	{
		collision::loose_oct_tree tree(NULL, 100., 10);
		tree.unmove_all();
		tree.get_aabb();
	}

	void use_animated_object()
	{

		configs::binary_config_value bcv = configs::binary_config_value();
		animation::skeleton_ptr ms = animation::skeleton_ptr();
		memory::stack_allocator stack_allocator;

		collision::animated_object ao = collision::animated_object(bcv, ms, 10, stack_allocator);
		ao.update(NULL, NULL);
		ao.destroy(NULL);
		ao.get_bones_count();
		ao.get_aabb();
		ao.get_geometry();

		render::scene_ptr scene = vostok::render::scene_ptr();
		memory::managed_allocator alloc = memory::managed_allocator(100, 100);
		render::one_way_render_channel channel = render::one_way_render_channel(alloc);
		render::engine::world world = render::engine::world();
		render::debug::renderer renderer = render::debug::renderer(channel, alloc, world);


		float4x4 transform;
		ao.draw_collision(scene, renderer, transform);

		ao.get_random_surface_point(10);
		ao.get_head_bone_center();
		ao.get_eyes_direction();
		ao.body_part_name(10);
	}

	void use_aabb_object()
	{
		collision::new_aabb_object( NULL, 20, float3( 1.f, 1.f, 1.f ) );
	}

	void use_animated_rigid_body()
	{
		{
			physics::bt_animated_rigid_body arb = physics::bt_animated_rigid_body( NULL, NULL, 10 );
			float4x4 matrix;

			arb.center_of_mass_offset();
			arb.update_bone_matrix(10, matrix, true);
			arb.get_aabb();
			arb.get_bone_transform(10);
		}

		{
			// btCompoundShape* new_compound_shape_from_hit_targets_config( configs::binary_config_value const& config, geometries_type& geometries_data, memory::base_allocator* allocator )
			configs::binary_config_value config = configs::binary_config_value();
			physics::geometries_type geometries( NULL, 10 );
			physics::new_compound_shape_from_hit_targets_config( config, geometries, NULL );

			physics::calculate_bt_animated_body_size_from_hit_targets_config( config );
		}

		{
			configs::binary_config_value config = configs::binary_config_value();
			animation::skeleton_ptr skeleton_ptr( NULL );
			// physics::calculate_bt_hit_target_size( config );

			// calculate_bt_animated_body_size_from_hit_targets_config
			physics::calculate_bt_animated_body_size_from_hit_targets_config( config );
			physics::new_animated_bt_hit_model( config, skeleton_ptr, NULL );
		}

		{
			// sushi@NOTE: Called from animated_object::animated_object there game_material_id is set to 10 and linker hardcoded it here
			// bt_animated_rigid_body*	new_animated_rigid_body		( btCompoundShape* shape, u16 game_material_id, memory::base_allocator* allocator );
			physics::new_animated_rigid_body ( NULL, 10, NULL );
		}
		{
			// void						destroy_animated_rigid_body	( bt_animated_rigid_body* body, memory::base_allocator* allocator );
			physics::destroy_animated_rigid_body( NULL, NULL );
		}

	}

}


namespace survarium
{

void bpp_dump_stats(u32, float, float, pcstr) { }


IncludeAll::IncludeAll()
{
	//
	//
	//
	vostok::use_game_core_initialize( );
	vostok::use_game_core_breath_vibration_calculator( );
	vostok::use_game_core_legs_ik_processor( );
	vostok::use_game_core_legs_ik_drawer( );
	vostok::use_game_core_ik_processor( NULL, NULL, NULL );
	vostok::use_medkit( );
	vostok::use_inventory_2( );
	vostok::use_victory_items_container_core( NULL );
	vostok::use_booby_trap_cook( );
	vostok::use_hittable_object( NULL );
	vostok::use_respawn_point_core( );
	vostok::use_damage_zone_core( );
	vostok::use_generic_anomaly_core( );
	vostok::use_artefact_container_core( );
	vostok::use_artefact_lifebone_core( );
	vostok::use_victory_item_core( );
	vostok::use_weapon_core_cook( );
	vostok::use_weapon_core_shotgun_reload_state_cook( );
	vostok::use_weapon_core_inactive_state_cook( );
	vostok::use_game_core_weapon_recoil_params( );
	vostok::use_game_core_character_dispersion_params( );
	vostok::use_game_core_weapon_dispersion_params( );
	vostok::use_recoil_calculator( );
	vostok::use_dispersion_calculator( );
	vostok::use_character_dispersion_calculator( );
	vostok::use_game_material_manager( );
	vostok::use_weapon_dispersion_calculator( );
	vostok::use_game_core_weapon_core_base_state( );
	vostok::use_game_core_weapon_core_animation_end_aware_state( );
	vostok::use_game_core_weapon_core_idle_state_base( );
	vostok::use_game_core_weapon_core_aimed_state_base( );
	vostok::use_game_core_weapon_core_idle_state( );
	vostok::use_game_core_weapon_core_aimed_state( );
	vostok::use_game_core_pistol_weapon_core_idle_state( );
	vostok::use_game_core_pistol_weapon_core_show_state( );
	vostok::use_game_core_weapon_core_hide_state( );
	vostok::use_game_core_double_barreled_weapon_core_idle_state( );
	vostok::use_game_core_pistol_weapon_core_aimed_idle_state( );
	vostok::use_game_core_pistol_weapon_core_aimed_fire_state( );
	vostok::use_game_core_double_barreled_weapon_core_aimed_idle_state( );
	vostok::use_game_core_weapon_core_show_state_base( );
	vostok::use_game_core_weapon_core_hide_state_base( );
	vostok::use_game_core_weapon_core_reload_state_base( );
	vostok::use_game_core_weapon_core_chamber_a_round_state_base( );
	vostok::use_game_core_weapon_core_fire_state_base( );
	vostok::use_game_core_weapon_core_shotgun_reload_state( );
	vostok::use_game_core_pistol_weapon_core_fire_state( );
	vostok::use_game_core_weapon_core_initialize_weapon_logic( );
	vostok::use_game_core_weapon_core_small_setters( );
	vostok::use_bullet( );
	vostok::use_inventory( );
	vostok::use_damage_model_cook( );
	vostok::use_ladder( NULL );
	vostok::use_game_core_affects_threshold();
	vostok::use_game_core_player_stamina();
	vostok::use_game_core_player_stealth();
	vostok::use_game_core_player_input();
	vostok::use_client_player_update( NULL );
	vostok::use_game_core_weapon_state();
	vostok::use_game_core_player_logic_base_state();
	vostok::use_game_core_player_logic_jump_state();
	vostok::use_game_core_jump_logic_state_inactive();
	vostok::use_game_core_jump_logic_state_landing();
	vostok::use_game_core_jump_logic_state_start();
	vostok::use_game_core_jump_logic();
	vostok::use_game_core_collision_sensor();
	vostok::use_game_core_collision_geometry();
	vostok::use_game_core_scheduler();
	vostok::use_game_core_inventory_holder();
	vostok::use_game_core_weapon_user_animations_selector();
	vostok::use_game_core_weapon_user_animations_container_cook();
	vostok::use_game_core_base_project();
	vostok::use_game_core_booby_trap_core_get_speed();
	vostok::use_bt_character_controller();
	vostok::use_physics_api();
	vostok::use_log();
	vostok::use_network_core_http_client();
	vostok::use_static_rigid_body();
	vostok::use_animated_object();
	vostok::use_animated_rigid_body();
	vostok::use_aabb_object();
	vostok::use_collision_shape();
	vostok::use_bullet_character_controller();

	//
	// YEEET
	//
	hit_type_parameters* htp = new hit_type_parameters("hand", 1., 1., 1., 0);
	htp->apply_damage(10., 100);
	htp->set_parameters(10., 20., 30.);

	booster_damage_protector* bdp = new booster_damage_protector("hand", 0.5, 1.);
	bdp->reduce_damage("__whatever", "hand", 100, 10);

	vostok::ai::npc_statistics stats = vostok::ai::npc_statistics();

	//
	// DAMAGE MODEL
	//
	damage_model dm = damage_model(type_apply_directly);

	dm.add_body_part(NULL);
	dm.hit_body_part(1, "part_name", "damage_type", 10., 20., 30, NULL);
	dm.apply_med_kit("head", 1000);
	dm.tick(10, 20);
	dm.fill_stats(stats, 20);
	dm.dump_stats(&bpp_dump_stats);
	dm.reset();
	dm.apply_affect("part_name", affects_type_bleeding, affect_canceling);
	dm.cancel_affect("part_name", affects_type_bleeding);
	dm.subscribe_on_affect(affects_type_bleeding, NULL);
	dm.unsubscribe_from_affect(affects_type_bleeding, NULL);
	dm.notify_on_affect_event("body_part_name", affects_type_bleeding, affect_canceling);
	dm.add_damage_protector("damage_type", 1000., 10000.);
	dm.register_body_part_damage_protector("part_name", NULL);
	dm.unregister_body_part_damage_protector("part_name", NULL);
	dm.get_affects_applying_type();
	dm.get_body_part("part_name");
	dm.pop_body_part();
	dm.get_total_health();
	// dm.on_broken_limb_affect("bodypart", affects_type_bleeding, affect_canceling);

	//
	// BODY PART PARAMETERS
	//
	body_part_parameters* bpp = new body_part_parameters(
		"name",
		10.f,
		10.f,
		10.f,
		true,
		dm,
		1);



	bpp->add_hit_type(NULL);
	bpp->add_threshold(NULL);
	bpp->hit_by_type("hit_type", 10, 10., 10., false, NULL);
	bpp->increase_health(10);
	bpp->decrease_health(20);
	bpp->regenerate(10, 20);
	bpp->dump_state(&bpp_dump_stats, 0);
	bpp->dump_state(stats, 10);
	bpp->reset();

	bpp->apply_affect_by_force(affects_type_blindness, affect_canceling, 20);
	bpp->can_affect_death();
	bpp->has_affect_protector(affects_type_blindness);
	bpp->get_health_in_percentage();
	bpp->cancel_affect_by_force(affects_type_blindness);
	bpp->add_damage_protector(NULL);
	bpp->remove_damage_protector(NULL);
	bpp->pop_hit_type();
	bpp->pop_threshold();

	bpp->is_affect_applied(affects_type_blindness);
	bpp->get_hit_parameters("hit_params");
	bpp->set_parameters(10.f, 20.f);

	Callback1 cb1;
	Callback2 cb2;
}


Callback1::Callback1()
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



void Callback1::complete(const char *name)
{
	m_complete = boost::bind(&Callback1::complete, this, _1);
}

void Callback1::partial(const char *name, int value)
{
	m_partial  = boost::bind(&Callback1::partial , this, _1, 10);
}



Callback2::Callback2()
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}

void Callback2::complete(const char *name, int value)
{
	m_complete = boost::bind(&Callback2::complete, this, _1, _2);
}

void Callback2::partial(const char *name, int value, float precision)
{
	m_partial  = boost::bind(&Callback2::partial , this, _1, _2, 10.f);
}


}
