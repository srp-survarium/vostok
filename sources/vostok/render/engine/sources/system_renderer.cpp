#include "pch.h"
#include "system_renderer.h"
#include "help_math.h"

#include <vostok/particle/world.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/resource_manager.h>
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

//TODO: not here
const D3D_INPUT_ELEMENT_DESC F_L_sl[] =
{
	{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",		0, DXGI_FORMAT_R8G8B8A8_UNORM,	0, 12,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	1, DXGI_FORMAT_R32_FLOAT,		0, 28,	D3D_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD",	2, DXGI_FORMAT_R32_UINT,		0, 32,	D3D_INPUT_PER_VERTEX_DATA, 0},
};

system_renderer::system_renderer( renderer_context* renderer_context ) :
	m_cook_data_to_delete		( 0 ),
	m_selection_color			( 0.0f, 0.0f, 0.5f, 1.0f ),
	m_selection_rate			( 1.0f ),
	m_current_selection_color	( 0.0f, 0.0f, 0.0f, 0.0f ),
	m_ghost_model_color			( 0.2f, 0.2f, 0.2f, 0.2f ),
	m_renderer_context			( renderer_context ),
	m_WVP_sl					( 0 ),
	m_c_start_corner			( 0 ),
	m_vertex_stream				( 1024 * 1024 ),
	m_index_stream				( 512 * 1024 ),
	m_vertex_stream_quad		( 1024 * 1024 ),
	m_index_stream_quad			( 512 * 1024 ),
	m_grid_mode					( false ),
	m_grid_density				( 0.0f ),
	m_grid_density_constant		( 0 ),
	m_color_write				( true ),
	m_rotation_mode_ref_value	( 0 )
{
	ASSERT						( m_renderer_context );

	m_renderer_context->m_quad_ib			= create_quad_ib( );
	m_renderer_context->m_g_quad_uv			= resource_manager::ref( ).create_geometry( vertex_formats::F_TL, array_size( vertex_formats::F_TL ), sizeof( vertex_formats::TL ), backend::ref( ).vertex.buffer( ), *m_renderer_context->m_quad_ib );
	m_renderer_context->m_g_quad_2uv		= resource_manager::ref( ).create_geometry( vertex_formats::F_TL2uv, array_size( vertex_formats::F_TL2uv ), sizeof( vertex_formats::TL2uv ), backend::ref( ).vertex.buffer( ), *m_renderer_context->m_quad_ib );
	m_renderer_context->m_g_quad_eye_ray	= resource_manager::ref( ).create_geometry( vertex_formats::F_Tquad, array_size( vertex_formats::F_Tquad ), sizeof( vertex_formats::Tquad ), backend::ref( ).vertex.buffer( ), *m_renderer_context->m_quad_ib );

	m_colored_geom		= resource_manager::ref( ).create_geometry( vertex_formats::F_L, array_size( vertex_formats::F_L ), sizeof( vertex_formats::L ), m_vertex_stream.buffer( ), m_index_stream.buffer( ) );

	m_colored_geom_sl	= resource_manager::ref( ).create_geometry( F_L_sl, array_size( F_L_sl ), sizeof( vertex_colored_sl ), m_vertex_stream.buffer( ), m_index_stream.buffer( ) );

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
}

untyped_buffer* system_renderer::create_quad_ib( )
{
	// LOCALS
	// u16[24576] 						indices
	// ******

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

	// FUNCTION BODY[0x6449a0]: 28
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6449ad>|0x00d|+0x013:'81'
	// <0>
	// <1>
	// <2>
	// <0x6449c0>|0x020|+0x009:'85'
	// <0x6449c9>|0x029|+0x009:'86'
	// <0x6449d2>|0x032|+0x009:'87'
	// <0>
	// <0x6449db>|0x03b|+0x009:'89'
	// <0x6449e4>|0x044|+0x006:'90'
	// <0x6449ea>|0x04a|+0x006:'91'
	// <0>
	// <0x6449f0>|0x050|+0x008:'93'
	// <0>
	// <1>
	// <2>
	// <0x6449f8>|0x058|+0x01a:'97'
	// ******
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

	// FUNCTION BODY[0x644700]: 17
	// <0x644700>|0x000|+0x012:'254'
	// <0x644712>|0x012|-0x00a:'254'
	// <0>
	// <0x644708>|0x008|+0x005:'256'
	// <0>
	// <1>
	// <0x64470d>|0x00d|+0x050:'259'
	// <0x64475d>|0x05d|-0x042:'260'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x64471b>|0x01b|+0x045:'270'
	// <0x644760>|0x060|-0x004:'270'
	// <0x64475c>|0x05c|+0x003:'271'
	// <0x64475f>|0x05f|+0x003:'271'
	// ******
}

system_renderer::~system_renderer( )
{
	// FUNCTION BODY[0x644e70]: 1
	// <0>
	// ******
}

void system_renderer::set_w( float4x4 const& m )
{
	m_renderer_context->set_w( m );

	// FUNCTION BODY[0x644990]: 1
	// <0x644990>|0x000|+0x009:'293'
	// ******
}

// claude@NOTE: set_v/set_p and the three matrix getters are declared by the canonical
// class but have no out-of-line target copy (/OPT:REF stripped these editor-facing
// forwarders); ported from the legacy system_renderer.cpp at their legacy position.
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
	// LOCALS
	// u32 								ioffset
	// u32 								voffset
	// ******

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

	// FUNCTION BODY[0x646460]: 28
	// <0x646467>|0x007|+0x016:'326'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x64647d>|0x01d|+0x004:'335'
	// <0x646481>|0x021|+0x008:'336'
	// <0x646489>|0x029|+0x01f:'337'
	// <0x6464a8>|0x048|+0x028:'338'
	// <0>
	// <1>
	// <2>
	// <0x6464d0>|0x070|+0x004:'342'
	// <0x6464d4>|0x074|+0x016:'343'
	// <0x6464ea>|0x08a|+0x010:'344'
	// <0x6464fa>|0x09a|+0x024:'345'
	// <0x64651e>|0x0be|+0x00b:'346'
	// <0>
	// <0x646529>|0x0c9|+0x007:'348'
	// <0x646530>|0x0d0|+0x024:'349'
	// <0x646554>|0x0f4|+0x002:'350'
	// <0x646556>|0x0f6|+0x02a:'351'
	// <0>
	// <0x646580>|0x120|+0x019:'353'
	// ******
}

float2 clip_2_screen(
	float3 const&		world_pixel,
	float4x4 const&		wvpMatrix,
	u32					screen_width,
	u32					screen_height
)
{
	// LOCALS
	// float4 							result
	// ******

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

	// FUNCTION BODY[0x6447d0]: 16
	// <0x6447d3>|0x003|+0x0b1:'358'
	// <0>
	// <0x644884>|0x0b4|+0x023:'360'
	// <0x6448a7>|0x0d7|+0x008:'361'
	// <0>
	// <0x6448af>|0x0df|+0x008:'363'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6448b7>|0x0e7|+0x0b2:'370'
	// <0x644969>|0x199|+0x01a:'371'
	// <0>
	// <0x644983>|0x1b3|+0x007:'373'
	// ******
}

static u8 pattern_length = 8;

// frac: COMDAT copy of the help_math.h inline (FUNCTION BODY[0x6446f0])

u8 calc_pattern( float2 const& begin, float2 const& end )
{
	return static_cast_checked< u8 >( frac( math::max( math::abs( end.elements[0] - begin.elements[0] ), math::abs( end.elements[1] - begin.elements[1] ) ) / (float)pattern_length ) * ( (float)pattern_length ) );

	// FUNCTION BODY[0x644770]: 1
	// <0x644776>|0x006|+0x048:'385'
	// ******
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
	// LOCALS
	// float 							shift
	// u32 								ioffset
	// u32 								screen_width
	// vertex_colored_sl* 				vbuffer
	// float4x4 						wvpMatrix
	// u32 								vertex_count
	// u32 								u32_color
	// u32 								voffset
	// u32 								screen_height
	// u32 								i
	// float2 							screen_end
	// float3 							current_point
	// float2 							screen_start
	// ******

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
		line_begin.pattern = line_end.pattern = pattern;

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

	if ( use_depth )
		m_sh_sl->apply		( effect_system_line::z_enabled, 0 );
	else
		m_sh_sl->apply		( effect_system_line::z_disabled, 0 );

	backend::ref( ).set_vs_constant( m_WVP_sl, transpose( wvpMatrix ) );

	backend::ref( ).render_indexed( D3D_PRIMITIVE_TOPOLOGY_LINELIST, vertex_count, ioffset, voffset );

	// FUNCTION BODY[0x646070]: 72
	// <0x64607d>|0x00d|+0x014:'390'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x646091>|0x021|+0x035:'398'
	// <0>
	// <0x6460c6>|0x056|+0x02c:'400'
	// <0x6460f2>|0x082|+0x01d:'401'
	// <0>
	// <1>
	// <0x64610f>|0x09f|+0x071:'404'
	// <0x646180>|0x110|-0x06c:'404'
	// <0x646114>|0x0a4|+0x007:'405'
	// <0>
	// <1>
	// <2>
	// <0x64611b>|0x0ab|+0x018:'409'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x646133>|0x0c3|+0x01c:'414'
	// <0>
	// <0x64614f>|0x0df|+0x003:'416'
	// <0>
	// <0x646152>|0x0e2|+0x1a9:'418'
	// <0x6462fb>|0x28b|-0x177:'418'
	// <0>
	// <1>
	// <0x646184>|0x114|+0x008:'421'
	// <0>
	// <0x64618c>|0x11c|+0x005:'423'
	// <0x646191>|0x121|+0x00f:'424'
	// <0>
	// <0x6461a0>|0x130|+0x012:'426'
	// <0x6461b2>|0x142|+0x007:'427'
	// <0x6461b9>|0x149|+0x004:'428'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6461bd>|0x14d|+0x038:'433'
	// <0x6461f5>|0x185|+0x01f:'434'
	// <0>
	// <0x646214>|0x1a4|+0x058:'436'
	// <0>
	// <0x64626c>|0x1fc|+0x01a:'438'
	// <0x646286>|0x216|+0x025:'439'
	// <0>
	// <0x6462ab>|0x23b|+0x00a:'441'
	// <0x6462b5>|0x245|+0x00b:'442'
	// <0>
	// <1>
	// <0x6462c0>|0x250|+0x020:'445'
	// <0x6462e0>|0x270|+0x020:'446'
	// <0>
	// <1>
	// <0x646300>|0x290|+0x02d:'449'
	// <0x64632d>|0x2bd|+0x02e:'450'
	// <0>
	// <0x64635b>|0x2eb|+0x008:'452'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x646363>|0x2f3|+0x021:'457'
	// <0>
	// <0x646384>|0x314|+0x059:'459'
	// <0>
	// <0x6463dd>|0x36d|+0x078:'461'
	// ******
}

void system_renderer::draw_3D_point(
	float3 const&			position,
	float					width,
	math::color const&		color,
	bool					use_depth
)
{
	// LOCALS
	// float4x4 						proj_matrix
	// float 							dist
	// fixed_vector< vertex_colored, 4 > vertices
	// u32 								screen_width
	// float4x4 						inv_view_matrix
	// float3[4] 						quad_position
	// fixed_vector< u16, 6 > 			indices
	// float3 							offset_by_x
	// float4x4 						view_matrix
	// float4x4 						inv_view_proj_matrix
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector< vertex_colored, 4 >
	// 	colored_vertices_type;

	// typedef
	// 	fixed_vector< u16, 6 >
	// 	colored_indices_type;

	// ******

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

	// Distance to view pos.
	float dist = ( float3( inv_view_matrix.e30, inv_view_matrix.e31, inv_view_matrix.e32 ) - position ).length( );

	// Calc quad vertex offsets.
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

	// FUNCTION BODY[0x645a60]: 50
	// <0x645a60>|0x000|+0x022:'466'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x645a82>|0x022|+0x020:'471'
	// <0>
	// <0x645aa2>|0x042|+0x019:'473'
	// <0x645abb>|0x05b|+0x014:'474'
	// <0>
	// <0x645acf>|0x06f|+0x018:'476'
	// <0>
	// <0x645ae7>|0x087|+0x00f:'478'
	// <0>
	// <1>
	// <0x645af6>|0x096|+0x01d:'481'
	// <0>
	// <1>
	// <0x645b13>|0x0b3|+0x05f:'484'
	// <0>
	// <1>
	// <0x645b72>|0x112|+0x176:'487'
	// <0x645ce8>|0x288|+0x0d2:'488'
	// <0>
	// <1>
	// <0x645dba>|0x35a|+0x0dc:'491'
	// <0>
	// <0x645e96>|0x436|+0x010:'493'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x645ea6>|0x446|+0x00e:'499'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x645eb4>|0x454|+0x105:'504'
	// <0>
	// <1>
	// <0x645fb9>|0x559|+0x019:'507'
	// <0x645fd2>|0x572|+0x06b:'508'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x64603d>|0x5dd|+0x026:'515'
	// ******
}

// claude@NOTE: legacy body diverged - legacy fill_surface is a static free single-surface TL quad; canonical member takes 4 surfaces + depth + viewport + quad coords (see legacy remainder note); matcher-phase work.
// STATE[STUB]
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
	// LOCALS
	// D3D11_VIEWPORT 					prev_view_port
	// D3D11_VIEWPORT 					view_port
	// u32 								offset
	// float 							pos_x0
	// float 							pos_x1
	// float 							pos_y1
	// float 							pos_y0
	// ******

	// CALL SITE INFO
	// <0x644a52> -> void < unknown >( u32*, D3D11_VIEWPORT* )
	// ******

	// FUNCTION BODY[0x644a20]: 42
	// <0>
	// <1>
	// <0x644a29>|0x009|+0x02b:'532'
	// <0>
	// <0x644a54>|0x034|+0x017:'534'
	// <0x644a6b>|0x04b|+0x010:'535'
	// <0x644a7b>|0x05b|+0x007:'536'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x644a82>|0x062|+0x035:'541'
	// <0>
	// <0x644ab7>|0x097|+0x006:'543'
	// <0x644abd>|0x09d|+0x015:'544'
	// <0x644ad2>|0x0b2|+0x002:'545'
	// <0x644ad4>|0x0b4|+0x019:'546'
	// <0>
	// <0x644aed>|0x0cd|+0x00d:'548'
	// <0x644afa>|0x0da|+0x019:'549'
	// <0x644b13>|0x0f3|+0x002:'550'
	// <0x644b15>|0x0f5|+0x01e:'551'
	// <0>
	// <1>
	// <2>
	// <0x644b33>|0x113|+0x015:'555'
	// <0x644b48>|0x128|+0x008:'556'
	// <0>
	// <1>
	// <2>
	// <0x644b50>|0x130|+0x06d:'560'
	// <0x644bbd>|0x19d|+0x060:'561'
	// <0x644c1d>|0x1fd|+0x033:'562'
	// <0x644c50>|0x230|+0x06b:'563'
	// <0x644cbb>|0x29b|+0x036:'564'
	// <0x644cf1>|0x2d1|+0x037:'565'
	// <0>
	// <0x644d28>|0x308|+0x008:'567'
	// <0>
	// <0x644d30>|0x310|+0x076:'569'
	// <0>
	// <0x644da6>|0x386|+0x02a:'571'
	// ******
}

void system_renderer::draw_aabb( math::aabb const& aabb, math::color const& color )
{
	// LOCALS
	// vertex_colored[8] 				vertices
	// ******

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

	// FUNCTION BODY[0x6465a0]: 20
	// <0x6465a6>|0x006|+0x00d:'576'
	// <0>
	// <1>
	// <2>
	// <0x6465b3>|0x013|+0x004:'580'
	// <0x6465b7>|0x017|+0x008:'581'
	// <0x6465bf>|0x01f|+0x053:'582'
	// <0x646612>|0x072|+0x01d:'583'
	// <0x64662f>|0x08f|+0x031:'584'
	// <0x646660>|0x0c0|+0x038:'585'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x646698>|0x0f8|+0x0a8:'595'
	// ******
}

// claude@NOTE: no target address - draw_obb is declared in the target's class
// but has no out-of-line copy (fully inlined under LTCG); body ported from the
// legacy system_renderer.cpp at its legacy position, with the fifth
// draw_lines argument (covering_effect) spelled out as the canonical
// declaration has no default.
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

// claude@NOTE: the three grid/rotation mode setters are declared by the canonical class
// but have no out-of-line target copy; ported from the legacy system_renderer.cpp at
// their legacy position (directly after draw_obb).
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
	// LOCALS
	// u32 								ioffset
	// u32 								voffset
	// ******

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

	// FUNCTION BODY[0x6457c0]: 47
	// <0x6457cd>|0x00d|+0x00f:'630'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6457dc>|0x01c|+0x004:'638'
	// <0x6457e0>|0x020|+0x016:'639'
	// <0x6457f6>|0x036|+0x00f:'640'
	// <0x645805>|0x045|+0x028:'641'
	// <0>
	// <1>
	// <2>
	// <0x64582d>|0x06d|+0x008:'645'
	// <0x645835>|0x075|+0x012:'646'
	// <0x645847>|0x087|+0x010:'647'
	// <0x645857>|0x097|+0x024:'648'
	// <0>
	// <0x64587b>|0x0bb|+0x00b:'650'
	// <0>
	// <0x645886>|0x0c6|+0x007:'652'
	// <0>
	// <0x64588d>|0x0cd|+0x029:'654'
	// <0>
	// <0x6458b6>|0x0f6|+0x002:'656'
	// <0>
	// <0x6458b8>|0x0f8|+0x009:'658'
	// <0x6458c1>|0x101|+0x005:'659'
	// <0x6458c6>|0x106|+0x002:'660'
	// <0x6458c8>|0x108|+0x009:'661'
	// <0x6458d1>|0x111|+0x002:'662'
	// <0x6458d3>|0x113|+0x002:'663'
	// <0x6458d5>|0x115|+0x010:'664'
	// <0>
	// <1>
	// <0x6458e5>|0x125|+0x022:'667'
	// <0>
	// <0x645907>|0x147|+0x079:'669'
	// <0>
	// <1>
	// <2>
	// <0x645980>|0x1c0|+0x029:'673'
	// <0x6459a9>|0x1e9|+0x00b:'674'
	// <0x6459b4>|0x1f4|+0x022:'675'
	// <0x6459d6>|0x216|+0x07b:'676'
	// ******
}

void system_renderer::draw_ui_vertices(
	vertex_formats::TL const*		vertices,
	u32 const&						count,
	s32								prim_type,
	s32								point_type
)
{
	// LOCALS
	// u32 								v_offset
	// ******

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

	// FUNCTION BODY[0x645680]: 39
	// <0x64568c>|0x00c|+0x00f:'697'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64569b>|0x01b|+0x016:'702'
	// <0x6456b1>|0x031|+0x01c:'703'
	// <0x6456cd>|0x04d|+0x029:'704'
	// <0>
	// <0x6456f6>|0x076|+0x00b:'706'
	// <0>
	// <0x645701>|0x081|+0x007:'708'
	// <0>
	// <0x645708>|0x088|+0x008:'710'
	// <0x645710>|0x090|+0x004:'711'
	// <0>
	// <1>
	// <0x645714>|0x094|+0x00a:'714'
	// <0>
	// <1>
	// <0x64571e>|0x09e|+0x010:'717'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64572e>|0x0ae|+0x024:'722'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x645752>|0x0d2|+0x012:'727'
	// <0>
	// <0x645764>|0x0e4|-0x019:'729'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x64574b>|0x0cb|+0x065:'736'
	// ******
}

void system_renderer::draw_render_models_selection( vector< render_model_instance_impl_ptr >& render_models )
{
	// LOCALS
	// math::aabb 						draw_box
	// ******

	// CALL SITE INFO
	// <0x6469b2> -> math::aabb < unknown >()
	// ******

	if ( !is_effects_ready( ) )
		return;

	math::aabb draw_box = math::create_invalid_aabb( );

	for ( vector< render_model_instance_impl_ptr >::iterator it = render_models.begin( ), it_end = render_models.end( ); it != it_end; ++it )
		draw_box.modify				( ( *it )->get_aabb( ) );

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

	// FUNCTION BODY[0x646910]: 44
	// <0x64691b>|0x00b|+0x011:'1003'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x64692c>|0x01c|+0x014:'1008'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x646940>|0x030|+0x05b:'1013'
	// <0>
	// <0x64699b>|0x08b|+0x005:'1015'
	// <0>
	// <1>
	// <0x6469a0>|0x090|+0x03b:'1018'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <0x6469db>|0x0cb|+0x045:'1037'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x646a20>|0x110|+0x0c4:'1046'
	// ******
}

void system_renderer::draw_particle_system_instance_selections( vector< resources::unmanaged_resource_ptr > const& instances )
{
	// LOCALS
	// particle::world* 				part_world
	// resources::unmanaged_resource_ptr const* instance_it
	// vectora< particle::render_particle_emitter_instance* > emitters
	// particle::render_particle_emitter_instance* const* it
	// ******

	// STATICS
	// static < NoType > 				 = <0x645668>;
	// ******

	// CALL SITE INFO
	// <0x64532d> -> void < unknown >( resources::unmanaged_resource_ptr, vectora< particle::render_particle_emitter_instance* >& )
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x634385 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x634389 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x634390 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

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

	// FUNCTION BODY[0x6451f0]: 80
	// <0x6451f0>|0x000|+0x01a:'1070'
	// <0>
	// <1>
	// <0x64520a>|0x01a|+0x00f:'1073'
	// <0>
	// <1>
	// <0x645219>|0x029|+0x013:'1076'
	// <0>
	// <1>
	// <0x64522c>|0x03c|+0x00c:'1079'
	// <0>
	// <1>
	// <2>
	// <0x645238>|0x048|+0x05e:'1083'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x645296>|0x0a6|+0x05b:'1091'
	// <0>
	// <0x6452f1>|0x101|+0x00f:'1093'
	// <0>
	// <0x645300>|0x110|+0x005:'1095'
	// <0>
	// <0x645305>|0x115|+0x02a:'1097'
	// <0>
	// <0x64532f>|0x13f|+0x014:'1099'
	// <0>
	// <0x645343>|0x153|+0x002:'1101'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x645345>|0x155|+0x026:'1116'
	// <0>
	// <0x64536b>|0x17b|+0x008:'1118'
	// <0>
	// <1>
	// <0x645373>|0x183|+0x012:'1121'
	// <0>
	// <0x645385>|0x195|+0x002:'1123'
	// <0x645387>|0x197|+0x002:'1124'
	// <0>
	// <1>
	// <0x645389>|0x199|+0x005:'1127'
	// <0x64538e>|0x19e|+0x002:'1128'
	// <0>
	// <1>
	// <2>
	// <0x645390>|0x1a0|+0x010:'1132'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x6453a0>|0x1b0|+0x243:'1142'
	// <0x6455e3>|0x3f3|+0x014:'1143'
	// <0>
	// <0x6455f7>|0x407|+0x00f:'1145'
	// <0>
	// <0x645606>|0x416|+0x034:'1147'
	// <0>
	// <0x64563a>|0x44a|+0x024:'1149'
	// ******
}

void system_renderer::draw_speedtree_instance_selections( vector< speedtree_instance_ptr > const& instances )
{
	// LOCALS
	// math::aabb 						draw_box
	// ******

	if ( !is_effects_ready( ) )
		return;

	if ( !instances.size( ) )
		return;

	m_current_selection_color	= m_selection_color *
								  math::pow( math::abs( math::cos( m_renderer_context->m_current_time * m_selection_rate ) ), 0.5f );

	for ( vector< speedtree_instance_ptr >::const_iterator it = instances.begin( ); it != instances.end( ); ++it )
	{
		math::aabb			draw_box( math::create_zero_aabb( ) );
		draw_box			= it->c_ptr( )->m_speedtree_tree_ptr->m_bbox;
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

	// FUNCTION BODY[0x646750]: 77
	// <0x64675a>|0x00a|+0x00f:'1154'
	// <0>
	// <1>
	// <0x646769>|0x019|+0x011:'1157'
	// <0>
	// <1>
	// <2>
	// <0x64677a>|0x02a|+0x07e:'1161'
	// <0>
	// <0x6467f8>|0x0a8|+0x029:'1163'
	// <0>
	// <1>
	// <0x646821>|0x0d1|+0x030:'1166'
	// <0x646851>|0x101|-0x04d:'1167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x646804>|0x0b4|+0x062:'1176'
	// <0x646866>|0x116|+0x094:'1176'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <45>
	// <46>
	// <47>
	// <48>
	// <49>
	// <50>
	// <51>
	// <52>
	// <53>
	// ******
}

// claude@NOTE: no addressed target record (inlined into renderer::render or
// folded); the legacy body is blocked on members the canonical class dropped
// (m_terrain_debug_material / on_material_loaded) and on the retired render-side
// terrain subsystem (scene::select_terrain_cells, terrain::m_grid_geom_*), so it
// has no faithful port - link stub only. Legacy body: `git show
// 8bb5b3dfc:temp/render_legacy/engine/sources/system_renderer.cpp`.
// STATE[STUB]
void system_renderer::draw_debug_terrain( )
{
}

// claude@NOTE: the six members below are declared by the canonical class but have no
// out-of-line target copy (/OPT:REF stripped the editor-facing surface); ported from
// the legacy system_renderer.cpp at their legacy position, adapted to the canonical
// declarations (base_scene_view_ptr / base_output_window_ptr / math::rectangle< float2 >
// spellings, explicit apply( 0, 0 ) arity, PIX scopes dropped).
void system_renderer::set_model_ghost_mode(
	polymorph_vector_base< render_model_instance > const& /*render_models*/,
	bool /*value*/
)
{
	if (!is_effects_ready())
		return;

	//for (polymorph_vector_base<render_model_instance>::iterator it=render_models.begin(); it!=render_models.end(); ++it)
	//{
	//	render_model_instance* instance = (*it);
	//
	//	if (value)
	//	{
	//		m_render_model_to_material[instance] = instance->m_material_effects;
	//
	//		material_effects temp_material;
	//		temp_material.m_material = instance->m_material_effects.m_material;
	//		instance->m_material_effects = temp_material;
	//
	//		instance->m_material_effects.m_effects[forward_render_stage]	 = m_editor_model_ghost_shader;
	//		instance->m_material_effects.stage_enable[forward_render_stage] = true;
	//	}
	//	else
	//	{
	//		if (m_render_model_to_material.find(instance)!=m_render_model_to_material.end())
	//		{
	//			instance->m_material_effects = m_render_model_to_material[instance];
	//			m_render_model_to_material.erase(instance);
	//		}
	//	}
	//}
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
