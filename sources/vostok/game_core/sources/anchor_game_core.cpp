#include "pch.h"
#include "anchor.h"

#include "../../collision/sources/loose_oct_tree.h"

// claude@NOTE: g_is_server is a global bool read by weapon_core::activate/deactivate; the
// real definition lives in the application/engine module (outside this build's scope), so
// provide it here in the anchor TU to satisfy the link (weapon_core.obj only references it).
bool g_is_server = false;

namespace vostok {
	void example_callback( const char* name )
	{
		printf( "%s\n", name );
	}
}

// This file's only collision anchors are collision_geometry / collision_sensor,
// which never reference animated_object or bone_collision_data - so it does NOT
// include animated_object.h. That keeps its dllexport explicit instantiation
// (template class buffer_vector<bone_collision_data>) owned by a single anchor TU,
// anchor_collision.cpp (via use_animated_object).

#include <vostok/ai/npc_statistics.h>

#include <vostok/configs_binary_config_value.h>

#include <vostok/animation/skeleton.h>
#include <vostok/animation/mixing_animation_lexeme_parameters.h>

#include <vostok/game_core/affects_threshold.h>
#include <vostok/game_core/breath_vibration_calculator.h>
#include <vostok/game_core/breath_holding_params.h>
#include "breath_holding_states.h"
#include "breath_holding_states_inline.h"
#include <vostok/game_core/usable_object_user_data.h>
#include <vostok/game_core/collision_user.h>
#include <vostok/game_core/normal_random.h>
#include <vostok/game_core/bullet.h>
#include <vostok/game_core/bullet_manager.h>
#include <vostok/game_core/client_player_update.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_sensor.h>
#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_holder.h>
#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/inventory_item_props.h>
#include <vostok/game_core/interactive_object.h>
#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/material_pair.h>
#include <vostok/game_core/hand_to_weapon_ik_processor.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/game_core/weapon_user_animations_selector.h>
#include <vostok/game_core/weapon_user_animations_container_cook.h>
#include <vostok/game_core/base_project.h>
#include <vostok/game_core/static_collision.h>
#include <vostok/game_core/base_player_creation_params.h>
#include <vostok/game_core/ladder.h>
#include <vostok/game_core/medkit.h>
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/player_state.h>
#include <vostok/game_core/server_player_update.h>
#include <vostok/game_core/hit_info.h>
#include <vostok/game_core/legs_ik_processor.h>
#include <vostok/game_core/legs_ik_drawer.h>
#include <vostok/game_core/player_logic_base_state.h>
#include "player_logic_jump_state.h"
#include "player_logic_crouch_state.h"
#include "player_logic_stand_state.h"
#include "player_logic_sprint_state.h"
#include "jump_logic_state_inactive.h"
#include "jump_logic_state_landing.h"
#include "jump_logic_state_start.h"
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/player_stealth.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/weapon_core.h>
#include <vostok/game_core/weapon_ammo_info.h>
#include <vostok/game_core/weapon_state.h>
#include <vostok/game_core/weapon_core_base_state.h>
#include <vostok/game_core/weapon_core_animation_end_aware_state.h>
#include <vostok/game_core/weapon_core_idle_state_base.h>
#include <vostok/game_core/weapon_core_aimed_state_base.h>
#include <vostok/game_core/weapon_core_idle_state.h>
#include <vostok/game_core/weapon_core_aimed_state.h>
#include <vostok/game_core/pistol_weapon_core_idle_state.h>
#include <vostok/game_core/double_barreled_weapon_core_idle_state.h>
#include <vostok/game_core/pistol_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/double_barreled_weapon_core_aimed_idle_state.h>
#include <vostok/game_core/weapon_core_reload_state.h>
#include <vostok/game_core/pistol_weapon_core_show_state.h>
#include <vostok/game_core/pistol_weapon_core_hide_state.h>
#include <vostok/game_core/weapon_core_show_state.h>
#include <vostok/game_core/double_barreled_weapon_core_show_state.h>
#include <vostok/game_core/double_barreled_weapon_core_hide_state.h>
#include <vostok/game_core/weapon_core_hide_state.h>
#include <vostok/game_core/pistol_weapon_core_aimed_fire_state.h>
#include <vostok/game_core/weapon_core_state_cook_template.h>
#include <vostok/game_core/weapon_state_creation_params.h>
#include <vostok/game_core/weapon_core_show_state_base.h>
#include <vostok/game_core/weapon_core_hide_state_base.h>
#include <vostok/game_core/weapon_core_reload_state_base.h>
#include <vostok/game_core/weapon_core_chamber_a_round_state_base.h>
#include <vostok/game_core/weapon_core_fire_state_base.h>
#include <vostok/game_core/weapon_core_chamber_a_round_state.h>
#include <vostok/game_core/weapon_core_chamber_a_round_aimed_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/game_core/weapon_core_shotgun_reload_start_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_one_round_substate.h>
#include <vostok/game_core/weapon_core_shotgun_reload_finish_substate.h>
#include <vostok/game_core/pistol_weapon_core_fire_state.h>
#include <vostok/game_core/pistol_weapon_core_reload_state.h>
#include <vostok/game_core/double_barreled_weapon_core_fire_state.h>
#include <vostok/game_core/double_barreled_weapon_core_aimed_fire_state.h>
#include <vostok/game_core/double_barreled_weapon_core_reload_state.h>
#include <vostok/game_core/weapon_core_fire_state.h>
#include <vostok/game_core/weapon_core_aimed_fire_state.h>

#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/recoil_calculator.h>
#include <vostok/game_core/weapon_recoil_calculator.h>
#include <vostok/game_core/character_dispersion_calculator.h>
#include <vostok/game_core/weapon_dispersion_calculator.h>
#include <vostok/game_core/weapon_recoil_params.h>
#include <vostok/game_core/character_dispersion_params.h>
#include <vostok/game_core/weapon_dispersion_params.h>
#include <vostok/game_core/character_recoil_params.h>

#include <vostok/game_core/victory_item_core_cook.h>
#include <vostok/game_core/victory_item_core.h>
#include <vostok/game_core/weapon_core_cook.h>
#include <vostok/game_core/inventory_cook.h>
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
#include <vostok/game_core/player_profile.h>
#include <vostok/game_core/items_dictionary.h>
#include <vostok/game_core/items_dictionary_cook.h>
#include <vostok/game_core/player_parameters_modifyer.h>
#include <vostok/game_core/player_parameters_modifyer_cook.h>

// physics/collision headers needed by collision_sensor / collision_geometry anchors
// (contact_test_predicate, ghost_object's base_physics_objects_type) - over-inclusion
// is fine in this non-target TU.
#include <vostok/physics/contact_test_predicate.h>
#include <vostok/physics/ghost_object.h>
#include <vostok/physics/api.h>
#include <vostok/collision/api.h>

namespace survarium
{
	void game_core_initialize( );

	float4x4 get_bone_matrix_in_object_space( animation::skeleton_bone const& bone, animation::skeleton const& skeleton, float4x4 const* matrices );
	float4x4 get_bone_matrix_in_object_space_impl( animation::skeleton_bone const& bone, float4x4 const* matrices, animation::skeleton_bone const* first_non_root_bone );

	float get_additional_length( float3 const& upleg_dir, float3 const& leg_dir, float knee_len );
}

// player_logic_base_state's declared destructor is still needed by the abstract
// class vtable retained through the movement_animation_index anchor.
namespace survarium
{
	player_logic_base_state::~player_logic_base_state( ) { }

	void bpp_dump_stats(u32, float, float, pcstr) { }
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

	// claude@MATCH: use_game_core_breath_vibration_calculator removed - redundant.
	// set_breath_holding_params/set_user/tick now have real callers in the matched
	// weapon_core (weapon_core.cpp:790-791,877), and the calculator is a weapon_core
	// member so its ctor is reached there too.

	void use_game_core_breath_holding_states( )
	{
		float reserve = 0.0f;
		survarium::breath_holding_params params;
		survarium::breath_state_normal		st_normal( reserve );
		survarium::breath_state_holding		st_holding( reserve );
		survarium::breath_state_shortbreathing	st_short( reserve );

		// the overrides are private virtuals; dispatch through the base
		// interface so each is reachable and instantiated.
		survarium::breath_state& base_normal = st_normal;
		survarium::breath_state& base_holding = st_holding;
		survarium::breath_state& base_short = st_short;

		base_normal.set_breath_holding_params( &params );
		base_normal.initialize( );
		base_normal.tick( 0.0f );
		base_normal.is_ready_for_transition( );

		base_holding.set_breath_holding_params( &params );
		base_holding.tick( 0.0f );
		base_holding.is_ready_for_transition( );

		base_short.set_breath_holding_params( &params );
		base_short.tick( 0.0f );
		base_short.is_ready_for_transition( );

		example_callback( reinterpret_cast< pcstr >( &st_normal ) );
		example_callback( reinterpret_cast< pcstr >( &st_holding ) );
		example_callback( reinterpret_cast< pcstr >( &st_short ) );
	}

	void use_game_core_usable_object_user_data( )
	{
		survarium::usable_object_user_data data;
		example_callback( reinterpret_cast< pcstr >( &data ) );
	}

	// claude@MATCH: use_game_core_normal_random removed - rand_n now has a real
	// caller (weapon_core.cpp:540); normal_random is a weapon_core member.

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

		// escape the returned float4x4 so LTCG keeps the (observed) body.
		// get_bone_matrix_in_object_space_impl is recursive (calls itself), so the
		// wrapper here keeps a real standalone impl body - no address-of anchor needed.
		float4x4 result	= survarium::get_bone_matrix_in_object_space( *bone, *skeleton, matrices );
		example_callback( reinterpret_cast< pcstr >( &result ) );
	}

	// hand_to_weapon_ik_processor: anchor the public activate / process (ctor anchored
	// by the local instance). process() reaches the private hand_need_correction /
	// hand_need_interpolation / get_hand_new_start_transition_time / process_hand /
	// get_hand_coefficient.
	void use_game_core_hand_to_weapon_ik_processor( )
	{
		survarium::hand_to_weapon_ik_processor processor;
		processor.activate(
			*reinterpret_cast< vostok::animation::skeleton const* >( NULL ),
			*reinterpret_cast< vostok::animation::skeleton const* >( NULL )
		);
		processor.activate_hand( survarium::hand_to_weapon_ik_processor::left, true, 0u );
		processor.process( 0u, reinterpret_cast< float4x4 const* >( NULL ), reinterpret_cast< float4x4* >( NULL ) );

		example_callback( reinterpret_cast< pcstr >( &processor ) );
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

		// weapon_user_animations_selector::set_animations: same device; its target
		// copy keeps the plain /Od thiscall shape, so the emitted COMDAT can match.
		void ( survarium::weapon_user_animations_selector::*set_animations_fn )( survarium::weapon_user_animations_container_ptr const& )
			= &survarium::weapon_user_animations_selector::set_animations;
		example_callback( reinterpret_cast< pcstr >( &set_animations_fn ) );
	}

	// claude@MATCH: use_material_pair removed - decal1/decal1_size/has_particle now
	// have real callers in matched bullet.cpp (530-533). The address-take here kept a
	// standard out-of-line COMDAT; with the real call site present, dropping it lets
	// the sole-caller convention resolve as in the shipped binary (sole-caller lift).

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

		// The virtual calls above never odr-use the out-of-line definitions; a
		// constructed local references the ctor (and through its vtable every
		// override, plus the header-inline virtual dtor) so /OPT:REF keeps them.
		survarium::victory_items_container_core container;
		example_callback( reinterpret_cast< pcstr >( &container ) );
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

	// booby_trap_set_core's ctor / get_visible_place_transform / try_place_trap /
	// trap_index are protected non-virtuals: the cook vtable does not odr-use them.
	// A concrete derived helper that calls them keeps the out-of-line bodies under
	// /OPT:REF. The free predicates trap_is_active / find_free_trap_predicate are
	// taken by address.
	void use_booby_trap_set_internals( ::survarium::booby_trap_core* trap )
	{
		struct concrete_booby_trap_set : survarium::booby_trap_set_core
		{
			virtual	survarium::game_material_manager const&	get_game_material_manager	( ) { return *reinterpret_cast< survarium::game_material_manager const* >( NULL ); }

			void poke( survarium::booby_trap_core* trap )
			{
				vostok::math::float4x4 m;
				get_visible_place_transform( m );
				try_place_trap( );
				trap_index( *trap );
			}
		};

		static concrete_booby_trap_set	s_set;
		s_set.poke( trap );
	}

	void use_hittable_object( survarium::hittable_object* hittable_object )
	{
		configs::binary_config_value	config;

		hittable_object->load( config );
		hittable_object->set_transform( float4x4( ) ),
		hittable_object->insert( NULL );
		hittable_object->remove( );
	}

	void use_usable_object( survarium::usable_object* usable_object )
	{
		// keeps get_transform: the real caller (game_world_ui::update_minimap_objects)
		// is not matched yet, so /OPT:REF drops the unreferenced body.
		usable_object->get_transform( );
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

		// zone_group::on_zone_act is otherwise DCE'd (target rva 0x57d080); reach it
		// through an opaque pointer (the anchor never runs).
		survarium::zone_group* group = NULL;
		group->on_zone_act( NULL, NULL );
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

	void use_inventory_cook( )
	{
		static survarium::inventory_cook s_inventory_cook;
	}

	// items_dictionary_cook's ctor self-registers via register_cook(this). Constructing
	// it keeps the ctor + the vtable (translate_query, delete_resource); translate_query's
	// boost::bind(&on_items_dictionary_config_loaded,...) keeps that private method, which in
	// turn binds on_subresources_loaded - so the whole boost::bind chain cascades from here.
	void use_items_dictionary_cook( )
	{
		static survarium::items_dictionary_cook s_items_dictionary_cook;
		example_callback( reinterpret_cast< pcstr >( &s_items_dictionary_cook ) );
	}

	// player_parameters_modifyer_cook's ctor self-registers via register_cook(this).
	// translate_query / delete_resource / player_parameters_modifyer::apply are already
	// paired (the TU is reachable), but the ctor itself has no construction site - anchor
	// one here so its standalone body pairs in player_parameters_cook.obj.
	void use_player_parameters_modifyer_cook( )
	{
		static survarium::player_parameters_modifyer_cook s_player_parameters_modifyer_cook;
		example_callback( reinterpret_cast< pcstr >( &s_player_parameters_modifyer_cook ) );
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
		calc.tick( survarium::type_stand, true, false, 0, false, 0 );
		calc.fire( );
		calc.reload( );

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

	void use_game_core_weapon_recoil_calculator( )
	{
		// Anchor the calculator: ctor + public entry points keep the COMDATs;
		// the private helpers (process_compensation, get_random_*, reset) are
		// reached transitively from tick/fire/reload/chamber_a_round.
		survarium::weapon_recoil_calculator calc;
		calc.set_weapon( NULL );
		calc.set_interpolation_time( 0.0f );
		calc.set_character_multiplier( 1.0f );
		calc.set_player_compensation_multiplier( 1.0f );
		calc.tick( 0, 1.0f );
		calc.fire( );
		calc.reload( );
		calc.chamber_a_round( );
		calc.get_vertical_koef( );
		calc.get_horizontal_koef( );
		calc.get_back_koef( );

		survarium::pseudo_random pr( 0.0f );
		pr.random_f( 1.0f );

		example_callback( reinterpret_cast< pcstr >( &calc ) );
		example_callback( reinterpret_cast< pcstr >( &pr ) );
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

	void use_game_core_character_recoil_params( )
	{
		// Escape &params so the constant-only ctor stores are OBSERVED (else
		// LTCG dead-store-eliminates them and the ctor compiles empty).
		survarium::character_recoil_params params;

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
		// anchor character_recoil_calculator::set_character_recoil_params (its target
		// caller weapon_core::activate/deactivate is unmatched, so /OPT:REF drops it)
		calc.set_character_recoil_params( NULL );

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
		// same elision that empties the constant-only ctor - see README). The
		// direct calls below keep the setter bodies (also genuinely reached via
		// dispersion_calculator::set_weapon/tick -> m_weapon_calculator.set_*),
		// so the former member-fn-pointer address-of array was redundant.
		calc.set_reload_dispersion_amount( 10.0f );
		calc.set_one_shoot_dispersion_amount( 20.0f );
		calc.set_aiming_speed( 30.0f );
		calc.get_value( );
		calc.tick( 100 );
		calc.fire( );
		calc.reload( );

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

	void use_game_core_weapon_core_ik_callbacks( )
	{
		survarium::weapon_core weapon;
		vostok::animation::animation_callback_params params( 0, vostok::animation::skeleton_animation_ptr(), "", 0, 0, 0, 0 );
		weapon.on_animation_ik_interval( params );

		typedef vostok::animation::mixing::expression (survarium::weapon_core::*gwhe_t)(
			vostok::mutable_buffer&, bool, survarium::weapon_user_state_enum,
			vostok::animation::mixing::animation_lexeme& ) const;
		gwhe_t gwhe = &survarium::weapon_core::get_weapon_and_hands_animation_expression;
		example_callback( reinterpret_cast< pcstr >( &gwhe ) );
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

		// ODR-use the cook's allocate_resource / create_resource / on_subresources_ready via
		// qualified calls on the null cook (no construction -> no vtable / STUB-virtual codegen).
		vostok::resources::query_result_for_cook&	cook_query	= *reinterpret_cast< vostok::resources::query_result_for_cook* >( NULL );
		vostok::resources::queries_result&			cook_results	= *reinterpret_cast< vostok::resources::queries_result* >( NULL );
		mutable_buffer								allocated	= cook->cook_type::allocate_resource( cook_query, vostok::const_buffer( ), false );
		cook->cook_type::create_resource( cook_query, vostok::const_buffer( ), buffer );
		cook->cook_type::on_subresources_ready( cook_results, buffer, params );
		example_callback( reinterpret_cast< pcstr >( allocated.c_ptr( ) ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_reload_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		survarium::weapon_core_reload_state			state( weapon, 1.0f, animations, 8 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_reload_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_reload_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_reload_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_reload_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_reload_state*		object	= cook->cook_type::new_object( buffer, params, animations, 8 );
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

	void use_game_core_pistol_weapon_core_reload_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 16 ];
		survarium::pistol_weapon_core_reload_state	state( weapon, 1.0f, animations, 16 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_reload_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_reload_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::pistol_weapon_core_reload_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_reload_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_reload_state*	object	= cook->cook_type::new_object( buffer, params, animations, 16 );
	}

	void use_game_core_double_barreled_weapon_core_fire_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		survarium::double_barreled_weapon_core_fire_state	state( weapon, 1.0f, animations, 12 );

		state.survarium::double_barreled_weapon_core_fire_state::initialize( );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_fire_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_fire_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::double_barreled_weapon_core_fire_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_fire_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_fire_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
	}

	void use_game_core_double_barreled_weapon_core_aimed_fire_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		survarium::double_barreled_weapon_core_aimed_fire_state	state( weapon, 1.0f, animations, 12 );

		state.survarium::double_barreled_weapon_core_aimed_fire_state::initialize( );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_aimed_fire_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_aimed_fire_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::double_barreled_weapon_core_aimed_fire_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_aimed_fire_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_aimed_fire_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
	}

	void use_game_core_double_barreled_weapon_core_reload_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 16 ];
		survarium::double_barreled_weapon_core_reload_state	state( weapon, 1.0f, animations, 16 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_reload_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_reload_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::double_barreled_weapon_core_reload_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_reload_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_reload_state*	object	= cook->cook_type::new_object( buffer, params, animations, 16 );
	}

	void use_game_core_weapon_core_fire_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		survarium::weapon_core_fire_state			state( weapon, 1.0f, animations, 8 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_fire_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_fire_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_fire_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_fire_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_fire_state*			object	= cook->cook_type::new_object( buffer, params, animations, 8 );
	}

	void use_game_core_weapon_core_aimed_fire_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		survarium::weapon_core_aimed_fire_state		state( weapon, 1.0f, animations, 8 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_aimed_fire_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_aimed_fire_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_aimed_fire_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_aimed_fire_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_aimed_fire_state*	object	= cook->cook_type::new_object( buffer, params, animations, 8 );
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

	void use_game_core_weapon_core_show_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		bool										is_shown	= false;
		survarium::weapon_core_show_state			state( weapon, 1.0f, animations, 8, is_shown );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_show_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_show_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_show_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_show_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_show_state*			object	= cook->cook_type::new_object( buffer, params, animations, 8 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_double_barreled_weapon_core_show_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 16 ];
		bool										is_shown	= false;
		survarium::double_barreled_weapon_core_show_state	state( weapon, 1.0f, animations, 16, is_shown );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_show_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_show_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::double_barreled_weapon_core_show_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_show_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_show_state*	object	= cook->cook_type::new_object( buffer, params, animations, 16 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_pistol_weapon_core_hide_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 12 ];
		bool										is_shown	= false;
		survarium::pistol_weapon_core_hide_state	state( weapon, 1.0f, animations, 12, is_shown );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::pistol_weapon_core_hide_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::pistol_weapon_core_hide_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::pistol_weapon_core_hide_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::pistol_weapon_core_hide_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::pistol_weapon_core_hide_state*	object	= cook->cook_type::new_object( buffer, params, animations, 12 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_double_barreled_weapon_core_hide_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 16 ];
		bool										is_shown	= false;
		survarium::double_barreled_weapon_core_hide_state	state( weapon, 1.0f, animations, 16, is_shown );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::double_barreled_weapon_core_hide_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::double_barreled_weapon_core_hide_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::double_barreled_weapon_core_hide_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::double_barreled_weapon_core_hide_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::double_barreled_weapon_core_hide_state*	object	= cook->cook_type::new_object( buffer, params, animations, 16 );
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

	void use_game_core_weapon_core_chamber_a_round_state( )
	{
		survarium::weapon_core						weapon;
		vostok::resources::managed_resource_ptr		animations[ 8 ];
		survarium::weapon_core_chamber_a_round_state	state( weapon, 1.0f, animations, 8 );

		mutable_buffer								buffer;
		vostok::animation::mixing::animation_lexeme&	lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_chamber_a_round_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_chamber_a_round_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_chamber_a_round_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_chamber_a_round_state > cook_type;
		cook_type*									cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*	params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_chamber_a_round_state*	object	= cook->cook_type::new_object( buffer, params, animations, 8 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}

	void use_game_core_weapon_core_chamber_a_round_aimed_state( )
	{
		survarium::weapon_core								weapon;
		vostok::resources::managed_resource_ptr				animations[ 8 ];
		survarium::weapon_core_chamber_a_round_aimed_state	state( weapon, 1.0f, animations, 8 );

		mutable_buffer										buffer;
		vostok::animation::mixing::animation_lexeme&		lexeme	= *reinterpret_cast< vostok::animation::mixing::animation_lexeme* >( NULL );
		state.survarium::weapon_core_chamber_a_round_aimed_state::weapon_and_hands_expression( buffer, false, survarium::type_stand, lexeme );
		state.survarium::weapon_core_chamber_a_round_aimed_state::get_weapon_lexeme_pair( buffer, false, survarium::type_stand );
		state.survarium::weapon_core_chamber_a_round_aimed_state::get_user_hands_expression( lexeme, buffer, false, survarium::type_stand, lexeme );

		typedef survarium::weapon_core_state_cook_template< survarium::weapon_core_chamber_a_round_aimed_state > cook_type;
		cook_type*											cook	= reinterpret_cast< cook_type* >( NULL );
		survarium::weapon_state_creation_params const*		params	= reinterpret_cast< survarium::weapon_state_creation_params const* >( NULL );
		survarium::weapon_core_chamber_a_round_aimed_state*	object	= cook->cook_type::new_object( buffer, params, animations, 8 );
		example_callback( reinterpret_cast< pcstr >( object ) );

		example_callback( reinterpret_cast< pcstr >( &state ) );
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

	template < typename substate_t >
	void use_game_core_shotgun_reload_substate_impl( )
	{
		// claude@NOTE: constructing the substate emits its vtable, anchoring the virtual
		// overrides initialize/finalize/is_ready_for_transition; initialize's boost::bind in
		// turn references the non-virtual on_animation_end, so the whole reachable set is kept.
		survarium::weapon_core	weapon;
		substate_t				substate( weapon, 0.0f, NULL, 0 );

		example_callback( reinterpret_cast< pcstr >( &substate ) );
	}

	void use_game_core_weapon_core_shotgun_reload_start_substate( )
	{
		use_game_core_shotgun_reload_substate_impl< survarium::weapon_core_shotgun_reload_start_substate >( );
	}

	void use_game_core_weapon_core_shotgun_reload_one_round_substate( )
	{
		use_game_core_shotgun_reload_substate_impl< survarium::weapon_core_shotgun_reload_one_round_substate >( );
	}

	void use_game_core_weapon_core_shotgun_reload_finish_substate( )
	{
		use_game_core_shotgun_reload_substate_impl< survarium::weapon_core_shotgun_reload_finish_substate >( );
	}

	// claude@MATCH: use_bullet's fire() anchor removed - bullet_manager::fire (8-arg)
	// now has a real caller (weapon_core.cpp:571). bullet_manager's ctor is reached by
	// game_world.cpp's NEW(bullet_manager); but its DTOR is anchor-only (game_world
	// never deletes it - sushi@TODO), so keep a stack instance to anchor ~bullet_manager.
	void use_bullet( )
	{
		survarium::bullet_manager bullet_manager( NULL, NULL, NULL );
		example_callback( reinterpret_cast< pcstr >( &bullet_manager ) );
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
		wc.instant_aim_start( );
		wc.instant_aim_end( );
		wc.chamber_a_round( );
		wc.get_dispersion( );
		wc.instant_reload( );
		wc.instant_chamber_a_round( );
		wc.instant_idle_start( );

		wc.set_fire_bullet_transform( vostok::math::float4x4( ) );
		wc.set_next_fire_queue_type( );

		boost::function< enum vostok::animation::callback_return_type_enum( vostok::animation::animation_callback_params& ) > cb;
		wc.set_animation_callback( (pcstr)NULL, NULL, cb );
		wc.set_animation_callback( vostok::animation::channel_id_on_animation_end, NULL, cb );
		wc.remove_animation_callback( (pcstr)NULL, NULL );
		wc.remove_animation_callback( vostok::animation::channel_id_on_animation_end, NULL );

		// claude@NOTE: anchor the private predicates (friend reaches them). could_be_used
		// has no other reachable caller yet (base_player::tick_active_object still STUB).
		bool ( survarium::weapon_core::*pcu )( survarium::base_player const& ) const = &survarium::weapon_core::could_be_used;
		bool ( survarium::weapon_core::*pca )( survarium::base_player const& ) const = &survarium::weapon_core::could_be_aimed;
		bool ( survarium::weapon_core::*pip )( ) const = &survarium::weapon_core::instant_idle_predicate;
		bool ( survarium::weapon_core::*pcr )( ) const = &survarium::weapon_core::can_and_must_reload_predicate;
		bool ( survarium::weapon_core::*pcra )( ) const = &survarium::weapon_core::can_and_must_reload_and_animation_ended_predicate;
		example_callback( reinterpret_cast< pcstr >( &pcu ) );
		example_callback( reinterpret_cast< pcstr >( &pca ) );
		example_callback( reinterpret_cast< pcstr >( &pip ) );
		example_callback( reinterpret_cast< pcstr >( &pcr ) );
		example_callback( reinterpret_cast< pcstr >( &pcra ) );

		wc.get_ammo_slot( survarium::first_ammo );
		wc.ready_to_reload( );

		// recoil values + ammo info getters defined in weapon_core.cpp
		float ( survarium::weapon_core::*pvr )( ) const = &survarium::weapon_core::vertical_recoil_value;
		float ( survarium::weapon_core::*phr )( ) const = &survarium::weapon_core::horizontal_recoil_value;
		example_callback( reinterpret_cast< pcstr >( &pvr ) );
		example_callback( reinterpret_cast< pcstr >( &phr ) );
		survarium::weapon_ammo_info ai;
		wc.get_ammo_info( ai );
		wc.ammo_slot( );

		void ( survarium::weapon_core::*pla )( ) = &survarium::weapon_core::load_ammo;
		example_callback( reinterpret_cast< pcstr >( &pla ) );

		wc.instant_fire( 0 );
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


	// Retains inventory and body-part serialization until their real callers are reconstructed.
	void use_game_core_serialization(
		network_core::udp_match_packet*	packet
	)
	{
		// inventory::serialize is otherwise DCE'd ( /OPT:REF ); anchoring it also emits
		// the static call_item_serialize bind target.
		survarium::inventory					inventory;
		inventory.serialize		( *packet, 0 );

		// body_part_parameters::serialize is otherwise DCE'd (target rva 0x5871f0); no
		// default ctor, so reach it through an opaque pointer (the anchor never runs).
		survarium::body_part_parameters*		body_part = NULL;
		body_part->serialize	( *packet, 0 );
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


	// claude@MATCH: anchor for player_logic_crouch_state unit. Reaches the virtual
	// overrides through the base interfaces (they are private in crouch_state).
	void use_game_core_player_logic_crouch_state()
	{
		survarium::weapon_user_animations_selector&	owner	= *reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );

		survarium::player_logic_crouch_state	state( owner );

		vostok::ai::fsm_state&				fsm		= state;
		fsm.initialize( );
		fsm.execute( );
		fsm.finalize( );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}


	// claude@MATCH: anchor for player_logic_stand_state unit. initialize/execute/finalize
	// are private virtual overrides, so reach them through the base ai::fsm_state interface
	// (mirrors the sibling crouch/jump anchors and keeps the COMDATs paired).
	void use_game_core_player_logic_stand_state()
	{
		survarium::weapon_user_animations_selector&	owner	= *reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );

		survarium::player_logic_stand_state	state( owner );

		vostok::ai::fsm_state&				fsm		= state;
		fsm.initialize( );
		fsm.execute( );
		fsm.finalize( );

		example_callback( reinterpret_cast< pcstr >( &state ) );
	}


	// claude@MATCH: anchor for player_logic_sprint_state (sprint_state unit). Mirrors
	// the jump anchor: construct (ODR-uses ctor + the bound on_stamina_depleted),
	// reach the private virtual overrides through the base interface, and call the
	// public set_callbacks. Never runs; null refs are fine.
	void use_game_core_player_logic_sprint_state( )
	{
		survarium::weapon_user_animations_selector&	owner	= *reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );

		survarium::player_logic_sprint_state	state( owner );

		vostok::ai::fsm_state&				fsm		= state;

		fsm.initialize( );
		fsm.execute( );
		fsm.finalize( );

		boost::function< void() > const&	cb	=
			*reinterpret_cast< boost::function< void() > const* >( NULL );
		state.set_callbacks( cb, cb );

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
		// landing_predicate is private (target mangling ABE); it is ODR-used through
		// initialize_logic's boost::bind, anchored by the jump_logic construction below.
		jl.tick( );
		float	lt	= jl.look_time_factor( );
		bool	jf	= jl.is_jump_finished( );
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

		// claude@MATCH: the C4716 state-vtable trap is gone (jump_logic_state_*
		// selected_animations now return on the common ground), so constructing a
		// jump_logic anchors the ctor -> initialize_logic + the dtor without LNK1257.
		// The anchor never runs (the reference is NULL); construction only ODR-uses
		// the bodies for the linker.
		survarium::weapon_user_animations_selector& sel_owner =
			*reinterpret_cast< survarium::weapon_user_animations_selector* >( NULL );
		survarium::jump_logic anchored_jump_logic( sel_owner );
		example_callback( reinterpret_cast< pcstr >( &anchored_jump_logic ) );

		// get_move_direction is a file static in jump_logic.cpp (plain-name target
		// symbol); it is kept alive by its real callers activate/does_need_land_and_run.
		u32 idx = survarium::get_jump_animation_index( survarium::move_direction_on_site, true, survarium::jump_animations_part_start );
		example_callback( reinterpret_cast< pcstr >( &idx ) );

		// Address-of non-virtual members to ODR-use their bodies WITHOUT constructing a
		// jump_logic (which would emit the vtable and force still-STUB members to codegen).
		void ( survarium::jump_logic::*su )( survarium::base_player& ) = &survarium::jump_logic::set_user;
		void ( survarium::jump_logic::*de )( )                         = &survarium::jump_logic::deactivate;
		bool ( survarium::jump_logic::*dn )( ) const                   = &survarium::jump_logic::does_need_land_and_run;
		example_callback( reinterpret_cast< pcstr >( &su ) );
		example_callback( reinterpret_cast< pcstr >( &de ) );
		example_callback( reinterpret_cast< pcstr >( &dn ) );

		pcstr ( survarium::jump_logic::*gac )( const survarium::jump_animation_parts ) const = &survarium::jump_logic::get_animation_caption;
		vostok::resources::managed_resource_ptr ( survarium::jump_logic::*gma )( const bool ) const  = &survarium::jump_logic::get_move_animation;
		vostok::resources::managed_resource_ptr ( survarium::jump_logic::*gmla )( const bool ) const = &survarium::jump_logic::get_move_look_animation;
		pcstr ( survarium::jump_logic::*gmlc )( ) const = &survarium::jump_logic::get_move_look_caption;
		example_callback( reinterpret_cast< pcstr >( &gac ) );
		example_callback( reinterpret_cast< pcstr >( &gma ) );
		example_callback( reinterpret_cast< pcstr >( &gmla ) );
		example_callback( reinterpret_cast< pcstr >( &gmlc ) );
	}

	// get_shapes_centers has no target caller in the reconstructed game path yet.
	void use_game_core_collision_geometry()
	{
		survarium::collision_geometry& gm = *reinterpret_cast< survarium::collision_geometry* >( NULL );
		vostok::vectora<float3> centers_results( NULL );
		gm.get_shapes_centers( centers_results );
	}

	// claude@MATCH: use_game_core_scheduler trimmed - register_on_frame / register_for_update
	// have real callers in matched damage_zone_core.cpp / booby_trap_core.cpp /
	// generic_anomaly_core.cpp / medkit.cpp / oxygen_tank.cpp. Only on_frame (the public
	// dispatcher + its private record overload) has no real caller yet, so keep just it.
	void use_game_core_scheduler()
	{
		survarium::scheduler& sc = *reinterpret_cast< survarium::scheduler* >( NULL );
		sc.on_frame( 10, 10 );
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

	// claude@MATCH: base_player is abstract; a concrete derived stub (overriding every
	// pure virtual + the unoverridden inventory_holder pures) gives a constructible
	// instance so the ctor body is ODR-used and scorable.
	struct concrete_base_player : survarium::base_player
	{
		concrete_base_player( survarium::base_player_creation_params const& params, survarium::scheduler& s )
			: survarium::base_player( params, s ) { }

		// inventory_holder pure virtuals not overridden by base_player
		virtual bool							set_new_active_item	( survarium::inventory_item_ptr const& ) override { return false; }
		virtual void							take_inventory_item	( survarium::inventory_item_ptr const& ) override { }
		virtual physics::world*					get_physics_world	( ) override { return NULL; }

		// collision_user pure virtuals
		virtual float4x4 const&					get_transform		( ) const override { return *reinterpret_cast< float4x4* >( NULL ); }
		virtual void							use_ladder			( survarium::ladder* ) override { }

		// hit_receiver pure virtuals
		virtual void							hit					( survarium::hit_initiator const* const, u32 const, pcstr, float const, float const, survarium::bullet* const ) override { }
		virtual void							hit					( survarium::hit_initiator const* const, collision::bone_collision_data const&, pcstr, float const, float const, survarium::bullet* const ) override { }
		virtual float							get_speed			( ) const override { return 0.0f; }

		// base_player pure virtuals
		virtual animation::skeleton const&		skeleton			( ) const override { return *reinterpret_cast< animation::skeleton* >( NULL ); }
		virtual survarium::player_input const&	input				( ) const override { return *reinterpret_cast< survarium::player_input* >( NULL ); }
		virtual float3 const&					position			( ) const override { return *reinterpret_cast< float3* >( NULL ); }
		virtual float							get_look_pitch		( ) const override { return 0.0f; }
		virtual physics::bt_character_controller&	physics_controller	( ) override { return *reinterpret_cast< physics::bt_character_controller* >( NULL ); }
		virtual survarium::player_stamina&		stamina				( ) override { return *reinterpret_cast< survarium::player_stamina* >( NULL ); }
		virtual void							jump				( ) override { }
		virtual void							end_jump			( ) override { }
		virtual void							crouch				( ) override { }
		virtual void							stand_up			( ) override { }
		virtual survarium::game_team_id			team				( ) const override { return survarium::game_team_id( ); }
		virtual void							subscribe_animation_player	( pcstr, animation_callback const&, pcvoid, resources::managed_resource_ptr const&, u8, pcvoid ) override { }
		virtual void							subscribe_animation_player	( animation::reserved_channel_ids_enum, animation_callback const&, pcvoid, resources::managed_resource_ptr const&, pcvoid ) override { }
		virtual void							unsubscribe_animation_player( pcstr, pcvoid ) override { }
		virtual void							unsubscribe_animation_player( animation::reserved_channel_ids_enum, pcvoid ) override { }
		virtual bool							is_replaying_history	( ) const override { return false; }
		virtual u32								local_time			( u32 ) const override { return 0; }
		virtual survarium::engine&				get_engine			( ) override { return *reinterpret_cast< survarium::engine* >( NULL ); }
		virtual animation::animation_player const&	animation_player	( ) const override { return *reinterpret_cast< animation::animation_player* >( NULL ); }
		virtual animation::animation_player&	animation_player	( ) override { return *reinterpret_cast< animation::animation_player* >( NULL ); }
	};

	// claude@MATCH: anchor for base_player non-virtual leaf functions. The non-virtual
	// leaves are ODR-used by qualified calls on a null ref; the ctor needs the concrete
	// stub above. on_player_death also pulls in the free helper
	// call_player_death_subscriber_callback (taken as a function ref by for_each).
	void use_game_core_base_player( )
	{
		// Construct a real base_player_creation_params so its default ctor (member-ctor
		// chain: player_initial_info / breath_holding_params defaults, recoil / dispersion
		// / stamina / stealth) is ODR-used and pairs; escape it so the stores are observed.
		survarium::base_player_creation_params	cp;
		example_callback( reinterpret_cast< pcstr >( &cp ) );
		survarium::scheduler					sched( NULL );
		concrete_base_player					cbp( cp, sched );
		example_callback( reinterpret_cast< pcstr >( &cbp ) );

		survarium::base_player&	p	= *reinterpret_cast< survarium::base_player* >( NULL );
		p.subscribe_on_player_death( NULL );
		p.unsubscribe_from_player_death( NULL );
		p.on_player_death( );
		p.tick_active_object( );
		p.send_game_world_object(
			reinterpret_cast< survarium::game_world_object const* >( NULL ),
			*reinterpret_cast< boost::function< vostok::network_core::udp_match_packet& ( ) >* >( NULL ),
			*reinterpret_cast< boost::function< void ( vostok::network_core::udp_match_packet& ) >* >( NULL ) );
		example_callback( reinterpret_cast< pcstr >( &p ) );
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

		// claude@MATCH (batch2): anchor the remaining private/public leaf members that no
		// reachable caller keeps (LTCG would otherwise inline-drop them to None).
		bool ( self::*p7 )( ) const = &self::is_weapon_firing;
		bool ( self::*p8 )( ) const = &self::is_weapon_toggling;
		bool ( self::*p9 )( ) const = &self::is_weapon_in_idle;
		bool ( self::*p10 )( ) const = &self::broken_legs_predicate;
		bool ( self::*p11 )( ) const = &self::stand_predicate;
		bool ( self::*p12 )( ) const = &self::crouch_predicate;
		bool ( self::*p13 )( ) const = &self::jump_predicate;
		bool ( self::*p14 )( ) const = &self::sprint_predicate;
		animation::callback_return_type_enum ( self::*p15 )( animation::animation_callback_params& ) = &self::on_interval_ended;
		void ( self::*p16 )( pcstr, survarium::hit_affects_type_enum, survarium::affect_event_type_enum ) = &self::on_broken_limb_affect;
		void ( self::*p17 )( boost::function< void( ) > const&, boost::function< void( ) > const& ) = &self::set_sprint_callbacks;
		float ( self::*p18 )( ) const = &self::look_time_factor;
		float ( self::*p19 )( float, float, u32, u32, u32, float ) const = &self::look_time_factor_calculator;
		void ( self::*p20 )( animation::reserved_channel_ids_enum, pcvoid, self::animation_functor const& ) = &self::set_animation_callback;
		void ( self::*p21 )( pcstr, pcvoid, self::animation_functor const& ) = &self::set_animation_callback;
		void ( self::*p22 )( survarium::base_player&, boost::function< void( ) > const&, boost::function< void( ) > const& ) = &self::activate;
		example_callback( reinterpret_cast< pcstr >( &p7 ) );
		example_callback( reinterpret_cast< pcstr >( &p8 ) );
		example_callback( reinterpret_cast< pcstr >( &p9 ) );
		example_callback( reinterpret_cast< pcstr >( &p10 ) );
		example_callback( reinterpret_cast< pcstr >( &p11 ) );
		example_callback( reinterpret_cast< pcstr >( &p12 ) );
		example_callback( reinterpret_cast< pcstr >( &p13 ) );
		example_callback( reinterpret_cast< pcstr >( &p14 ) );
		example_callback( reinterpret_cast< pcstr >( &p15 ) );
		example_callback( reinterpret_cast< pcstr >( &p16 ) );
		example_callback( reinterpret_cast< pcstr >( &p17 ) );
		example_callback( reinterpret_cast< pcstr >( &p18 ) );
		example_callback( reinterpret_cast< pcstr >( &p19 ) );
		example_callback( reinterpret_cast< pcstr >( &p20 ) );
		example_callback( reinterpret_cast< pcstr >( &p21 ) );
		example_callback( reinterpret_cast< pcstr >( &p22 ) );
	}

	// A concrete local retains the base_project constructor and vtable.
	struct concrete_base_project : survarium::base_project
	{
	};

	void use_game_core_base_project( )
	{
		concrete_base_project	p;
		example_callback( reinterpret_cast< pcstr >( &p ) );

		// static_collision insert/remove are public non-virtual leaves on a null ref.
		survarium::static_collision&	sc	= *reinterpret_cast< survarium::static_collision* >( NULL );
		sc.insert( reinterpret_cast< vostok::physics::world* >( NULL ) );
		sc.remove( reinterpret_cast< vostok::physics::world* >( NULL ) );
		example_callback( reinterpret_cast< pcstr >( &sc ) );

		// read_transform is a free helper with external linkage used only by
		// project_cooker_simple::create_game_objects; ODR-use it to keep its body.
		survarium::read_transform(
			*reinterpret_cast< vostok::configs::binary_config_value const* >( NULL ),
			*reinterpret_cast< vostok::math::float4x4* >( NULL ) );
	}

	void use_game_core_weapon_user_animations_container_cook( )
	{
		// Constructing + registering keeps the vtable (translate_query, delete_resource).
		static survarium::weapon_user_animations_container_cook	s_cook;
		vostok::resources::register_cook( &s_cook );
		example_callback( reinterpret_cast< pcstr >( &s_cook ) );

		// claude@NOTE: translate_query (real body) -> on_config_loaded (real body) ->
		// create_requests_for_animations + on_animations_loaded -> get_animations_from_request_results<N>
		// are now all emitted via the boost::bind chain kept by this anchor. The free
		// create_requests/get_animations helpers still read 0% (per-obj pairing limit + ICF
		// fold of the 4 identical template instances); the member fns are matched (translate_query
		// 99.82, on_animations_loaded 99.97, on_config_loaded 91.05). See
		// docs/binary_matching/game_core/weapon_user_animations_container_cook.md.
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
}

namespace vostok
{
	void anchor_game_core( )
	{
		use_game_core_initialize( );
		use_game_core_legs_ik_processor( );
		use_game_core_ik_processor( NULL, NULL, NULL );
		use_game_core_hand_to_weapon_ik_processor( );
		use_medkit( );
		use_inventory_2( );
		use_victory_items_container_core( NULL );
		use_booby_trap_cook( );
		use_booby_trap_set_internals( NULL );
		use_hittable_object( NULL );
		use_usable_object( NULL );
		use_respawn_point_core( );
		use_damage_zone_core( );
		use_generic_anomaly_core( );
		use_artefact_container_core( );
		use_artefact_lifebone_core( );
		use_victory_item_core( );
		use_weapon_core_cook( );
		use_inventory_cook( );
		use_items_dictionary_cook( );
		use_player_parameters_modifyer_cook( );
		use_weapon_core_shotgun_reload_state_cook( );
		use_weapon_core_inactive_state_cook( );
		use_game_core_weapon_recoil_params( );
		use_game_core_weapon_recoil_calculator( );
		use_game_core_character_dispersion_params( );
		use_game_core_character_recoil_params( );
		use_game_core_weapon_dispersion_params( );
		use_recoil_calculator( );
		use_dispersion_calculator( );
		use_game_core_breath_holding_states( );
		use_game_core_usable_object_user_data( );
		use_character_dispersion_calculator( );
		use_game_material_manager( );
		use_weapon_dispersion_calculator( );
		use_game_core_weapon_core_base_state( );
		use_game_core_weapon_core_ik_callbacks( );
		use_game_core_weapon_core_animation_end_aware_state( );
		use_game_core_weapon_core_idle_state_base( );
		use_game_core_weapon_core_aimed_state_base( );
		use_game_core_weapon_core_idle_state( );
		use_game_core_weapon_core_aimed_state( );
		use_game_core_pistol_weapon_core_idle_state( );
		use_game_core_double_barreled_weapon_core_idle_state( );
		use_game_core_pistol_weapon_core_aimed_idle_state( );
		use_game_core_weapon_core_reload_state( );
		use_game_core_pistol_weapon_core_show_state( );
		use_game_core_weapon_core_show_state( );
		use_game_core_double_barreled_weapon_core_show_state( );
		use_game_core_double_barreled_weapon_core_hide_state( );
		use_game_core_pistol_weapon_core_hide_state( );
		use_game_core_weapon_core_hide_state( );
		use_game_core_pistol_weapon_core_aimed_fire_state( );
		use_game_core_double_barreled_weapon_core_aimed_idle_state( );
		use_game_core_weapon_core_show_state_base( );
		use_game_core_weapon_core_hide_state_base( );
		use_game_core_weapon_core_reload_state_base( );
		use_game_core_weapon_core_chamber_a_round_state_base( );
		use_game_core_weapon_core_fire_state_base( );
		use_game_core_weapon_core_initialize_weapon_logic( );
		use_game_core_weapon_core_chamber_a_round_state( );
		use_game_core_weapon_core_chamber_a_round_aimed_state( );
		use_game_core_weapon_core_shotgun_reload_state( );
		use_game_core_weapon_core_shotgun_reload_start_substate( );
		use_game_core_weapon_core_shotgun_reload_one_round_substate( );
		use_game_core_weapon_core_shotgun_reload_finish_substate( );
		use_game_core_pistol_weapon_core_fire_state( );
		use_game_core_pistol_weapon_core_reload_state( );
		use_game_core_double_barreled_weapon_core_fire_state( );
		use_game_core_double_barreled_weapon_core_aimed_fire_state( );
		use_game_core_double_barreled_weapon_core_reload_state( );

		use_game_core_weapon_core_fire_state( );
		use_game_core_weapon_core_aimed_fire_state( );
		use_game_core_weapon_core_small_setters( );
		use_bullet( );
		use_ladder( NULL );
		use_game_core_affects_threshold();
		use_game_core_player_stamina();
		use_game_core_player_stealth();
		use_game_core_serialization( NULL );
		use_game_core_weapon_state();
		use_game_core_player_logic_base_state();
		use_game_core_player_logic_jump_state();
		use_game_core_player_logic_crouch_state();
		use_game_core_player_logic_stand_state();
		use_game_core_player_logic_sprint_state();
		use_game_core_jump_logic_state_inactive();
		use_game_core_jump_logic_state_landing();
		use_game_core_jump_logic_state_start();
		use_game_core_jump_logic();
		use_game_core_collision_geometry();
		use_game_core_scheduler();
		use_game_core_inventory_holder();
		use_game_core_weapon_user_animations_selector();
		use_game_core_base_player();
		use_game_core_weapon_user_animations_container_cook();
		use_game_core_base_project();
		use_game_core_booby_trap_core_get_speed();

		//
		// YEEET
		//
		survarium::hit_type_parameters* htp = new survarium::hit_type_parameters("hand", 1., 1., 1., 0);
		htp->apply_damage(10., 100);
		htp->set_parameters(10., 20., 30.);

		survarium::booster_damage_protector* bdp = new survarium::booster_damage_protector("hand", 0.5, 1.);
		bdp->reduce_damage("__whatever", "hand", 100, 10);

		vostok::ai::npc_statistics stats = vostok::ai::npc_statistics();

		//
		// DAMAGE MODEL
		//
		survarium::damage_model dm = survarium::damage_model(survarium::type_apply_directly);

		dm.add_body_part(NULL);
		dm.hit_body_part(1, "part_name", "damage_type", 10., 20., 30, NULL);
		dm.apply_med_kit("head", 1000);
		dm.tick(10, 20);
		dm.fill_stats(stats, 20);
		dm.dump_stats(&survarium::bpp_dump_stats);
		dm.reset();
		dm.apply_affect("part_name", survarium::affects_type_bleeding, survarium::affect_canceling);
		dm.cancel_affect("part_name", survarium::affects_type_bleeding);
		dm.subscribe_on_affect(survarium::affects_type_bleeding, NULL);
		dm.unsubscribe_from_affect(survarium::affects_type_bleeding, NULL);
		dm.notify_on_affect_event("body_part_name", survarium::affects_type_bleeding, survarium::affect_canceling);
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
		survarium::body_part_parameters* bpp = new survarium::body_part_parameters(
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
		bpp->dump_state(&survarium::bpp_dump_stats, 0);
		bpp->dump_state(stats, 10);
		bpp->reset();

		bpp->apply_affect_by_force(survarium::affects_type_blindness, survarium::affect_canceling, 20);
		bpp->can_affect_death();
		bpp->has_affect_protector(survarium::affects_type_blindness);
		bpp->get_health_in_percentage();
		bpp->cancel_affect_by_force(survarium::affects_type_blindness);
		bpp->add_damage_protector(NULL);
		bpp->remove_damage_protector(NULL);
		bpp->pop_hit_type();
		bpp->pop_threshold();

		bpp->is_affect_applied(survarium::affects_type_blindness);
		bpp->get_hit_parameters("hit_params");
		bpp->set_parameters(10.f, 20.f);
	}
}
