////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "object_sky_ambient_occlusion.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/sky_ambient_occlusion_properties.h>
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

static u32	sky_ao_volume_ids	= 0;

 object_sky_ambient_occlusion::object_sky_ambient_occlusion( base_game_scene& w ) :
	game_object_static( w ),
	m_width( 512 ),
	m_depth( 512 ),
	m_height( 512 ),
	m_resolution_x( 256 ),
	m_resolution_y( 256 )
{
	m_sky_ao_volume_id = sky_ao_volume_ids++;
	m_enabled = true;
	m_generated = false;
}

 object_sky_ambient_occlusion::~object_sky_ambient_occlusion( )
{
}

void object_sky_ambient_occlusion::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform( t, m_transform );

	m_generated = false;
	m_width = t["width"];
	m_height			= t["height"];
	m_depth				= t["depth"];
	m_enabled			= t["enabled"];
	m_resolution_x		= t["resolution_x"];
	m_resolution_y		= t["resolution_y"];
	m_texture_name		= pcstr( t["texture"] );

	cb( *this );
}

void object_sky_ambient_occlusion::insert( )
{
	render::sky_ambient_occlusion_properties properties;
	properties.location				= m_transform.c.xyz( );
	properties.width				= (float)m_width;
	properties.height				= (float)m_height;
	properties.depth				= (float)m_depth;
	properties.enabled				= m_enabled;

	properties.texture_name			= m_texture_name;
	properties.texture_invalidated	= true;
	get_game_scene( ).renderer( ).scene( ).update_sky_ambient_occlusion( get_game_scene( ).render_scene( ), m_sky_ao_volume_id, properties );
}

void object_sky_ambient_occlusion::remove( )
{
	get_game_scene( ).renderer( ).scene( ).remove_sky_ambient_occlusion( get_game_scene( ).render_scene( ), m_sky_ao_volume_id );
}

} // namespace survarium
