// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "object_wire.h"
#include "game_object_static.h"
#include "base_game_scene.h"
#include "game.h"
#include <vostok/math_float4x4.h>
#include <vostok/configs_binary_config_value.h>
#include <vostok/resources.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_query_result.h>
#include <vostok/memory_writer.h>
#include <vostok/render/engine/model_format.h>
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>

namespace survarium {

void load_transform( configs::binary_config_value const& t, float4x4& dest );

object_wire::object_wire( base_game_scene& w ) :
	game_object_static( w ),
	m_points( NULL )
{
}

object_wire::~object_wire( )
{
	VOSTOK_FREE_IMPL( *::survarium::g_allocator, m_points );
}

static void create_wire_visual_source(
	memory::writer&		writer,
	float3*				points,
	u32					points_count,
	pcstr				material_name,
	float				wire_width
)
{
	render::model_header	hdr;

	hdr.platform_id			= render::platform_uni;
	hdr.bb.min				= float3( -1, -1, -1 );
	hdr.bb.max				= float3( 1, 1, 1 );
	hdr.type				= render::mt_user_mesh_wire;

	writer.open_chunk		( render::model_chunk_header );
	writer.write			( &hdr, sizeof( hdr ) );
	writer.close_chunk		( );

	writer.open_chunk		( render::model_texture );
	writer.write_string		( material_name );
	writer.write_float		( wire_width );
	writer.close_chunk		( );

	writer.open_chunk		( render::model_chunk_vertices );

	writer.write_u32		( points_count );

	for ( u32 i = 0; i < points_count; ++i )
		writer.write_float3( points[i] );

	writer.close_chunk		( );
}

// claude@NOTE: The 22-statement source shape matches. The residual is a shared-template
// /GL cut around creation_request and the Boost callback exception tail; reopen only
// after the compiler context or function-scoped MAX attribution changes.
void object_wire::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	load_transform	( t, m_transform );
	m_wire_width	= t["wire_width"];
	m_points_count	= t["points_count"];

	if ( m_points_count != 0 )
	{
		m_points		= ALLOC( float3, m_points_count );
		float3* p		= m_points;
		for ( u32 i = 0; i < m_points_count; ++i )
		{
			float3 pt	= t["points"][i];
			*p			= pt;
			++p;
		}
	}

	memory::writer	writer( g_allocator );
	writer.external_data	= true;

	pcstr material_name = "editor/wire";

	if ( m_points_count && m_points )
	{
		create_wire_visual_source( writer, m_points, m_points_count, material_name, m_wire_width );

		const_buffer creation_buffer( writer.pointer(), writer.size() );
		fixed_string< 32 > wire_name;
		wire_name.assignf( "wire_%X", this );

		resources::creation_request r[] =
		{
			resources::creation_request( wire_name.c_str(), creation_buffer, resources::user_mesh_class ),
		};

		resources::query_create_resources(
			r,
			boost::bind( &object_wire::resources_ready, this, _1, cb ),
			g_allocator
		);
	}
	else
	{
		LOG_WARNING( "empty wire" );
	}
}

void object_wire::insert( )
{
	if ( m_visual )
		get_game_scene().renderer().scene().add_model( get_game_scene().render_scene(), m_visual, m_transform );
}

void object_wire::remove( )
{
	if ( m_visual )
		get_game_scene().renderer().scene().remove_model( get_game_scene().render_scene(), m_visual );
}

// claude@NOTE: DELETE(pbyte) remains out-of-line through get_top_pointer/destructor
// helpers in base but reduces to the target's direct mspace_free. The following resource
// pointer assignment aligns; reopen after the shared memory-helper inline context changes.
void object_wire::resources_ready( resources::queries_result& data, boost::function< void( game_object_& ) >& cb )
{
	const_buffer user_data_to_create	= data[0].creation_data_from_user();
	pbyte creation_data					= (pbyte)user_data_to_create.c_ptr();
	DELETE								( creation_data );

	m_visual							= static_cast_resource_ptr< render::render_model_instance_ptr >( data[0].get_unmanaged_resource() );

	cb( *this );
}

} // namespace survarium
