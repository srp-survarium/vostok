////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "stats.h"
#include <vostok/ui/ui.h>
#include <vostok/ui/world.h>

using survarium::stats;

// set_active_scene: canonical is an inline no-source stub (m_active_scene_info
// not in this batch's port list) - left for manual review.
void stats::set_active_scene( pcstr scene_name )
{
	string64		buff;
	vostok::sprintf	(buff, "active scene: %s", scene_name );
	m_active_scene_info->set_text( buff );
}

// create: NOT HARVESTED. Structural divergence - the canonical stats grew many
// more widgets (m_crosshair_distance / m_player_*_speed / m_player_stamina /
// m_can_use / m_dispersion_components / m_player_logic_state /
// m_weapon_logic_state), so the carcass create() body (125 lines) is far larger
// than this 5-widget legacy. Needs manual review.
void stats::create()
{
	m_main_window						= m_ui_world.create_window();
	m_main_window->set_visible			(true);
	m_main_window->set_position			(float2(0.0f, 0.0f));
	m_main_window->set_size				(float2(1024.0f, 768.0f));

	m_fps								= m_ui_world.create_text();
	m_fps->w()->set_visible				(true);
	m_fps->w()->set_position			(float2(0.0f, 0.0f));
	m_fps->w()->set_size				(float2(50.0f, 20.0f));
	m_fps->set_font						(vostok::ui::fnt_arial);
	m_fps->set_text_mode				(vostok::ui::tm_default);
	m_fps->set_color					(0xffffffff);
	m_main_window->add_child			(m_fps->w(), true);

	m_camera_position					= m_ui_world.create_text();
	m_camera_position->w()->set_visible	(true);
	m_camera_position->w()->set_position(float2(0.0f, 20.0f));
	m_camera_position->w()->set_size	(float2(100.0f, 20.0f));
	m_camera_position->set_font			(vostok::ui::fnt_arial);
	m_camera_position->set_text_mode	(vostok::ui::tm_default);
	m_camera_position->set_color		(0xffffffff);
	m_main_window->add_child			(m_camera_position->w(), true);

	m_camera_direction					= m_ui_world.create_text();
	m_camera_direction->w()->set_visible(true);
	m_camera_direction->w()->set_position(float2(0.0f, 40.0f));
	m_camera_direction->w()->set_size	(float2(100.0f, 20.0f));
	m_camera_direction->set_font		(vostok::ui::fnt_arial);
	m_camera_direction->set_text_mode	(vostok::ui::tm_default);
	m_camera_direction->set_color		(0xffffffff);
	m_main_window->add_child			(m_camera_direction->w(), true);

	m_resources_activity					= m_ui_world.create_text();
	m_resources_activity->w()->set_visible(true);
	m_resources_activity->w()->set_position(float2(0.0f, 60.0f));
	m_resources_activity->w()->set_size	(float2(100.0f, 20.0f));
	m_resources_activity->set_font		(vostok::ui::fnt_arial);
	m_resources_activity->set_text_mode	(vostok::ui::tm_default);
	m_resources_activity->set_color		(0xffffffff);
	m_main_window->add_child			(m_resources_activity->w(), true);


	m_active_scene_info						= m_ui_world.create_text();
	m_active_scene_info->w()->set_visible	(true);
	m_active_scene_info->w()->set_position	(float2(0.0f, 80.0f));
	m_active_scene_info->w()->set_size		(float2(300.0f, 40.0f));
	m_active_scene_info->set_font			(vostok::ui::fnt_arial);
	m_active_scene_info->set_text_mode		(vostok::ui::tm_multiline);
	m_active_scene_info->set_color			(0xffffffff);
	m_main_window->add_child				(m_active_scene_info->w(), true);

}
