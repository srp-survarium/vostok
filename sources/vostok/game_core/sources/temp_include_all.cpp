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

#include <vostok/game_core/affects_threshold.h>
#include <vostok/game_core/bullet.h>
#include <vostok/game_core/bullet_manager.h>
#include <vostok/game_core/collision_geometry.h>
#include <vostok/game_core/collision_sensor.h>
#include <vostok/game_core/damage_model_cook.h>
#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/inventory_item_props.h>
#include <vostok/game_core/ladder.h>
#include <vostok/game_core/medkit.h>
#include <vostok/game_core/player_input.h>
#include <vostok/game_core/player_stamina.h>
#include <vostok/game_core/scheduler.h>
#include <vostok/game_core/weapon_core.h>

#include <vostok/game_core/game_material_manager.h>
#include <vostok/game_core/recoil_calculator.h>

#include <vostok/game_core/victory_item_core_cook.h>
#include <vostok/game_core/victory_item_core.h>
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

namespace vostok
{
	void use_booby_trap_core( survarium::booby_trap_core* booby_trap_core )
	{

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
		core.use_info( NULL );

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

	void use_bullet( )
	{
		survarium::bullet_manager			bullet_manager( NULL, NULL, NULL );
		survarium::weapon_ammunition_ptr	wa( NULL );
		survarium::weapon_core				wc;

		bullet_manager.fire( float3(), float3(), wa, wc, 10, NULL, NULL, true );
	}


	void use_inventory( )
	{
		survarium::medkit item;

		survarium::inventory_item_props props;
		item.get_item_props( props );

		item.remove( );

		item.reduce_damage( NULL, NULL, 0.0f, 0.0f );
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


	void use_game_core_player_input()
	{
		survarium::player_input input;
		input.is_sprinting();
		input.is_empty();
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
	vostok::use_booby_trap_cook( );
	vostok::use_hittable_object( NULL );
	vostok::use_respawn_point_core( );
	vostok::use_damage_zone_core( );
	vostok::use_generic_anomaly_core( );
	vostok::use_artefact_container_core( );
	vostok::use_artefact_lifebone_core( );
	vostok::use_victory_item_core( );
	vostok::use_recoil_calculator( );
	vostok::use_game_material_manager( );
	vostok::use_bullet( );
	vostok::use_inventory( );
	vostok::use_damage_model_cook( );
	vostok::use_ladder( NULL );
	vostok::use_game_core_affects_threshold();
	vostok::use_game_core_player_stamina();
	vostok::use_game_core_player_input();
	vostok::use_game_core_collision_sensor();
	vostok::use_game_core_collision_geometry();
	vostok::use_game_core_scheduler();
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
