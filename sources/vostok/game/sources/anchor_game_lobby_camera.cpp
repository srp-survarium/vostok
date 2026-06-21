////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for survarium::lobby_camera
// (lobby_camera.cpp). The lobby camera is created only by the lobby-scene call
// graph, which is still a stub, so /OPT:REF strips its out-of-line bodies from
// the base EXE and the delinker pairs nothing. use_game_lobby_camera()
// constructs a lobby_camera (self-guarded so the placeholder scene ref is never
// dereferenced) and references its public/private methods so the linker keeps
// the carcass object.
//
// Retire once the real lobby-scene call graph constructs the lobby camera itself.

#include "pch.h"

#include "lobby_camera.h"
#include "base_game_scene.h"
#include "camera_director.h"

namespace survarium {

void use_game_lobby_camera( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	static base_game_scene* volatile	s_scene		= 0;
	base_game_scene&					scene		= *s_scene;

	static input::world* volatile		s_world		= 0;
	static camera_director* volatile	s_director	= 0;
	static float3* volatile				s_vec		= 0;
	static float* volatile				s_dist		= 0;

	input::world&						world		= *s_world;
	camera_director&					director	= *s_director;
	float3 const&						vec			= *s_vec;
	float&								dist		= *s_dist;

	static volatile s32 v = 0;
	const s32 any_s32 = v;
	bool b = s_run;

	lobby_camera camera( scene );

	camera.on_keyboard_action( &world, ( input::enum_keyboard )any_s32, ( input::enum_keyboard_action )any_s32 );
	camera.on_gamepad_action( &world, ( input::gamepad_button )any_s32, ( input::enum_gamepad_action )any_s32 );
	camera.on_mouse_key_action( &world, ( input::mouse_button )any_s32, ( input::enum_mouse_key_action )any_s32 );
	camera.on_mouse_move( &world, any_s32, any_s32, any_s32 );
	camera.on_before_processing( &world, any_s32 );
	camera.on_after_processing( &world );
	camera.input_priority( );
	camera.on_activate( &director );
	camera.on_deactivate( );
	camera.on_focus( b );
	camera.tick( );
	camera.process_collision( vec, vec, dist );
}

} // namespace survarium
