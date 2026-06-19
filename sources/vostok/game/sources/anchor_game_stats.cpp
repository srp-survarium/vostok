#include "pch.h"

// Game-module /OPT:REF reachability anchor for the debug-HUD stats cone
// (stats / stats_graph / npc_stats). The original game wires these from a dev
// HUD path that our base build does not reach, so /OPT:REF strips every
// unreferenced method. use_game_stats() constructs each object and references
// each addressed method through a self-guarded sink so the carcass survives the
// link without ever running. Driven by a single call line in anchor_game().
//
// Retire once the real dev-HUD call graph reaches these for itself.

#include "stats.h"
#include "stats_graph.h"
#include "npc_stats.h"
#include "human_npc.h"

namespace vostok
{
	void use_game_stats( )
	{
		static volatile bool s_run = false;
		if( !s_run )
			return;

		// source every reference/arg through a volatile pointer: a constant-NULL
		// arg lets LTCG drop it (a tail-call draw becomes push 0 / call), so the
		// matched body diverges from the target that passes live args.
		static ui::world* volatile						s_world		= 0;
		static render::ui::renderer* volatile			s_renderer	= 0;
		static render::scene_view_ptr const* volatile	s_view		= 0;
		static float3 const* volatile					s_vec		= 0;
		static survarium::human_npc const* volatile		s_npc		= 0;
		static pcstr volatile							s_str		= 0;

		ui::world&						ui_world	= *s_world;
		render::ui::renderer&			ui_renderer	= *s_renderer;
		render::scene_view_ptr const&	scene_view	= *s_view;
		float3 const&					vec			= *s_vec;
		survarium::human_npc const*		npc			= s_npc;
		pcstr							str			= s_str;

		// volatile-sourced ctor args so LTCG cannot prove them the constant 0 the
		// other anchor (use_game_skeleton) passes - a constant arg collapses the
		// whole stats_graph ctor body to zero stores. Real callers pass live values.
		static volatile float			s_f			= 0.f;
		static volatile u32				s_u			= 0u;
		survarium::stats_graph			graph( s_f, s_f, s_f, s_f, s_u );
		graph.add_value						( s_f, s_f );
		graph.set_time_interval				( s_f );
		graph.render						( ui_renderer, scene_view, s_u, s_u, s_u, s_u, s_u );
		graph.stop_rendering				( );
		graph.average_value					( );

		survarium::stats				stats_obj( ui_world );
		stats_obj.draw						( ui_renderer, scene_view );
		stats_obj.set_fps_stats				( s_f );
		stats_obj.set_player_linear_speed	( s_f );
		stats_obj.set_player_angular_speed	( s_f );
		stats_obj.set_camera_stats			( vec, vec );
		stats_obj.set_resources_stats		( str );
		stats_obj.set_crosshair_info		( s_f );

		survarium::npc_stats			npc_stats_obj( ui_world );
		npc_stats_obj.draw					( ui_renderer, scene_view );
		npc_stats_obj.set_stats				( npc );
	}
}
