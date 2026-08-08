#include "pch.h"

// Game-module /OPT:REF reachability anchor for the survarium::base_game_scene
// base-class methods that live out-of-line in base_game_scene.cpp. base_game_scene
// is abstract (clear_resources stays pure-virtual) so it cannot be constructed;
// instead use_game_base() address-takes its public non-virtual methods through a
// volatile sink so the compiler emits each reference and /OPT:REF keeps the
// out-of-line bodies in the base EXE. The virtual methods (on_activate/on_deactivate/
// tick/on_after_tick) are kept by the derived-scene vtables already.
//
// Dispatched from anchor_game() (anchor_game.cpp). Retire once the real game call
// graph reaches these base methods for itself.

#include "base_game_scene.h"
#include "camera_director.h"

namespace survarium {

void use_game_base( )
{
	using namespace vostok;

	static pcvoid volatile s_sink = 0;

	scheduler&					( base_game_scene::* const m_scheduler )( )								= &base_game_scene::scheduler;
	swf_input_translator&		( base_game_scene::* const m_input )( )									= &base_game_scene::input_translator;
	render::scene_renderer&		( base_game_scene::* const m_scene_rndr )( ) const						= &base_game_scene::scene_renderer;
	math::uint2 const&			( base_game_scene::* const m_ows )( ) const								= &base_game_scene::output_window_size;
	void						( base_game_scene::* const m_init )( )									= &base_game_scene::init_physics;
	void						( base_game_scene::* const m_destroy )( )								= &base_game_scene::destroy_physics;
	bool						( base_game_scene::* const m_p2s )( float3 const&, float2& )			= &base_game_scene::point_to_screen;
	void						( base_game_scene::* const m_show_movie )( flash_movie_resource_ptr& )	= &base_game_scene::show_movie;
	void						( base_game_scene::* const m_hide_movie )( flash_movie_resource_ptr& )	= &base_game_scene::hide_movie;
	void						( base_game_scene::* const m_show_tm )( flash_text_manager* )			= &base_game_scene::show_text_manager;
	void						( base_game_scene::* const m_hide_tm )( flash_text_manager* )			= &base_game_scene::hide_text_manager;
	s_sink = *( pcvoid const* )&m_scheduler;
	s_sink = *( pcvoid const* )&m_input;
	s_sink = *( pcvoid const* )&m_scene_rndr;
	s_sink = *( pcvoid const* )&m_ows;
	s_sink = *( pcvoid const* )&m_init;
	s_sink = *( pcvoid const* )&m_destroy;
	s_sink = *( pcvoid const* )&m_p2s;
	s_sink = *( pcvoid const* )&m_show_movie;
	s_sink = *( pcvoid const* )&m_hide_movie;
	s_sink = *( pcvoid const* )&m_show_tm;
	s_sink = *( pcvoid const* )&m_hide_tm;
}

} // namespace survarium
