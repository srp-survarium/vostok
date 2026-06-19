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
#include <vostok/rtp/api.h>
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
#include "key_binder.h"
#include "lobby_menu.h"
#include "main_menu.h"
#include "free_fly_camera.h"
#include "camera_director.h"
#include "game_camera.h"
#include "stats.h"
#include "stats_graph.h"

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
#include "object_volumetric_sound.h"
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

	// file-local free function defined in object.cpp (no public header); the
	// game_object_static::load impl calls it.
	void load_transform( configs::binary_config_value const& t, float4x4& dest );

	// out-of-line base_game_scene base methods (anchor_game_base.cpp)
	void use_game_base( );
}

namespace vostok
{
	// NPC / animation cluster reachability anchor (anchor_game_npc.cpp).
	void use_game_npc( );

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

		ui::world* ui_world = ui::create_world( *( ui::engine* )NULL, *( render::ui::renderer* )NULL, *( memory::base_allocator* )NULL );
		ui::destroy_world( ui_world );

		animation::world* animation_world = animation::create_world( *( animation::engine* )NULL, NULL, NULL );
		animation::destroy_world( animation_world );
		animation::set_memory_allocator( *( animation::allocator_type* )NULL );

		input::world* input_world = input::create_world( *( input::engine* )NULL, NULL );
		input::destroy_world( input_world );
		input::set_memory_allocator( *( input::allocator_type* )NULL );

		rtp::world* rtp_world = rtp::create_world( *( rtp::engine* )NULL, *( animation::world* )NULL, *( render::scene_ptr const* )NULL, NULL, NULL );
		rtp::destroy_world( rtp_world );
		rtp::set_memory_allocator( *( rtp::allocator_type* )NULL );

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
		survarium::main_menu			main( g );
		survarium::camera_director		director( scene );
		survarium::game_camera			camera( scene );
		survarium::free_fly_camera		fly( scene, director );
		survarium::stats				stats_obj( *( ui::world* )NULL );
		survarium::stats_graph			graph( 0.f, 0.f, 0.f, 0.f, 0u );
		// project_cooker_simple is intentionally NOT constructed: its delete_resource
		// override has no body in our sources, so emitting its vtable would not link
		// (LNK2001). Its ~4 fns are a tiny residual the cook-matching phase recovers.

		// call the public non-virtual methods too (construction alone keeps only the
		// ctor/dtor/vtable - the target carries these out-of-line, so reference them).
		render::ui::renderer&			ui_renderer	= *( render::ui::renderer* )NULL;
		render::scene_view_ptr const&	scene_view	= *( render::scene_view_ptr const* )NULL;
		float3 const&					vec			= *( float3 const* )NULL;

		scene.unload( );
		scene.empty( );

		binder.set_default_controls( );
		binder.unbind_key( "", 0 );

		director.switch_to_camera( &camera, "" );
		director.tick( );
		director.apply( );

		camera.set_position_direction( vec, vec );

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
		void ( survarium::game::* const m_toggle )( )				= &survarium::game::toggle_console;
		void ( survarium::game::* const m_lobby )( )				= &survarium::game::switch_to_lobby;
		void ( survarium::game::* const m_clobby )( )				= &survarium::game::create_lobby_menu;
		void ( survarium::game::* const m_clogin )( )				= &survarium::game::create_login_menu;
		s_sink = *( pcvoid const* )&m_tick;
		s_sink = *( pcvoid const* )&m_enable;
		s_sink = *( pcvoid const* )&m_clear;
		s_sink = *( pcvoid const* )&m_unload;
		s_sink = *( pcvoid const* )&m_toggle;
		s_sink = *( pcvoid const* )&m_lobby;
		s_sink = *( pcvoid const* )&m_clobby;
		s_sink = *( pcvoid const* )&m_clogin;
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

		// object_volumetric_sound derives object_sound + link_resolver; reference
		// its load / insert / resolve_links (the link_resolver virtual) too.
		survarium::object_volumetric_sound	volumetric_sound( scene );
		volumetric_sound.load( cfg, "", cb );
		volumetric_sound.insert( );
		volumetric_sound.resolve_links( static_cast< survarium::base_project* >( NULL ), cfg );
	}

	// menu non-virtual method reachability anchor (anchor_game_menus.cpp)
	void use_game_menus( );

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

		// drive the self-guarded network-client carcass anchor; the placeholder
		// game& is never dereferenced (the anchor returns before touching it).
		survarium::anchor_game_network_clients( *( survarium::game* )NULL );
	}
}
