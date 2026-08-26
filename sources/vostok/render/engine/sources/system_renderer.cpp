#include "pch.h"
#include "system_renderer.h"

#include <vostok/particle/world.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/facade/material_effects_instance_cook_data.h>
#include <vostok/render/facade/render_stage_types.h>

#include "aabb_indices.h"
#include "effect_editor_model_ghost.h"
#include "effect_editor_selection.h"
#include "effect_particle_selection.h"
#include "effect_speedtree_selection.h"
#include "effect_system_colored.h"
#include "effect_system_line.h"
#include "effect_system_ui.h"
#include "effect_wireframe_colored.h"
#include "render_output_window.h"
#include "render_particle_emitter_instance.h"
#include "render_surface.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene.h"
#include "stage_particles.h"

namespace vostok {
namespace render {

struct vertex_colored_sl {
	float3	position;
	u32		color;
	float3	begin_line_position;
	float	shift;
	u32		pattern;
};

STATIC_SIZE_ASSERT( vertex_colored_sl, 0x24 );

struct screen_vertex {
	float4 position;
	float2 tc;

	void set( float4 const& in_position, float2 const& in_tc )
	{
		position	= in_position;
		tc			= in_tc;
	}
};

STATIC_SIZE_ASSERT( screen_vertex, 0x18 );

//TODO: not here
const D3D_INPUT_ELEMENT_DESC F_L_sl[] =
{
	{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 12,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	1, DXGI_FORMAT_R32_FLOAT,		0, 28,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	2, DXGI_FORMAT_R32_UINT,		0, 32,	D3D_INPUT_PER_VERTEX_DATA, 0},
};

untyped_buffer* system_renderer::create_quad_ib( )
{

	const u32 quad_count	= 4 * 1024;
	const u32 idx_count		= quad_count * 2 * 3;

	u16	indices[idx_count];

	int		vertex_id	= 0;
	int		idx			= 0;
	for ( int i = 0; i < quad_count; ++i )
	{
		indices[idx++] = u16( vertex_id + 0 );
		indices[idx++] = u16( vertex_id + 1 );
		indices[idx++] = u16( vertex_id + 2 );

		indices[idx++] = u16( vertex_id + 3 );
		indices[idx++] = u16( vertex_id + 2 );
		indices[idx++] = u16( vertex_id + 1 );

		vertex_id += 4;
	}

	return resource_manager::ref( ).create_buffer( idx_count * sizeof( u16 ), indices, enum_buffer_type_index, false, false );

}

system_renderer::system_renderer( renderer_context* renderer_context ) :
	m_renderer_context			( renderer_context ),
	m_vertex_stream				( 1024 * 1024 ),
	m_index_stream				( 1024 * 1024 ),
	m_vertex_stream_quad		( 1024 ),
	m_index_stream_quad			( 1024 ),
	m_grid_mode					( false ),
	m_color_write				( true )
{
	ASSERT						( m_renderer_context );

	m_renderer_context->m_quad_ib			= create_quad_ib( );
	m_renderer_context->m_g_quad_uv			= resource_manager::ref( ).create_geometry( vertex_formats::F_TL, array_size( vertex_formats::F_TL ), sizeof( vertex_formats::TL ), backend::ref( ).vertex.buffer( ), *m_renderer_context->m_quad_ib );
	m_renderer_context->m_g_quad_2uv		= resource_manager::ref( ).create_geometry( vertex_formats::F_TL2uv, array_size( vertex_formats::F_TL2uv ), sizeof( vertex_formats::TL2uv ), backend::ref( ).vertex.buffer( ), *m_renderer_context->m_quad_ib );
	m_renderer_context->m_g_quad_eye_ray	= resource_manager::ref( ).create_geometry( vertex_formats::F_Tquad, array_size( vertex_formats::F_Tquad ), sizeof( vertex_formats::Tquad ), backend::ref( ).vertex.buffer( ), *m_renderer_context->m_quad_ib );

#ifndef MASTER_GOLD
	m_colored_geom		= resource_manager::ref( ).create_geometry( vertex_formats::F_L, array_size( vertex_formats::F_L ), sizeof( vertex_formats::L ), m_vertex_stream.buffer( ), m_index_stream.buffer( ) );
#endif // #ifndef MASTER_GOLD

	m_colored_geom_sl	= resource_manager::ref( ).create_geometry( F_L_sl, array_size( F_L_sl ), sizeof( vertex_colored_sl ), m_vertex_stream.buffer( ), m_index_stream.buffer( ) );

	m_selection_color = math::float4( 0.0f, 0.0f, 0.5f, 1.0f );
	m_selection_rate = 1.0f;
	m_current_selection_color = math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
#ifndef MASTER_GOLD
	m_ghost_model_color = math::float4( 0.2f, 0.2f, 0.2f, 0.2f );
#endif // #ifndef MASTER_GOLD
	effect_manager::ref( ).create_effect< effect_system_colored >( &m_sh_vcolor );

	effect_manager::ref( ).create_effect< effect_system_line >( &m_sh_sl );
	m_grid_density_constant		= backend::ref( ).register_constant_host( "grid_density", rc_float );
	m_ui_geom					= resource_manager::ref( ).create_geometry( vertex_formats::F_TL, array_size( vertex_formats::F_TL ), sizeof( vertex_formats::TL ), m_vertex_stream.buffer( ), *m_renderer_context->m_quad_ib );

	pvoid buffer				= ALLOCA( 1 * Kb );
	effect_options_descriptor	desc( buffer, 1 * Kb );
	desc["ui_texture0"]			= "ui/ui_font_arial_21_1024";
	desc["ui_texture1"]			= "ui/ui_skull";

	effect_manager::ref( ).create_effect< effect_system_ui >( &m_sh_ui, desc );
	m_WVP_sl					= backend::ref( ).register_constant_host( "m_WVP_sl", rc_float );

	// Setup selection shader and color constant.

	resource_manager::ref( ).register_constant_binding( shader_constant_binding( "selection_color", &m_current_selection_color ) );
	effect_manager::ref( ).create_effect< effect_wireframe_colored >( &m_notexture_shader );

	effect_manager::ref( ).create_effect< effect_speedtree_selection >( &m_speedtree_selection_shader );

	for ( u32 i = 0; i < num_vertex_input_types; i++ )
	{
		if ( i == post_process_vertex_input_type )
			continue;

		u8 data[Kb];
		effect_options_descriptor desc( data, Kb );
		desc["vertex_input_type"] = ( enum_vertex_input_type )i;

		effect_manager::ref( ).create_effect< effect_editor_selection >( &m_editor_selection_shader[i], desc );
	}

	effect_manager::ref( ).create_effect< effect_editor_model_ghost >( &m_editor_model_ghost_shader );

	effect_manager::ref( ).create_effect< effect_particle_selection >( &m_sh_particle_selection );

	m_c_start_corner = backend::ref( ).register_constant_host( "start_corner", rc_float );
	m_cook_data_to_delete = MT_NEW( material_effects_instance_cook_data )(
		post_process_vertex_input_type,
		NULL,
		false
	);
	D3D_INPUT_ELEMENT_DESC const screen_vertex_layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D_INPUT_PER_VERTEX_DATA, 0 },
	};

	u16 indices[6] = { 0, 1, 2, 3, 2, 1 };
	m_screen_vertex_ib = resource_manager::ref( ).create_buffer( 6 * sizeof( u16 ), indices, enum_buffer_type_index, false, false );
	m_screen_vertex_geometry = resource_manager::ref( ).create_geometry(
		screen_vertex_layout,
		sizeof( screen_vertex ),
		m_vertex_stream_quad.buffer( ),
		*m_screen_vertex_ib
	);
}

bool system_renderer::is_effects_ready( ) const
{
	for ( u32 i = 0; i < num_vertex_input_types; i++ )
	{
		if ( i == post_process_vertex_input_type )
			continue;

		if ( m_editor_selection_shader[i].c_ptr( ) == NULL )
			return false;
	}

	return
		   m_sh_vcolor.c_ptr( ) != NULL
		&& m_sh_ui.c_ptr( ) != NULL
		&& m_sh_sl.c_ptr( ) != NULL
		&& m_editor_model_ghost_shader.c_ptr( ) != NULL
		&& m_speedtree_selection_shader.c_ptr( ) != NULL
		&& m_notexture_shader.c_ptr( ) != NULL
		&& m_sh_particle_selection.c_ptr( ) != NULL;

}

system_renderer::~system_renderer( )
{
}

void system_renderer::set_w( float4x4 const& m )
{
	m_renderer_context->set_w( m );

}

void system_renderer::set_v( float4x4 const& m )
{
	m_renderer_context->set_v(m);
}

void system_renderer::set_p( float4x4 const& m )
{
	m_renderer_context->set_p(m);
}

float4x4 const& system_renderer::get_w( ) const
{
	return m_renderer_context->get_w();
}

float4x4 const& system_renderer::get_v( ) const
{
	return m_renderer_context->get_v();
}

float4x4 const& system_renderer::get_p( ) const
{
	return m_renderer_context->get_p();
}

void system_renderer::draw_lines(
	vertex_colored const* const		vertices_begin,
	vertex_colored const* const		vertices_end,
	u16 const* const				indices_begin,
	u16 const* const				indices_end,
	bool							covering_effect
)
{
	if ( !is_effects_ready( ) )
		return;

	R_ASSERT_CMP( vertices_begin, <, vertices_end );
	R_ASSERT_CMP( indices_begin,  <, indices_end );

	// Prepare vertex buffer.
	vertex_colored* vbuffer;
	u32 voffset;
	u32 const vertices_size	= vertices_end - vertices_begin;
	m_vertex_stream.lock	( vertices_size, &vbuffer, voffset );
	memory::copy			( vbuffer, vertices_size * sizeof( vertex_colored ), vertices_begin, vertices_size * sizeof( vertex_colored ) );
	m_vertex_stream.unlock	( );

	// Prepare index buffer
	u32 ioffset;
	u32 const indices_size	= indices_end - indices_begin;
	u16* ibuffer			= (u16*)m_index_stream.lock( indices_size, ioffset );
	memory::copy			( ibuffer, indices_size * sizeof( u16 ), indices_begin, indices_size * sizeof( u16 ) );
	m_index_stream.unlock	( );
	m_colored_geom->apply	( );

	if ( covering_effect )
		m_sh_vcolor->apply	( effect_system_colored::cover, 0 );
	else
		m_sh_vcolor->apply	( 0, 0 );

	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_LINELIST, indices_size, ioffset /* ? */, voffset );
}

static float2 clip_2_screen(
	float3 const&		world_pixel,
	float4x4 const&		wvpMatrix,
	u32					screen_width,
	u32					screen_height
)
{
	float4 result = wvpMatrix.transform( float4( world_pixel, 1.0f ) );

	if ( math::is_zero( result.elements[3], math::epsilon_5 ) )
		result.elements[3]	= math::epsilon_3;

	result = result * ( 1.0f / result.elements[3] );

	result.elements[0] *= 0.5f;
	result.elements[1] *= -0.5f;

	result = result + float4( 0.5f, 0.5f, 0.0f, 0.0f );

	result.elements[0] *= screen_width;
	result.elements[1] *= screen_height;

	return float2( result.elements[0], result.elements[1] );

}

static u8 pattern_length = 8;

static float frac( float f )
{
	return		( f - static_cast< int >( f ) );
}

static u8 calc_pattern( float2 const& begin, float2 const& end )
{
	return static_cast_checked< u8 >( frac( math::max( fabsf( end.elements[0] - begin.elements[0] ), fabsf( end.elements[1] - begin.elements[1] ) ) / (float)pattern_length ) * ( (float)pattern_length ) );
}

void system_renderer::draw_screen_lines(
	float3 const*			points,
	u32						count,
	math::color const&		color,
	float					width,
	u32						pattern,
	bool					use_depth,
	bool					is_screen_space_coord
)
{

	if ( !is_effects_ready( ) )
		return;

	ASSERT( count > 0 );

	VOSTOK_UNREFERENCED_PARAMETER( width );

	// Vertices already in world space.
	float4x4 wvpMatrix = is_screen_space_coord ? float4x4( ).identity( ) : math::mul4x4( m_renderer_context->get_v( ), m_renderer_context->get_p( ) );

	u32 screen_width	= backend::ref( ).target_width( );
	u32 screen_height	= backend::ref( ).target_height( );

	u32 u32_color		= color.m_value;
	u32 vertex_count	= count * 2 - 2;

	// Prepare index buffer
	u32 ioffset;
	u16* ibuffer = (u16*)m_index_stream.lock( vertex_count, ioffset );

	// Prepare vertex buffer
	vertex_colored_sl* vbuffer;
	u32 voffset;
	m_vertex_stream.lock( vertex_count, &vbuffer, voffset );

	float shift = 0.0f;

	for ( u32 i = 1; i < count; ++i )
	{
		float3 const&			start_point		= points[ i - 1 ],
								current_point	= points[ i ];

		vertex_colored_sl&		line_begin		= *vbuffer++;
		vertex_colored_sl&		line_end		= *vbuffer++;

		line_begin.position = start_point;
		line_begin.color = u32_color;
		line_begin.begin_line_position = start_point;

		line_begin.shift = line_end.shift = shift;
		line_begin.pattern = line_end.pattern = 0xFFFFFFFF;

		float2 screen_start	= clip_2_screen( start_point, wvpMatrix, screen_width, screen_height );
		float2 screen_end	= clip_2_screen( current_point, wvpMatrix, screen_width, screen_height );

		shift = shift + calc_pattern( screen_start, screen_end );

		if ( (u32)shift >= pattern_length )
			shift = shift - pattern_length;

		line_end.position = current_point;
		line_end.color = u32_color;
		line_end.begin_line_position = start_point;

		*ibuffer++ = static_cast_checked< u16 >( i * 2 - 2 );
		*ibuffer++ = static_cast_checked< u16 >( i * 2 - 1 );
	}

	m_vertex_stream.unlock	( );
	m_index_stream.unlock	( );

	m_colored_geom_sl->apply( );
	m_sh_sl->apply( effect_system_line::z_disabled, 0 );

	backend::ref( ).set_vs_constant( m_WVP_sl, transpose( wvpMatrix ) );

	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_LINELIST, vertex_count, ioffset, voffset );

}

void system_renderer::draw_3D_point(
	float3 const&			position,
	float					width,
	math::color const&		color,
	bool					use_depth
)
{
	if ( !is_effects_ready( ) )
		return;

	VOSTOK_UNREFERENCED_PARAMETER( use_depth );

	u32 screen_width	= backend::ref( ).target_width( );

	float4x4 view_matrix = m_renderer_context->get_v( ),
			 proj_matrix = m_renderer_context->get_p( );

	float4x4 inv_view_proj_matrix = math::mul4x4( view_matrix, proj_matrix );

	inv_view_proj_matrix.try_invert( inv_view_proj_matrix );

	float4x4 inv_view_matrix = view_matrix;

	inv_view_matrix.try_invert( inv_view_matrix );

	float dist = ( float3( inv_view_matrix.e30, inv_view_matrix.e31, inv_view_matrix.e32 ) - position ).length( );
	float3 offset_by_x = inv_view_proj_matrix.transform_direction( float3( 1000, 0, 0 ) ).normalize( ) * 1.0f / (float)screen_width * width * 0.5f * dist,
		   offset_by_y = inv_view_proj_matrix.transform_direction( float3( 0, -1000, 0 ) ).normalize( ) * 1.0f / (float)screen_width * width * 0.5f * dist;

	float3 quad_position[4] = {
		position - offset_by_x - offset_by_y,
		position - offset_by_x + offset_by_y,
		position + offset_by_x + offset_by_y,
		position + offset_by_x - offset_by_y
	};

	typedef fixed_vector< vertex_colored, 4 >	colored_vertices_type;
	typedef fixed_vector< u16, 6 >				colored_indices_type;
	colored_vertices_type vertices;
	colored_indices_type indices;

	// Setup vertices.
	for ( u32 i = 0; i < 4; ++i )
		vertices.push_back	( vertex_colored( quad_position[i], color ) );

	// Setup indices.
	indices.push_back(2); indices.push_back(1); indices.push_back(0);
	indices.push_back(3); indices.push_back(2); indices.push_back(0);
	draw_triangles(
		&*vertices.begin( ),
		&*vertices.end( ),
		&*indices.begin( ),
		&*indices.end( ),
		false
	);
}

void system_renderer::fill_surface(
	render_target_ptr		surface0,
	render_target_ptr		surface1,
	render_target_ptr		surface2,
	render_target_ptr		surface3,
	render_target_ptr		depth_rt,
	bool					reset_depth_rt,
	D3D11_VIEWPORT*			user_view_port,
	float					pos_x,
	float					pos_y,
	float					size_x,
	float					size_y
)
{
	D3D11_VIEWPORT prev_view_port;
	u32 viewport_count					= 1;
	device::ref( ).d3d_context( )->RSGetViewports( &viewport_count, &prev_view_port );

	D3D11_VIEWPORT view_port;
	view_port.TopLeftX					= 0.f;
	view_port.TopLeftY					= 0.f;
	view_port.Width						= float( surface0->width( ) );
	view_port.Height						= float( surface0->height( ) );
	view_port.MinDepth					= 0.f;
	view_port.MaxDepth					= 1.f;

	backend::ref( ).set_render_targets(
		surface0.c_ptr( ),
		surface1.c_ptr( ),
		surface2.c_ptr( ),
		surface3.c_ptr( )
	);

	if ( reset_depth_rt )
		backend::ref( ).reset_depth_stencil_target( );
	else
		backend::ref( ).set_depth_stencil_target( depth_rt.c_ptr( ) );

	if ( user_view_port )
		device::ref( ).d3d_context( )->RSSetViewports( 1, user_view_port );
	else
		device::ref( ).d3d_context( )->RSSetViewports( 1, &view_port );

	float const pos_x0					= pos_x * 2.f - 1.f;
	float const pos_x1					= (pos_x + size_x) * 2.f - 1.f;
	float const pos_y1					= -((pos_y + size_y) * 2.f - 1.f);
	float const pos_y0					= -(pos_y * 2.f - 1.f);

	u32 offset;
	screen_vertex* pv					= m_vertex_stream_quad.lock<screen_vertex>( 4, offset );
	pv->set( float4( pos_x0, pos_y1, 0.f, 1.f ), float2( 0.f, 1.f ) ); ++pv;
	pv->set( float4( pos_x0, pos_y0, 0.f, 1.f ), float2( 0.f, 0.f ) ); ++pv;
	pv->set( float4( pos_x1, pos_y1, 0.f, 1.f ), float2( 1.f, 1.f ) ); ++pv;
	pv->set( float4( pos_x1, pos_y0, 0.f, 1.f ), float2( 1.f, 0.f ) ); ++pv;
	m_vertex_stream_quad.unlock( );

	m_screen_vertex_geometry->apply( );
	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 6, 0, offset );
	device::ref( ).d3d_context( )->RSSetViewports( 1, &prev_view_port );
}

void system_renderer::draw_aabb( math::aabb const& aabb, math::color const& color )
{

	if ( !is_effects_ready( ) )
		return;

	vertex_colored vertices[] = {
		vertex_colored( aabb.min, color ),
		vertex_colored( float3( aabb.min.x, aabb.min.y, aabb.max.z ), color ),
		vertex_colored( float3( aabb.min.x, aabb.max.y, aabb.min.z ), color ),
		vertex_colored( float3( aabb.max.x, aabb.min.y, aabb.min.z ), color ),
		vertex_colored( float3( aabb.min.x, aabb.max.y, aabb.max.z ), color ),
		vertex_colored( float3( aabb.max.x, aabb.min.y, aabb.max.z ), color ),
		vertex_colored( float3( aabb.max.x, aabb.max.y, aabb.min.z ), color ),
		vertex_colored( aabb.max, color ),
	};

	draw_lines	(
		vertices,
		vertices + array_size( vertices ),
		aabb_indices,
		aabb_indices + array_size( aabb_indices ),
		false
	);

}

void system_renderer::draw_obb( float4x4 const& transform, math::color const& color )
{
	if ( !is_effects_ready( ) )
		return;

	render::vertex_colored vertices[] = {
		vertex_colored( transform.transform_position( float3( -1.f, -1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( -1.f, -1.f, +1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( -1.f, +1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, -1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( -1.f, +1.f, +1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, -1.f, +1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, +1.f, -1.f ) ), color ),
		vertex_colored( transform.transform_position( float3( +1.f, +1.f, +1.f ) ), color ),
	};

	draw_lines	(
		vertices,
		vertices + array_size( vertices ),
		aabb_indices,
		aabb_indices + array_size( aabb_indices ),
		false
	);
}

void system_renderer::setup_grid_render_mode( u32 grid_density )
{
	m_grid_mode = true;
	m_grid_density = grid_density/100.f;
}

void system_renderer::remove_grid_render_mode( )
{
	m_grid_mode = false;
}

void system_renderer::setup_rotation_control_modes( bool color_write )
{
	m_color_write = color_write;
}

void system_renderer::draw_triangles(
	vertex_colored const* const		vertices_begin,
	vertex_colored const* const		vertices_end,
	u16 const* const				indices_begin,
	u16 const* const				indices_end,
	bool							covering_effect
)
{
	if ( !is_effects_ready( ) )
		return;

	R_ASSERT_CMP( vertices_begin, <, vertices_end );
	R_ASSERT_CMP( indices_begin,  <, indices_end );

	// Prepare ill vertex buffer.
	u32 voffset;
	u32 const vertices_size	= vertices_end - vertices_begin;
	vertex_colored* vbuffer	= (vertex_colored*)m_vertex_stream.lock( vertices_size, sizeof( vertex_colored ), voffset );
	memory::copy			( vbuffer, vertices_size * sizeof( vertex_colored ), vertices_begin, vertices_size * sizeof( vertex_colored ) );
	m_vertex_stream.unlock	( );

	// Prepare index buffer
	u32 ioffset;
	u32 const indices_size	= indices_end - indices_begin;
	u16* ibuffer			= (u16*)m_index_stream.lock( indices_size, ioffset );
	memory::copy			( ibuffer, indices_size * sizeof( u16 ), indices_begin, indices_size * sizeof( u16 ) );
	m_index_stream.unlock	( );

	m_colored_geom->apply	( );

	if ( covering_effect )
	{
		m_sh_vcolor->apply		( effect_system_colored::cover, 0 );
	}
	else
	{
		if ( !m_color_write )
			m_sh_vcolor->apply		( effect_system_colored::voided, 0 );
		else
		if ( !m_grid_mode )
			m_sh_vcolor->apply		( effect_system_colored::solid, 0 );
		else
			m_sh_vcolor->apply		( effect_system_colored::stenciled, 0 );
	}

	backend::ref( ).set_ps_constant( m_grid_density_constant, m_grid_density );

	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices_size, ioffset /* ? */, voffset );
	m_sh_vcolor->apply( effect_system_colored::wireframe, 0 );
	m_colored_geom->apply( );
	backend::ref( ).set_ps_constant( m_grid_density_constant, m_grid_density );
	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices_size, ioffset /* ? */, voffset );
}

void system_renderer::draw_ui_vertices(
	vertex_formats::TL const*		vertices,
	u32 const&						count,
	s32								prim_type,
	s32								point_type
)
{

	if ( !is_effects_ready( ) )
		return;

	u32 v_offset;
	vertex_formats::TL* vbuffer;
	m_vertex_stream.lock	( count, &vbuffer, v_offset );
	memory::copy			( vbuffer, count * sizeof( vertex_formats::TL ), vertices, count * sizeof( vertex_formats::TL ) );
	m_vertex_stream.unlock	( );

	m_ui_geom->apply( );

	if ( prim_type == 0 )
	{
		if ( point_type == 0 )
			m_sh_ui->apply( effect_system_ui::ui_font, 0 );

		else if ( point_type == 1 )
			m_sh_ui->apply( effect_system_ui::ui, 0 );

		else if ( point_type == 2 )
			m_sh_ui->apply( effect_system_ui::ui_fill, 0 );

		else
			UNREACHABLE_CODE();

		backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 3 * count / 2, 0, v_offset );

	}
	else if ( prim_type == 1 )
	{
		m_sh_ui->apply( effect_system_ui::ui_line_strip, 0 );

		backend::ref( ).render( D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, count, v_offset );
	}
	else
	{
		UNREACHABLE_CODE();
	}

}

void system_renderer::draw_render_models_selection( vector< render_model_instance_impl_ptr >& render_models )
{
	if ( !is_effects_ready( ) )
		return;

	if ( !render_models.size( ) )
		return;
	math::aabb draw_box = math::create_invalid_aabb( );
	for ( vector< render_model_instance_impl_ptr >::iterator it = render_models.begin( ), it_end = render_models.end( ); it != it_end; ++it )
	{
		render_model_instance_impl_ptr& instance = *it;

		draw_box.modify( instance->get_aabb( ).modify( instance->transform( ) ) );
	}
	m_current_selection_color		= m_selection_color * math::pow( math::abs( math::cos( m_renderer_context->m_current_time * m_selection_rate ) ), 0.5f );
	draw_aabb						(
		draw_box,
		math::color(
			m_current_selection_color.x,
			m_current_selection_color.y,
			m_current_selection_color.z,
			1.0f
		)
	);
}

void system_renderer::draw_particle_system_instance_selections( vector< resources::unmanaged_resource_ptr > const& instances )
{
	if ( instances.empty( ) )
		return;

	if ( !is_effects_ready( ) )
		return;

	if ( !m_renderer_context->scene( ) )
		return;

	if ( !m_renderer_context->scene( )->particle_world( ) )
		return;

	m_current_selection_color	= m_selection_color *
								  math::pow( math::abs( math::cos( m_renderer_context->m_current_time * m_selection_rate ) ), 0.5f );

	// TODO: LODs?
	particle::world* part_world	= m_renderer_context->scene( )->particle_world( );

	for ( vector< resources::unmanaged_resource_ptr >::const_iterator instance_it = instances.begin( );
																	  instance_it != instances.end( );
																	  ++instance_it )
	{
		particle::render_particle_emitter_instances_type emitters( g_allocator );

		part_world->get_render_emitter_instances( *instance_it, emitters );

		for ( particle::render_particle_emitter_instances_type::const_iterator it = emitters.begin( ); it != emitters.end( ); ++it )
		{
			render::render_particle_emitter_instance*	instance		= static_cast< render::render_particle_emitter_instance* >( *it );
			u32 const									num_particles	= instance->get_num_particles( );

			if ( !num_particles )
				continue;

			switch ( instance->vertex_type( ) ) {
				case particle::particle_vertex_type_billboard:
					m_sh_particle_selection->apply( effect_particle_selection::billboard, 0 );
					break;

				case particle::particle_vertex_type_billboard_subuv:
					m_sh_particle_selection->apply( effect_particle_selection::billboard_subuv, 0 );
					break;

				case particle::particle_vertex_type_trail:
				case particle::particle_vertex_type_beam:
					m_sh_particle_selection->apply( effect_particle_selection::beamtrail, 0 );
					break;
			}

			particle_shader_constants::ref( ).set(
				m_renderer_context->get_v_inverted( ).transform_direction( float3( 0, 1000, 0 ) ).normalize( ),
				m_renderer_context->get_v_inverted( ).transform_direction( float3( 1000, 0, 0 ) ).normalize( ),
				m_renderer_context->get_v_inverted( ).lines[3].xyz( ),
				instance->locked_axis( ),
				instance->screen_alignment( )
				);
			particle_shader_constants::ref( ).set_time( m_renderer_context->m_current_time );

			m_renderer_context->set_w( instance->transform( ) );

			instance->render( m_renderer_context->get_v_inverted( ).lines[3].xyz( ), num_particles );
		}
	}
}

void system_renderer::draw_speedtree_instance_selections( vector< speedtree_instance_ptr > const& instances )
{

	if ( !is_effects_ready( ) )
		return;

	if ( !instances.size( ) )
		return;

	m_current_selection_color	= m_selection_color *
								  math::pow( math::abs( math::cos( m_renderer_context->m_current_time * m_selection_rate ) ), 0.5f );

	for ( vector< speedtree_instance_ptr >::const_iterator it = instances.begin( ); it != instances.end( ); ++it )
	{

		math::aabb			draw_box( it->c_ptr( )->m_speedtree_tree_ptr->m_bbox );
		draw_box.modify		( it->c_ptr( )->m_transform );
		draw_aabb			( draw_box,
			math::color(
				m_current_selection_color.x,
				m_current_selection_color.y,
				m_current_selection_color.z,
				1.0f
			)
		);
	}
}

// No faithful body: the target has no out-of-line record and required terrain members are absent.
// STATE[STUB]
void system_renderer::draw_debug_terrain( )
{
}

void system_renderer::set_model_ghost_mode(
	polymorph_vector_base< render_model_instance > const& /*render_models*/,
	bool /*value*/
)
{
	if (!is_effects_ready())
		return;
}

void system_renderer::draw_ghost_render_models( vector< render_surface_instance* >& render_models )
{
	if (!is_effects_ready())
		return;

	float3 view_pos = m_renderer_context->get_v_inverted().c.xyz();

	for (vector< render_surface_instance* >::iterator it=render_models.begin(); it!=render_models.end(); ++it)
	{
		render_surface_instance& instance = *(*it);

		float4x4 selected_transform = *instance.m_transform;
		m_renderer_context->set_w( selected_transform);

		float4 old_current_selection_color = m_current_selection_color;
		m_current_selection_color = m_ghost_model_color;

		m_editor_selection_shader[instance.m_render_surface->get_vertex_input_type()]->apply( 0, 0 );
		instance.set_constants();
		instance.m_render_surface->m_render_geometry.geom->apply();
		backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance.m_render_surface->m_render_geometry.primitive_count*3, 0, 0);

		m_current_selection_color = old_current_selection_color;
	}
}

void system_renderer::draw_system_render_models( vector< render_surface_instance* > const& render_models )
{
	if (!is_effects_ready())
		return;

	for (vector< render_surface_instance* >::const_iterator it=render_models.begin(); it!=render_models.end(); ++it)
	{
		render_surface_instance const& instance = *(*it);

		m_renderer_context->set_w( *instance.m_transform);

		instance.m_render_surface->get_material_effects().m_effects[forward_render_stage]->apply( 0, 0 );
		instance.m_render_surface->m_render_geometry.geom->apply();
		backend::ref().render_indexed( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, instance.m_render_surface->m_render_geometry.primitive_count*3, 0, 0);
	}
}

void system_renderer::setup_scene_view( base_scene_view_ptr view_ptr )
{
	m_renderer_context->set_scene_view		( view_ptr);
}

void system_renderer::setup_render_output_window(
	base_output_window_ptr in_output_window,
	math::rectangle< float2 > const& viewport
)
{
	render_output_window* output_window = (render_output_window*)in_output_window.c_ptr();

	m_renderer_context->set_target_context		( &output_window->target_context(), false );
	backend::ref().set_render_output			( output_window->render_output() );
	backend::ref().reset_depth_stencil_target	();

	backend::ref().set_render_targets			( &*m_renderer_context->m_targets->m_family[rt_present].target,0, 0, 0);
	backend::ref().reset_depth_stencil_target	( );

	math::rectangle< float2 > res_viewport( float2( 0.f, 0.f ), float2( 1.f, 1.f ) );

	R_ASSERT			( viewport.width() );
	R_ASSERT			( viewport.height() );

	res_viewport.left	= math::max( res_viewport.left,		viewport.left);
	res_viewport.right	= math::min( res_viewport.right,	viewport.right);
	res_viewport.top	= math::max( res_viewport.top,		viewport.top);
	res_viewport.bottom	= math::min( res_viewport.bottom,	viewport.bottom);

	u32 const window_width	= backend::ref().target_width( );
	u32 const window_height	= backend::ref().target_height( );

	D3D11_VIEWPORT d3d_viewport	= { window_width * res_viewport.left, window_height * res_viewport.top, window_width * res_viewport.width(), window_height * res_viewport.height(), 0.f, 1.f};
	backend::ref().set_viewport	( d3d_viewport);
}

void system_renderer::set_selection_parameters( float4 in_selection_color, float in_selection_rate )
{
	m_selection_color = in_selection_color;
	m_selection_rate  = in_selection_rate;
}

} // namespace render
} // namespace vostok
