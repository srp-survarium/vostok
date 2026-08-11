#include "pch.h"
#include "../../game_core/sources/anchor.h"	// survarium::IncludeAll + the anchor_* decls (game_core-private header)

// Game-module /OPT:REF reachability anchor for the game-skeleton symbols that no
// engine-module anchor owns. The game_core/anchor_*.cpp files own every
// use_game_core_* / use_<engine-module>_* symbol. Everything left over lives here:
//   - use_engine_user_world_cone(): the ai/ui/animation/input/rtp world cone the
//     game module pulls up,
//   - use_inventory(): the (still-empty) inventory stub,
//   - use_game_skeleton(): the game/menu/camera/scene/stats/key_binder object cone,
//   - anchor_game_network_clients(): the matched network-client carcass
//     (anchor_game_clients.cpp).
// anchor_game() is dispatched from survarium::IncludeAll::IncludeAll() (anchor.cpp),
// instantiated in game_entry_point.cpp::create_world - the real engine startup root
// (the game_world ctor is a never-instantiated stub, so an anchor wired there gets
// /OPT:REF-stripped along with its whole cone). This file + anchor_game_clients.cpp
// replace the deleted monolithic temp_include_all.cpp.
//
// Retire once the real game call graph (game_module::create_world and friends)
// reaches these for itself.

// for use_engine_user_world_cone (the temp game-cone anchor)
#include <vostok/ai/api.h>
#include <vostok/ai_navigation/api.h>
#include <vostok/animation/api.h>
#include <vostok/input/api.h>
#include <vostok/ui/api.h>

#include <vostok/render/facade/one_way_render_channel.h>
#include <vostok/render/world.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/debug_renderer.h>

#include <vostok/managed_allocator.h>

#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_item.h>
#include <vostok/game_core/inventory_item_props.h>
#include <vostok/game_core/medkit.h>

// the game-skeleton object cone. game itself is abstract (engine_user::world::load
// stays pure-virtual - game's load has a different signature), so it can't be
// constructed; instead use_game_skeleton() (a) constructs each concrete subsystem
// game owns/creates - game_world, the menus, the cameras, the cooks, stats - to keep
// their ctors/dtors/vtables, and (b) address-takes game's own public methods through
// a volatile sink to keep the game vtable + members past /OPT:REF.
#include "game.h"
#include "game_world.h"
#include "key_binder.h"
#include "lobby_menu.h"
#include "login_menu.h"
#include "main_menu.h"
#include "free_fly_camera.h"
#include "camera_director.h"
#include "game_camera.h"
#include "stats.h"
#include "stats_graph.h"
#include "damage_model_stats.h"
#include "game_generate_shaders.h"

// the object-skeleton base cone (game_object_ / game_object_static + the file-local
// load_transform free function in object.cpp); the object_* visual family derives
// from game_object_static, but its base ctors/load live out-of-line in object.cpp.
#include "game_object_static.h"
#include "object_light.h"
#include "object_decal.h"
#include "object_wire.h"
#include "object_sky.h"
#include "object_environment.h"
#include "object_particle_visual.h"
#include "object_sound.h"
#include "object_volumetric_sound.h"
#include "object_lpv_occluder.h"
#include "object_ambient_volume.h"
#include "object_environment_probe.h"
#include "object_sky_ambient_occlusion.h"
#include "object_volume_fog.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

// the network-client carcass anchor (anchor_game_clients.cpp); it self-guards and
// never runs, so the placeholder game& is never dereferenced. Driving it from the
// reachable anchor_game() keeps the matched network_client / lobby_client /
// match_client / messaging_client / network_stats objects AND the game/menu/
// camera/cook/stats skeleton they reference through game& in the base EXE.
namespace vostok {
	// defined in anchor_game_stats.cpp: keeps the debug-HUD stats cone
	// (stats / stats_graph / npc_stats) past /OPT:REF.
	void use_game_stats( );
}

namespace survarium {
	void anchor_game_network_clients( game& g );
	void use_game_world( );	// anchor_game_world.cpp (game-class public methods)

	// the options-menu item-widget carcass anchor (anchor_game_options_items.cpp);
	// self-guarded, never runs.
	void use_game_options_items( );

	// the lobby-menu UI carcass anchor (anchor_game_lobby_ui.cpp); self-guarded.
	void use_game_lobby_ui( );

	// the lobby character-preview scene carcass anchor (anchor_game_lobby_scene.cpp);
	// self-guarded, never runs.
	void use_game_lobby_scene( );

	// the lobby_camera carcass anchor (anchor_game_lobby_camera.cpp); self-guarded.
	void use_game_lobby_camera( );

	// the fingers_to_weapon_corrector carcass anchor (anchor_game_fingers.cpp);
	// self-guarded.
	void use_game_fingers( );

	// the survarium::player carcass anchor (anchor_game_player.cpp); pins the
	// player out-of-line bodies so /OPT:REF keeps them for the delinker.
	void anchor_game_player( );
	void anchor_game_world_ui( );
	void anchor_game_chat( game& g );

	// game_world NPC-management reachability anchor (anchor_game_world_npc.cpp); it
	// self-guards and never runs, so the placeholder game_world& is never touched.
	void anchor_game_world_npc( game_world& w );

	// the player_cook carcass anchor (anchor_game_player_cook.cpp); self-guarded.
	void use_game_player_cook( );
	// player_logic_preview_state carcass anchor (anchor_game_player_logic.cpp); self-guarded.
	void use_game_player_logic( );

	// file-local free function defined in object.cpp (no public header); the
	// game_object_static::load impl calls it.
	void load_transform( configs::binary_config_value const& t, float4x4& dest );

	// out-of-line base_game_scene base methods (anchor_game_base.cpp)
	void use_game_base( );
	// player_tick.cpp / player_input_handler.cpp reachability anchor
	// (anchor_game_player_input.cpp).
	void use_game_player_input( );
	// the game_options menu-input carcass anchor (anchor_game_options.cpp); keeps
	// every out-of-line game_options body + swf_input_translator::process_keyboard
	// past /OPT:REF until game::activate_main_menu's input path is matched.
	void use_game_options( );

	// the generic_anomaly / object_vegetation world-object carcass anchor
	// (anchor_game_anomaly_veg.cpp); self-guarded, never runs.
	void use_game_anomaly_veg( );

	// the global_input_handler carcass anchor (anchor_game_global_input.cpp);
	// self-guarded, never runs. Reached only from the still-stub
	// game::on_base_resources_created static g_input_handler initializer.
	void use_game_global_input( );
}

namespace vostok
{
	// NPC / animation cluster reachability anchor (anchor_game_npc.cpp).
	void use_game_npc( );
	// defined in anchor_game_animation.cpp (the animation-controller carcass anchor)
	void use_game_animation( );

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

	void use_engine_user_world_cone()
	{
		ai::world* ai_world = ai::create_world( *( ai::engine* )NULL );
		ai::destroy_world( ai_world );
		ai::set_memory_allocator( *( ai::allocator_type* )NULL );

		ui::world* ui_world = ui::create_world( *( ui::engine* )NULL, *( render::ui::renderer* )NULL, *( memory::base_allocator* )NULL, NULL );
		ui::destroy_world( ui_world );

		input::world* input_world = input::create_world( *( input::engine* )NULL, NULL );
		input::destroy_world( input_world );
		input::set_memory_allocator( *( input::allocator_type* )NULL );

		ai::navigation::world* ai_navigation_world = ai::navigation::create_world( *( ai::navigation::engine* )NULL, *( render::scene_ptr const* )NULL, *( render::debug::renderer* )NULL );
		ai::navigation::destroy_world( ai_navigation_world );
		ai::navigation::set_memory_allocator( *( ai::navigation::allocator_type* )NULL );
	}

	// Keep the game-skeleton object cone (game + game_world + menus/cameras/cooks/
	// stats/key_binder) past /OPT:REF. Self-guards like anchor_game_clients so the
	// heavy ctors never actually run (the refs are placeholders); the compiler still
	// emits every reference, so /OPT:REF keeps the carcass objects in the base EXE.
	void use_game_skeleton( )
	{
		static volatile bool s_run = false;
		if( !s_run )
			return;

		survarium::game& g = *( survarium::game* )NULL;

		// concrete subsystems game owns/creates: constructing each keeps its
		// ctor/dtor/vtable (the menus/cameras/scenes are vtable'd).
		survarium::game_world			scene( g );
		survarium::key_binder			binder( g );
		survarium::lobby_menu			lobby( g );
		survarium::login_menu			login( g );
		// volatile source so set_status's status arg is not constant-folded (it would
		// collapse the matched m_status = status store into m_status = <const>)
		static volatile int s_login_status = survarium::login_menu_status_connected;
		login.set_status				( (survarium::login_menu_status_enum)s_login_status );
		survarium::main_menu			main( g );
		survarium::camera_director		director( scene );
		survarium::game_camera			camera( scene );
		survarium::free_fly_camera		fly( scene, director );
		survarium::stats				stats_obj( *( ui::world* )NULL );
		survarium::stats_graph			graph( 0.f, 0.f, 0.f, 0.f, 0u );
		survarium::damage_model_stats	dm_stats( *( ui::world* )NULL );
		// project_cooker_simple is intentionally NOT constructed: its delete_resource
		// override has no body in our sources, so emitting its vtable would not link
		// (LNK2001). Its ~4 fns are a tiny residual the cook-matching phase recovers.

		// call the public non-virtual methods too (construction alone keeps only the
		// ctor/dtor/vtable - the target carries these out-of-line, so reference them).
		render::ui::renderer&			ui_renderer	= *( render::ui::renderer* )NULL;
		render::scene_view_ptr const&	scene_view	= *( render::scene_view_ptr const* )NULL;
		float3 const&					vec			= *( float3 const* )NULL;

		// source the scalar args through volatile sinks so LTCG cannot prove this
		// anchor is the sole caller with a constant arg and constant-propagate it
		// into the body (folding the switch / dropping the param store).
		static survarium::input_mode_type_enum volatile	s_mode	= survarium::first_person_mode;
		static survarium::player_input_handler* volatile	s_cam	= 0;
		static u8 volatile									s_item	= 0;
		static float4x4 const* volatile						s_xform	= 0;

		static survarium::player_ptr* volatile	s_player	= 0;

		scene.unload( );
		scene.empty( );
		scene.switch_camera_mode( s_mode );
		scene.set_local_player_camera( s_cam );
		scene.put_victory_item( s_item, *s_xform );
		scene.get_current_time_in_ms( );
		scene.on_player_killed( *s_player, s_item, s_item );

		// get_sound_world returns sound::world& - reference the address (the
		// header forward-declares sound::world incompletely)
		static pcvoid volatile s_gw_sink = 0;
		sound::world& ( survarium::game_world::* const m_gsw )( ) const = &survarium::game_world::get_sound_world;
		s_gw_sink = *( pcvoid const* )&m_gsw;

		binder.set_default_controls( );
		binder.unbind_key( "", 0 );

		director.switch_to_camera( &camera, "" );
		director.tick( );
		director.apply( );

		camera.set_position_direction( vec, vec );
		camera.get_vertical_fov( );

		stats_obj.draw( ui_renderer, scene_view );
		stats_obj.set_fps_stats( 0.f );
		stats_obj.set_camera_stats( vec, vec );
		stats_obj.set_resources_stats( "" );

		graph.add_value( 0.f, 0.f );
		graph.set_time_interval( 0.f );
		graph.render( ui_renderer, scene_view, 0u, 0u, 0u, 0u, 0u );
		graph.stop_rendering( );
		graph.average_value( );

		// game's own public methods: address-take through a volatile sink so the
		// game vtable + members stay linked (game is abstract -> can't construct it).
		static pcvoid volatile s_sink = 0;
		void ( survarium::game::* const m_tick )( u32 )				= &survarium::game::tick;
		void ( survarium::game::* const m_enable )( bool )			= &survarium::game::enable;
		void ( survarium::game::* const m_clear )( )				= &survarium::game::clear_resources;
		void ( survarium::game::* const m_unload )( pcstr, bool )	= &survarium::game::unload;
		s_sink = *( pcvoid const* )&m_tick;
		s_sink = *( pcvoid const* )&m_enable;
		s_sink = *( pcvoid const* )&m_clear;
		s_sink = *( pcvoid const* )&m_unload;

		// generate_shaders_world: keep ctor + tick + generate_renderer_shaders +
		// generate_materials_shaders past /OPT:REF. volatile sink prevents LTCG
		// from proving the args constant / devirtualizing / inlining.
		static render::world* volatile					s_rw = 0;
		static survarium::generate_shaders_world* volatile	s_gs = 0;
		if ( s_rw )
		{
			survarium::generate_shaders_world gsw( *s_rw );
			gsw.tick( 0 );
		}
	}

	// Keep the object-skeleton base cone past /OPT:REF. game_object_static is
	// concrete (it overrides the pure game_object_::load), so constructing one keeps
	// its ctor + vtable + out-of-line load; reference load_transform directly. The
	// out-of-line game_object_::game_object_ base ctor is kept by a direct-derived
	// subclass whose ctor forwards to it (game_object_static inlines the base ctor, so
	// a separate direct subclass is needed to emit the out-of-line reference).
	struct anchor_game_object : survarium::game_object_
	{
		anchor_game_object( survarium::base_game_scene& s ) : survarium::game_object_( s ) { }
		virtual void load(
			configs::binary_config_value const&,
			pcstr,
			boost::function< void( survarium::game_object_& ) >& ) { }
	};

	void use_object_skeleton( )
	{
		static volatile bool s_run = false;
		if( !s_run )
			return;

		// source the placeholders through volatile pointers so LTCG cannot prove the
		// scene/config args are the constant null this anchor is the sole caller with
		// (a constant arg gets propagated into the ctor, dropping the m_game_scene =
		// s store and the s parameter - see object.cpp ctors).
		static survarium::base_game_scene* volatile			s_scene	= 0;
		static configs::binary_config_value const* volatile	s_cfg	= 0;
		static boost::function< void( survarium::game_object_& ) >* volatile	s_cb = 0;
		static float4x4* volatile							s_xf	= 0;

		survarium::base_game_scene&					scene	= *s_scene;
		configs::binary_config_value const&			cfg		= *s_cfg;
		boost::function< void( survarium::game_object_& ) >&	cb	= *s_cb;
		float4x4&									xf		= *s_xf;

		anchor_game_object				base_obj( scene );
		survarium::game_object_static	static_obj( scene );
		static_obj.load( cfg, "", cb );

		survarium::load_transform( cfg, xf );

		// the concrete object_* visual family derived from game_object_static; each
		// is constructed (ctor/dtor/vtable) and its load/insert/remove referenced.
		survarium::object_light			light( scene );
		light.load( cfg, "", cb );
		light.insert( );
		light.remove( );

		survarium::object_decal			decal( scene );
		decal.load( cfg, "", cb );
		decal.insert( );
		decal.remove( );

		survarium::object_wire			wire( scene );
		wire.load( cfg, "", cb );
		wire.insert( );
		wire.remove( );

		survarium::object_sky			sky( scene );
		sky.load( cfg, "", cb );
		sky.insert( );
		sky.remove( );

		survarium::object_environment	environment( scene );
		environment.load( cfg, "", cb );
		environment.insert( );
		environment.remove( );

		survarium::object_particle_visual	particle( scene );
		particle.load( cfg, "", cb );
		particle.insert( );
		particle.remove( );

		survarium::object_sound			sound_obj( scene );
		sound_obj.load( cfg, "", cb );
		sound_obj.insert( );
		sound_obj.remove( );

		// object_volumetric_sound derives object_sound + link_resolver; reference
		// its load / insert / resolve_links (the link_resolver virtual) too.
		survarium::object_volumetric_sound	volumetric_sound( scene );
		volumetric_sound.load( cfg, "", cb );
		volumetric_sound.insert( );
		volumetric_sound.resolve_links( static_cast< survarium::base_project* >( NULL ), cfg );

		// object_lpv_occluder: only load is matched (facade-free); insert/remove are
		// render-facade-walled stubs. Construct + reference all four so load pairs and
		// the vtable/ctor/dtor stay past /OPT:REF.
		survarium::object_lpv_occluder	lpv_occluder( scene );
		lpv_occluder.load( cfg, "", cb );
		lpv_occluder.insert( );
		lpv_occluder.remove( );

		// object_ambient_volume: only load is matched (facade-free); insert/remove are
		// render-facade-walled stubs (ambient_volume_properties + the scene_renderer
		// cook methods). Same construct + reference pattern.
		survarium::object_ambient_volume	ambient_volume( scene );
		ambient_volume.load( cfg, "", cb );
		ambient_volume.insert( );
		ambient_volume.remove( );

		// object_environment_probe: only ctor/dtor/load are matched (facade-free);
		// insert/remove are render-facade-walled stubs (environment_probe_properties +
		// scene_renderer cook methods). Same construct + reference pattern.
		survarium::object_environment_probe	environment_probe( scene );
		environment_probe.load( cfg, "", cb );
		environment_probe.insert( );
		environment_probe.remove( );

		// object_sky_ambient_occlusion: only ctor/dtor/load are matched (facade-free);
		// insert/remove are render-facade-walled stubs (sky_ambient_occlusion_properties +
		// scene_renderer cook methods). Same construct + reference pattern.
		survarium::object_sky_ambient_occlusion	sky_ambient_occlusion( scene );
		sky_ambient_occlusion.load( cfg, "", cb );
		sky_ambient_occlusion.insert( );
		sky_ambient_occlusion.remove( );

		// object_volume_fog: load + insert + remove all matched now that the render
		// volume_fog cook chain (volume_fog_parameters + scene_renderer cook methods)
		// is reconstructed. Same construct + reference pattern keeps it past /OPT:REF.
		survarium::object_volume_fog	volume_fog( scene );
		volume_fog.load( cfg, "", cb );
		volume_fog.insert( );
		volume_fog.remove( );
	}

	// menu non-virtual method reachability anchor (anchor_game_menus.cpp)
	void use_game_menus( );
	void use_game_weapons( );
	void use_game_booby_trap( );	// anchor_game_booby_trap.cpp
	void use_game_victory_item( );	// anchor_game_victory_item.cpp
	void use_game_small_utils( );	// anchor_game_small_utils.cpp

	// the player_cook carcass anchor (anchor_game_player_cook.cpp); self-guarded.
	void anchor_game( )
	{
		use_inventory( );
		use_engine_user_world_cone( );
		use_game_skeleton( );
		use_object_skeleton( );
		use_game_menus( );
		use_game_npc( );
		use_game_stats( );
		survarium::use_game_base( );
		survarium::use_game_world( );	// anchor_game_world.cpp (game-class public methods)
		use_game_weapons( );
		use_game_booby_trap( );
		use_game_victory_item( );
		use_game_small_utils( );
		use_game_animation( );
		survarium::use_game_player_logic( );
		survarium::use_game_options( );
		survarium::use_game_options_items( );
		survarium::use_game_lobby_ui( );
		survarium::use_game_lobby_scene( );
		survarium::use_game_lobby_camera( );
		survarium::use_game_fingers( );
		survarium::use_game_player_cook( );
		survarium::use_game_anomaly_veg( );
		survarium::use_game_global_input( );

		// drive the self-guarded network-client carcass anchor; the placeholder
		// game& is never dereferenced (the anchor returns before touching it).
		survarium::anchor_game_network_clients( *( survarium::game* )NULL );

		// keep the player tick/history + input-handler carcass objects linked.
		survarium::use_game_player_input( );
		// pin the survarium::player carcass symbols.
		survarium::anchor_game_player( );
		// keep the matched game_world_ui methods (no reachable caller yet).
		survarium::anchor_game_world_ui( );
		// drive the self-guarded chat-handler anchor (same self-guard contract).
		survarium::anchor_game_chat( *( survarium::game* )NULL );
		// drive the self-guarded game_world NPC-management anchor; the placeholder
		// game_world& is never dereferenced (the anchor returns before touching it).
		survarium::anchor_game_world_npc( *( survarium::game_world* )NULL );
	}
}
