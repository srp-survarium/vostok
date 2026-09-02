// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "stats.h"
#include <vostok/ui/ui.h>
#include <vostok/ui/world.h>

namespace survarium {

stats::stats( ui::world& ui_world ) :
	m_ui_world( ui_world ),
	m_crosshair_dist( 0.f ),
	m_odd_row_color( 0xff80ffff ),
	m_even_row_color( 0xffffff80 )
{
	create	( );
}

stats::~stats( )
{
	m_ui_world.destroy_window( m_main_window );
}

void stats::draw( render::ui::renderer& w, render::scene_view_ptr const& scene_view )
{
	m_main_window->draw( w, scene_view );
	m_dispersion_components->w()->set_visible( false );
}

void stats::set_fps_stats( const float fps )
{
	string64		buff;
	vostok::sprintf	( buff, "FPS: %3.2f", fps );
	m_fps->set_text	( buff );
}

void stats::set_player_angular_speed( const float speed )
{
	string64		buff;
	vostok::sprintf	( buff, "angular speed: %3.2f", speed );
	m_player_angular_speed->set_text( buff );
}

void stats::set_player_linear_speed( const float speed )
{
	string64		buff;
	vostok::sprintf	( buff, "linear speed: %3.2f", speed );
	m_player_linear_speed->set_text( buff );
}

void stats::set_camera_stats( float3 const& pos, float3 const& dir )
{
	string64		buff1;
	vostok::sprintf	( buff1, "camera position: %3.2f %3.2f %3.2f", pos.x, pos.y, pos.z );
	m_camera_position->set_text	( buff1 );

	string64		buff2;
	vostok::sprintf	( buff2, "camera direction: %3.2f %3.2f %3.2f", dir.x, dir.y, dir.z );
	m_camera_direction->set_text( buff2 );

	string64		buff3;
	vostok::sprintf	( buff3, "crosshair distance: %3.1fm", m_crosshair_dist );
	m_crosshair_distance->set_text( buff3 );
}

void stats::set_crosshair_info( const float dist )
{
	m_crosshair_dist				= dist;
}

void stats::set_resources_stats( pcstr str )
{
	m_resources_activity->set_text	( str );
}

// sushi@NOTE: Retail hoists the final weapon-position loads across create_text;
// keeping the reference here preserves the target statement order.
void stats::create( )
{
	m_main_window						= m_ui_world.create_window();
	m_main_window->set_visible			( true );
	m_main_window->set_position			( float2( 0.f, 0.f ) );
	m_main_window->set_size				( float2( 1280.f, 720.f ) );

	m_fps								= m_ui_world.create_text();
	m_fps->w()->set_visible				( true );
	m_fps->w()->set_position			( float2( 0.f, 0.f ) );
	m_fps->w()->set_size				( float2( 50.f, 20.f ) );
	m_fps->set_font						( vostok::ui::fnt_arial );
	m_fps->set_text_mode				( vostok::ui::tm_default );
	m_fps->set_color					( m_odd_row_color );
	m_main_window->add_child			( m_fps->w(), true );

	m_camera_position					= m_ui_world.create_text();
	m_camera_position->w()->set_visible	( true );
	m_camera_position->w()->set_position( float2( 0.f, 20.f ) );
	m_camera_position->w()->set_size	( float2( 100.f, 20.f ) );
	m_camera_position->set_font			( vostok::ui::fnt_arial );
	m_camera_position->set_text_mode	( vostok::ui::tm_default );
	m_camera_position->set_color		( m_even_row_color );
	m_main_window->add_child			( m_camera_position->w(), true );

	m_camera_direction					= m_ui_world.create_text();
	m_camera_direction->w()->set_visible( true );
	m_camera_direction->w()->set_position( float2( 0.f, 40.f ) );
	m_camera_direction->w()->set_size	( float2( 100.f, 20.f ) );
	m_camera_direction->set_font		( vostok::ui::fnt_arial );
	m_camera_direction->set_text_mode	( vostok::ui::tm_default );
	m_camera_direction->set_color		( m_odd_row_color );
	m_main_window->add_child			( m_camera_direction->w(), true );

	m_crosshair_distance				= m_ui_world.create_text();
	m_crosshair_distance->w()->set_visible( true );
	m_crosshair_distance->w()->set_position( float2( 0.f, 60.f ) );
	m_crosshair_distance->w()->set_size	( float2( 50.f, 20.f ) );
	m_crosshair_distance->set_font		( vostok::ui::fnt_arial );
	m_crosshair_distance->set_text_mode	( vostok::ui::tm_default );
	m_crosshair_distance->set_color		( m_even_row_color );
	m_main_window->add_child			( m_crosshair_distance->w(), true );

	m_resources_activity				= m_ui_world.create_text();
	m_resources_activity->w()->set_visible( true );
	m_resources_activity->w()->set_position( float2( 0.f, 80.f ) );
	m_resources_activity->w()->set_size	( float2( 100.f, 20.f ) );
	m_resources_activity->set_font		( vostok::ui::fnt_arial );
	m_resources_activity->set_text_mode	( vostok::ui::tm_default );
	m_resources_activity->set_color		( m_odd_row_color );
	m_main_window->add_child			( m_resources_activity->w(), true );

	m_active_scene_info					= m_ui_world.create_text();
	m_active_scene_info->w()->set_visible( true );
	m_active_scene_info->w()->set_position( float2( 0.f, 100.f ) );
	m_active_scene_info->w()->set_size	( float2( 300.f, 40.f ) );
	m_active_scene_info->set_font		( vostok::ui::fnt_arial );
	m_active_scene_info->set_text_mode	( vostok::ui::tm_multiline );
	m_active_scene_info->set_color		( m_even_row_color );
	m_main_window->add_child			( m_active_scene_info->w(), true );

	m_player_angular_speed				= m_ui_world.create_text();
	m_player_angular_speed->w()->set_visible( true );
	m_player_angular_speed->w()->set_position( float2( 0.f, 120.f ) );
	m_player_angular_speed->w()->set_size( float2( 50.f, 20.f ) );
	m_player_angular_speed->set_font	( vostok::ui::fnt_arial );
	m_player_angular_speed->set_text_mode( vostok::ui::tm_default );
	m_player_angular_speed->set_color	( m_odd_row_color );
	m_main_window->add_child			( m_player_angular_speed->w(), true );

	m_player_linear_speed				= m_ui_world.create_text();
	m_player_linear_speed->w()->set_visible( true );
	m_player_linear_speed->w()->set_position( float2( 0.f, 140.f ) );
	m_player_linear_speed->w()->set_size( float2( 50.f, 20.f ) );
	m_player_linear_speed->set_font		( vostok::ui::fnt_arial );
	m_player_linear_speed->set_text_mode( vostok::ui::tm_default );
	m_player_linear_speed->set_color	( m_odd_row_color );
	m_main_window->add_child			( m_player_linear_speed->w(), true );

	m_player_stamina					= m_ui_world.create_text();
	m_player_stamina->w()->set_visible	( true );
	m_player_stamina->w()->set_position	( float2( 0.f, 160.f ) );
	m_player_stamina->w()->set_size		( float2( 50.f, 20.f ) );
	m_player_stamina->set_font			( vostok::ui::fnt_arial );
	m_player_stamina->set_text_mode		( vostok::ui::tm_default );
	m_player_stamina->set_color			( m_even_row_color );
	m_main_window->add_child			( m_player_stamina->w(), true );

	m_can_use							= m_ui_world.create_text();
	m_can_use->w()->set_visible			( true );
	m_can_use->w()->set_position		( float2( 0.f, 220.f ) );
	m_can_use->w()->set_size			( float2( 50.f, 20.f ) );
	m_can_use->set_font					( vostok::ui::fnt_arial );
	m_can_use->set_text_mode			( vostok::ui::tm_default );
	m_can_use->set_color				( m_odd_row_color );
	m_main_window->add_child			( m_can_use->w(), true );

	m_player_logic_state				= m_ui_world.create_text();
	m_player_logic_state->w()->set_visible( true );
	m_player_logic_state->w()->set_position( float2( 0.f, 180.f ) );
	m_player_logic_state->w()->set_size	( float2( 50.f, 20.f ) );
	m_player_logic_state->set_font		( vostok::ui::fnt_arial );
	m_player_logic_state->set_text_mode	( vostok::ui::tm_default );
	m_player_logic_state->set_color		( m_even_row_color );
	m_main_window->add_child			( m_player_logic_state->w(), true );

	m_weapon_logic_state				= m_ui_world.create_text();
	m_weapon_logic_state->w()->set_visible( true );
	m_weapon_logic_state->w()->set_position( float2( 0.f, 200.f ) );
	m_weapon_logic_state->w()->set_size	( float2( 50.f, 20.f ) );
	m_weapon_logic_state->set_font		( vostok::ui::fnt_arial );
	m_weapon_logic_state->set_text_mode	( vostok::ui::tm_default );
	m_weapon_logic_state->set_color		( m_odd_row_color );
	m_main_window->add_child			( m_weapon_logic_state->w(), true );

	float2 const& weapon_pos			= m_weapon_logic_state->w()->get_position();
	m_dispersion_components				= m_ui_world.create_text();
	m_dispersion_components->w()->set_visible( true );
	m_dispersion_components->w()->set_position( float2( weapon_pos.x, weapon_pos.y + 20.f ) );
	m_dispersion_components->w()->set_size( float2( 50.f, 20.f ) );
	m_dispersion_components->set_font	( vostok::ui::fnt_arial );
	m_dispersion_components->set_text_mode( vostok::ui::tm_default );
	m_dispersion_components->set_color	( m_even_row_color );
	m_main_window->add_child			( m_dispersion_components->w(), true );
}

} // namespace survarium
