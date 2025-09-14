////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_scene.h"
#include "game.h"
#include <vostok/render/facade/scene_renderer.h>

#include <vostok/physics/api.h>
#include <vostok/physics/world.h>

namespace survarium {

#ifdef VOSTOK_STATIC_LIBRARIES
extern vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	g_physics_allocator;
#endif // #ifdef VOSTOK_STATIC_LIBRARIES

game_scene::game_scene( game& g ) 
:m_is_active( false ),
m_physics_world	( NULL ),
m_game(g)
{
}

void game_scene::init_physics( )
{
#ifdef VOSTOK_STATIC_LIBRARIES
	m_physics_world					= vostok::physics::create_world_bt( survarium::g_physics_allocator.c_ptr(), *this );
#else // #ifdef VOSTOK_STATIC_LIBRARIES
	m_physics_world					= vostok::physics::create_world_bt( memory::g_crt_allocator, *this );
#endif // #ifdef VOSTOK_STATIC_LIBRARIES
	m_physics_world->initialize		( );
}

void game_scene::tick( )
{
	if(m_physics_world)
		m_physics_world->tick();
}

render::game::renderer& game_scene::renderer( )				
{ 
	return m_game.renderer(); 
}

void game_scene::on_activate( ) 
{ 
	m_is_active = true; 
#ifndef MASTER_GOLD
	LOG_INFO("scene [%s] is Activated", dbg_name() );
#endif //#ifndef MASTER_GOLD
}

void game_scene::on_deactivate( ) 
{ 
	m_is_active = false; 
#ifndef MASTER_GOLD
	LOG_INFO("scene [%s] is Deactivated", dbg_name() );
#endif //#ifndef MASTER_GOLD
}

void game_scene::apply_camera( camera_director_ptr cd )
{
	m_inverted_view_matrix			= cd->get_inverted_view_matrix();
	m_projection_matrix				= cd->get_projection_matrix();

	renderer().scene().set_view_matrix			( m_scene_view, math::invert4x3( m_inverted_view_matrix ) );
	renderer().scene().set_projection_matrix	( m_scene_view, m_projection_matrix );
}

} // namespace survarium