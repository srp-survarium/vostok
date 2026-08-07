#include "pch.h"
#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_geometry.h>
#include <vostok/render/core/resource_manager.h>
#include "effect_light_propagation_volumes.h"
#include "radiance_volume.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"

namespace vostok {
namespace render {

static bool s_lpv1_value = false;
static console_commands::cc_bool s_lpv1(
	"lpv1",
	s_lpv1_value,
	false,
	console_commands::command_type_user_specific
);

const D3D_INPUT_ELEMENT_DESC injection_geometry_vertex_layout[] =
{
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 0,  D3D_INPUT_PER_VERTEX_DATA, 0 },
};

const D3D_INPUT_ELEMENT_DESC sliced_cube_vertex_layout[] =
{
	{"POSITION",  0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,	D3D_INPUT_PER_VERTEX_DATA, 0}, // grid normalized x, y, z
	{"TEXCOORD",  0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 16,	D3D_INPUT_PER_VERTEX_DATA, 0}, // transformed xy rt position, and slice index
};

injection_geometry::injection_geometry( u32 rsm_size ) :
	m_num_points	( rsm_size * rsm_size ),
	m_stride		( sizeof( float2 ) ),
	m_rsm_size_x	( rsm_size ),
	m_rsm_size_y	( rsm_size )
{
	// FUNCTION BODY[0x5efee0]
	float2* vertices					= ALLOC(float2, m_num_points);
	float2* vertices_begin				= vertices;

	for (u32 u = 0; u < rsm_size; u++)
		for (u32 v = 0; v < rsm_size; v++)
			*(vertices++) = float2(float(u), float(v)) / float(rsm_size);

	m_vertext_declaration				= resource_manager::ref().create_declaration(injection_geometry_vertex_layout, array_size(injection_geometry_vertex_layout));
	m_vertex_buffer						= resource_manager::ref().create_buffer	(
		m_num_points * m_stride,
		vertices_begin,
		enum_buffer_type_vertex,
		false,
		false
	);
	FREE								(vertices_begin);
}

injection_geometry::injection_geometry( u32 rsm_size_x, u32 rsm_size_y ) :
	m_num_points	( 0 ),
	m_stride		( sizeof( float2 ) ),
	m_rsm_size_x	( 0 ),
	m_rsm_size_y	( 0 )
{
	// FUNCTION BODY[0x5efec0]
	prepare(rsm_size_x, rsm_size_y);
}

void injection_geometry::prepare( u32 rsm_size_x, u32 rsm_size_y )
{
	// FUNCTION BODY[0x5efd80]
	if (m_rsm_size_x == rsm_size_x && m_rsm_size_y == rsm_size_y)
		return;

	m_num_points						= rsm_size_x * rsm_size_y;
	m_rsm_size_x						= rsm_size_x;
	m_rsm_size_y						= rsm_size_y;

	float2* vertices					= ALLOC(float2, m_num_points);
	float2* vertices_begin				= vertices;

	for (u32 u = 0; u < rsm_size_x; u++)
		for (u32 v = 0; v < rsm_size_y; v++)
			*(vertices++) = float2(float(u) / float(rsm_size_x), float(v) / float(rsm_size_y));

	m_vertext_declaration				= resource_manager::ref().create_declaration(injection_geometry_vertex_layout, array_size(injection_geometry_vertex_layout));
	m_vertex_buffer						= resource_manager::ref().create_buffer	(
		m_num_points * m_stride,
		vertices_begin,
		enum_buffer_type_vertex,
		false,
		false
	);
	FREE								(vertices_begin);
}

void injection_geometry::draw( )
{
	// FUNCTION BODY[0x5ef240]
	backend::ref().set_declaration			(m_vertext_declaration.c_ptr());
	backend::ref().set_vb					(m_vertex_buffer.c_ptr(), m_stride);
	backend::ref().set_ib					(NULL);
	backend::ref().render					(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, m_num_points, 0);
}

sliced_cube_geometry::sliced_cube_geometry( u32 in_num_cells ) :
	m_slices	( in_num_cells ),
	m_stride	( sizeof( sliced_cube_geometry_vertex ) )
{
	// FUNCTION BODY[0x5ef970]
	u32 const						num_vertices	= m_slices * 4;
	u32 const						num_indices		= m_slices * 6;

	sliced_cube_geometry_vertex*	vertices		= (sliced_cube_geometry_vertex*)ALLOCA(m_stride * num_vertices);
	u16*							indices			= (u16*)ALLOCA(sizeof(u16) * num_indices);

	sliced_cube_geometry_vertex*	vertices_begin	= vertices;
	u16*							indices_begin	= indices;

	for (u32 slice_index = 0; slice_index < m_slices; slice_index++)
	{
		float const slice_z				= float(slice_index) / float(m_slices);

		new(vertices)sliced_cube_geometry_vertex;
		vertices->position				= float4(0.0f, 0.0f, slice_z, float(slice_index));
		vertices->xy_and_slice_index	= float4(vertices->position.x * 2.0f - 1.0f, (1.0f - vertices->position.y) * 2.0f - 1.0f, 0.0f, 1.0f);
		vertices++;

		new(vertices)sliced_cube_geometry_vertex;
		vertices->position				= float4(1.0f, 0.0f, slice_z, float(slice_index));
		vertices->xy_and_slice_index	= float4(vertices->position.x * 2.0f - 1.0f, (1.0f - vertices->position.y) * 2.0f - 1.0f, 0.0f, 1.0f);
		vertices++;

		new(vertices)sliced_cube_geometry_vertex;
		vertices->position				= float4(1.0f, 1.0f, slice_z, float(slice_index));
		vertices->xy_and_slice_index	= float4(vertices->position.x * 2.0f - 1.0f, (1.0f - vertices->position.y) * 2.0f - 1.0f, 0.0f, 1.0f);
		vertices++;

		new(vertices)sliced_cube_geometry_vertex;
		vertices->position				= float4(0.0f, 1.0f, slice_z, float(slice_index));
		vertices->xy_and_slice_index	= float4(vertices->position.x * 2.0f - 1.0f, (1.0f - vertices->position.y) * 2.0f - 1.0f, 0.0f, 1.0f);
		vertices++;

		u16 const offset				= static_cast<u16>(slice_index * 4);

		// clockwise
		*(indices++)					= 0 + offset;
		*(indices++)					= 1 + offset;
		*(indices++)					= 2 + offset;
		*(indices++)					= 0 + offset;
		*(indices++)					= 2 + offset;
		*(indices++)					= 3 + offset;
	}

	m_vertext_declaration				= resource_manager::ref().create_declaration(sliced_cube_vertex_layout, array_size(sliced_cube_vertex_layout));
	m_vertex_buffer						= resource_manager::ref().create_buffer(num_vertices * m_stride, vertices_begin, enum_buffer_type_vertex, false, false);
	m_index_buffer						= resource_manager::ref().create_buffer(num_indices * sizeof(u16), indices_begin, enum_buffer_type_index,  false, false);
}

void sliced_cube_geometry::draw( )
{
	// FUNCTION BODY[0x5ef100]
	backend::ref().set_declaration		(m_vertext_declaration.c_ptr());
	backend::ref().set_vb				(m_vertex_buffer.c_ptr(), m_stride);
	backend::ref().set_ib				(m_index_buffer.c_ptr(), 0);
	backend::ref().render_indexed		(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_slices * 6, 0, 0);
}

radiance_volume::radiance_volume(
	u32 const	in_rsm_size,
	u32 const	in_num_cells,
	u32 const	in_num_propagate_iterations,
	float const	in_scale,
	float const	in_flux_amplifier
) :
	m_injection_geometry			( in_rsm_size ),
	m_injection_geometry_from_camera( 128, 128 ),
	m_sliced_cube_geometry			( in_num_cells ),
	m_scale							( in_scale ),
	m_flux_amplifier				( in_flux_amplifier ),
	m_num_cells						( in_num_cells ),
	m_bbox							( math::create_zero_aabb( ) ),
	m_previous_origin				( 0.0f, 0.0f, 0.0f ),
	m_num_propagate_iterations		( in_num_propagate_iterations ),
	m_is_position_changed			( true ),
	m_prev_position					( 0.0f, 0.0f, 0.0f )
{
	// FUNCTION BODY[0x5f1780]
	DXGI_FORMAT rv_format = s_lpv1_value ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R16G16B16A16_FLOAT;

	m_3d_rt_previous_radiance_r = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_previous_radiance_g = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_previous_radiance_b = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );

	m_3d_rt_accumulated_propagation_r = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_accumulated_propagation_g = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_accumulated_propagation_b = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );

	m_3d_t_accumulated_propagation_r = m_3d_rt_accumulated_propagation_r->get_texture( );
	m_3d_t_accumulated_propagation_g = m_3d_rt_accumulated_propagation_g->get_texture( );
	m_3d_t_accumulated_propagation_b = m_3d_rt_accumulated_propagation_b->get_texture( );

	m_3d_rt_radiance_r = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_radiance_g = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_radiance_b = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );

	m_3d_t_radiance_r = m_3d_rt_radiance_r->get_texture( );
	m_3d_t_radiance_g = m_3d_rt_radiance_g->get_texture( );
	m_3d_t_radiance_b = m_3d_rt_radiance_b->get_texture( );

	m_3d_rt_radiance_r_apply = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_radiance_g_apply = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_radiance_b_apply = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );

	m_3d_t_radiance_r_apply = m_3d_rt_radiance_r_apply->get_texture( );
	m_3d_t_radiance_g_apply = m_3d_rt_radiance_g_apply->get_texture( );
	m_3d_t_radiance_b_apply = m_3d_rt_radiance_b_apply->get_texture( );

	m_3d_t_previous_radiance_r = m_3d_rt_previous_radiance_r->get_texture( );
	m_3d_t_previous_radiance_g = m_3d_rt_previous_radiance_g->get_texture( );
	m_3d_t_previous_radiance_b = m_3d_rt_previous_radiance_b->get_texture( );

	m_3d_rt_occluders = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_t_occluders = m_3d_rt_occluders->get_texture( );

	m_3d_rt_radiance_intermediate_r = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_radiance_intermediate_g = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );
	m_3d_rt_radiance_intermediate_b = resource_manager::ref( ).create_volume_render_target( NULL, in_num_cells, in_num_cells, in_num_cells, rv_format, enum_rt_usage_render_target, D3D11_USAGE_DEFAULT );

	m_3d_t_radiance_intermediate_r = m_3d_rt_radiance_intermediate_r->get_texture( );
	m_3d_t_radiance_intermediate_g = m_3d_rt_radiance_intermediate_g->get_texture( );
	m_3d_t_radiance_intermediate_b = m_3d_rt_radiance_intermediate_b->get_texture( );

	m_rsm_source_size = options::ref( ).current.m_light_propagation_volumes_rsm_size;
	m_rsm_downsampled_size = m_rsm_source_size / 2;

	m_rt_rms_albedo_source = resource_manager::ref( ).create_render_target( NULL, m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_albedo_source = m_rt_rms_albedo_source->get_texture( );
	m_rt_rms_normal_source = resource_manager::ref( ).create_render_target( NULL, m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_normal_source = m_rt_rms_normal_source->get_texture( );
	m_rt_rms_position_source = resource_manager::ref( ).create_render_target( NULL, m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_position_source = m_rt_rms_position_source->get_texture( );

	m_rt_rms_albedo_source_temp = resource_manager::ref( ).create_render_target( NULL, m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_albedo_source_temp = m_rt_rms_albedo_source_temp->get_texture( );
	m_rt_rms_normal_source_temp = resource_manager::ref( ).create_render_target( NULL, m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_normal_source_temp = m_rt_rms_normal_source_temp->get_texture( );
	m_rt_rms_position_source_temp = resource_manager::ref( ).create_render_target( NULL, m_rsm_source_size, m_rsm_source_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_position_source_temp = m_rt_rms_position_source_temp->get_texture( );

	m_rt_rms_albedo = resource_manager::ref( ).create_render_target( NULL, m_rsm_downsampled_size, m_rsm_downsampled_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_albedo = m_rt_rms_albedo->get_texture( );
	m_rt_rms_normal = resource_manager::ref( ).create_render_target( NULL, m_rsm_downsampled_size, m_rsm_downsampled_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_normal = m_rt_rms_normal->get_texture( );
	m_rt_rms_position = resource_manager::ref( ).create_render_target( NULL, m_rsm_downsampled_size, m_rsm_downsampled_size, DXGI_FORMAT_R16G16B16A16_FLOAT, enum_rt_usage_render_target, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );
	m_t_rms_position = m_rt_rms_position->get_texture( );

	m_radiance_depth_stencil = resource_manager::ref( ).create_render_target( NULL, in_num_cells, in_num_cells, DXGI_FORMAT_R16_TYPELESS, enum_rt_usage_depth_stencil, res_texture_ptr( ), 0, D3D11_USAGE_DEFAULT, 1, 0 );

	m_c_rsm_size = backend::ref( ).register_constant_host( "rsm_size", rc_int );
	m_c_grid_size = backend::ref( ).register_constant_host( "grid_size", rc_float );
	m_c_grid_origin = backend::ref( ).register_constant_host( "grid_origin", rc_float );
	m_c_grid_origin_and_inv_grid_scale = backend::ref( ).register_constant_host( "grid_origin_and_inv_grid_scale", rc_float );
	m_c_grid_cell_size = backend::ref( ).register_constant_host( "grid_cell_size", rc_float );
	m_c_num_grid_cells = backend::ref( ).register_constant_host( "num_grid_cells", rc_float );
	m_c_light_direction = backend::ref( ).register_constant_host( "light_direction", rc_float );
	m_c_light_position = backend::ref( ).register_constant_host( "light_position", rc_float );
	m_c_propagate_iteration_index = backend::ref( ).register_constant_host( "propagate_iteration_index", rc_int );
	m_c_inject_flux_weight = backend::ref( ).register_constant_host( "inject_flux_weight", rc_float );
	m_c_flux_amplifier = backend::ref( ).register_constant_host( "flux_amplifier", rc_float );
	m_c_eye_ray_corner = backend::ref( ).register_constant_host( "s_eye_ray_corner", rc_float );
	m_c_cascade_index = backend::ref( ).register_constant_host( "cascade_index", rc_int );
	m_c_occlusion_amplifier = backend::ref( ).register_constant_host( "occlusion_amplifier", rc_float );

	effect_manager::ref( ).create_effect< effect_light_propagation_volumes >( &m_lpv_effect );
}

radiance_volume::~radiance_volume( )
{
	// FUNCTION BODY[0x5ef340]
}

bool radiance_volume::is_effects_ready( ) const
{
	// FUNCTION BODY[0x5eebf0]
	return m_lpv_effect.c_ptr() != NULL;
}

void radiance_volume::set_origin( float3 const& in_origin )
{
	// claude@NOTE: legacy body diverged - legacy set_origin rebuilds m_bbox from scale+origin, no origin-history trio; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5eecc0]
	m_prev_previous_origin = m_previous_origin;
	m_previous_origin = m_next_origin;
	m_next_origin = in_origin;
}

float3 const& radiance_volume::get_origin( ) const
{
	// claude@NOTE: legacy body diverged - legacy get_origin returns m_bbox.min; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5eebe0]
	return m_next_origin;
}

float3 const& radiance_volume::get_previous_origin( ) const
{
	// claude@NOTE: no legacy ancestor - no origin history in the legacy radiance_volume; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x5eebd0]
	return m_previous_origin;
}

float radiance_volume::get_scale( ) const
{
	// FUNCTION BODY[0x5eebc0]
	return m_scale;
}

u32 radiance_volume::get_num_cells( ) const
{
	// FUNCTION BODY[0x5eebb0]
	return m_num_cells;
}

void radiance_volume::prepare_gv( )
{
	// FUNCTION BODY[0x5eec00]
	m_is_position_changed	= false;

//	if (!math::is_similar(m_prev_position, m_bbox.min, math::epsilon_3) || !options::ref().current.m_lpv_gather_occluders_from_camera_view)
	{
		m_is_position_changed	= true;
		m_prev_position			= m_bbox.min;

		backend::ref().set_render_targets		(&*m_3d_rt_occluders, 0, 0, 0);
		backend::ref().clear_render_targets		(0.0f, 0.0f, 0.0f, 0.0f);
		backend::ref().reset_render_targets		();
	}
}

void radiance_volume::prepare( float3 const& view_position, float3 const& view_direction, float offset_from_center )
{
	// FUNCTION BODY[0x5eedc0]
	backend::ref().set_render_targets		(&*m_3d_rt_radiance_r, &*m_3d_rt_radiance_g, &*m_3d_rt_radiance_b, 0);
	backend::ref().clear_render_targets		(0.0f, 0.0f, 0.0f, 0.0f);

	backend::ref().set_render_targets		(&*m_3d_rt_radiance_intermediate_r, &*m_3d_rt_radiance_intermediate_g, &*m_3d_rt_radiance_intermediate_b, 0);
	backend::ref().clear_render_targets		(0.0f, 0.0f, 0.0f, 0.0f);

	backend::ref().set_render_targets		(&*m_3d_rt_accumulated_propagation_r, &*m_3d_rt_accumulated_propagation_g, &*m_3d_rt_accumulated_propagation_b, 0);
	backend::ref().clear_render_targets		(0.0f, 0.0f, 0.0f, 0.0f);

	float const cell_size					=	get_scale() / float(get_num_cells());

	float3 cascade_origin					=	view_position - float3(0.5f, 0.5f, 0.5f) * get_scale();
	cascade_origin							+=	view_direction * get_scale() * offset_from_center;
	cascade_origin							/=  cell_size;
	cascade_origin.x						=	float(math::floor(cascade_origin.x));
	cascade_origin.y						=	float(math::floor(cascade_origin.y));
	cascade_origin.z						=	float(math::floor(cascade_origin.z));
	cascade_origin							*=  cell_size;

	if (!options::ref().current.m_lpv_movable)
		cascade_origin						=	float3(0.0f, 0.0f, 0.0f);

	set_origin								(cascade_origin);
}

void radiance_volume::begin_render_to_cells( )
{
	// FUNCTION BODY[0x5ef050]
	backend::ref().get_viewport				(m_saved_viewport);
	backend::ref().set_depth_stencil_target	(NULL);

	D3D11_VIEWPORT							cells_viewport;

	cells_viewport.TopLeftX					= 0.0f;
	cells_viewport.TopLeftY					= 0.0f;
	cells_viewport.Width					= float(m_num_cells);
	cells_viewport.Height					= float(m_num_cells);
	cells_viewport.MinDepth					= 0;
	cells_viewport.MaxDepth					= 1.f;

	backend::ref().set_viewport				(cells_viewport);
}

void radiance_volume::end_render_to_cells( )
{
	// FUNCTION BODY[0x5ef000]
	backend::ref().reset_render_targets		();
	backend::ref().reset_depth_stencil_target();
	backend::ref().set_viewport				(m_saved_viewport);
}

void radiance_volume::fill_previous_result( )
{
	// FUNCTION BODY[0x5eeba0]
	//resource_manager::ref().copy3D(&*m_3d_t_previous_radiance_r, 0, 0, 0, &*m_3d_t_accumulated_propagation_r, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
	//resource_manager::ref().copy3D(&*m_3d_t_previous_radiance_g, 0, 0, 0, &*m_3d_t_accumulated_propagation_g, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
	//resource_manager::ref().copy3D(&*m_3d_t_previous_radiance_b, 0, 0, 0, &*m_3d_t_accumulated_propagation_b, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
}

void radiance_volume::prepare_final( )
{
	// FUNCTION BODY[0x5f1620]
	begin_render_to_cells( );

	backend::ref( ).set_render_targets(
		&*m_3d_rt_radiance_r_apply,
		&*m_3d_rt_radiance_g_apply,
		&*m_3d_rt_radiance_b_apply,
		0
	);
	backend::ref( ).clear_render_targets( 0.0f, 0.0f, 0.0f, 0.0f );

	backend::ref( ).set_render_targets(
		&*m_3d_rt_radiance_r_apply,
		&*m_3d_rt_radiance_g_apply,
		&*m_3d_rt_radiance_b_apply,
		0
	);
	m_lpv_effect->apply( 5, 0 );
	backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_accumulated_propagation_r );
	backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_accumulated_propagation_g );
	backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_accumulated_propagation_b );
	m_sliced_cube_geometry.draw( );

	end_render_to_cells( );

	m_prev_previous_origin = m_previous_origin;
	m_previous_origin = m_bbox.min;
}

void radiance_volume::propagate_lighting_iter(
	u32 const cascade_index,
	u32 const iteration_index
)
{
	// FUNCTION BODY[0x5f1090]
	begin_render_to_cells( );

	if ( iteration_index == 0 )
	{
		backend::ref( ).set_render_targets(
			&*m_3d_rt_radiance_intermediate_r,
			&*m_3d_rt_radiance_intermediate_g,
			&*m_3d_rt_radiance_intermediate_b,
			0
		);
		m_lpv_effect->apply( 5, 0 );
		backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_radiance_r );
		backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_radiance_g );
		backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_radiance_b );
		m_sliced_cube_geometry.draw( );
	}

	if ( iteration_index % 2 == 0 )
	{
		backend::ref( ).set_render_targets(
			&*m_3d_rt_accumulated_propagation_r,
			&*m_3d_rt_accumulated_propagation_g,
			&*m_3d_rt_accumulated_propagation_b,
			0
		);
		m_lpv_effect->apply( 4, 0 );
		backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_radiance_r );
		backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_radiance_g );
		backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_radiance_b );
		backend::ref( ).set_ps_texture( "t_occluders", &*m_3d_t_occluders );
		backend::ref( ).set_ps_constant( m_c_num_grid_cells, float( m_num_cells ) );
		backend::ref( ).set_ps_constant( m_c_propagate_iteration_index, iteration_index );
		backend::ref( ).set_ps_constant( m_c_flux_amplifier, m_flux_amplifier );
		backend::ref( ).set_ps_constant( m_c_cascade_index, cascade_index );
		backend::ref( ).set_ps_constant(
			m_c_occlusion_amplifier,
			options::ref( ).current.m_lpv_occlusion_amplifier
		);
	}
	else
	{
		backend::ref( ).set_render_targets(
			&*m_3d_rt_radiance_r,
			&*m_3d_rt_radiance_g,
			&*m_3d_rt_radiance_b,
			0
		);
		m_lpv_effect->apply( 4, 0 );
		backend::ref( ).set_gs_constant( m_c_grid_size, float( m_num_cells ) );
		backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_radiance_intermediate_r );
		backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_radiance_intermediate_g );
		backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_radiance_intermediate_b );
		backend::ref( ).set_ps_texture( "t_occluders", &*m_3d_t_occluders );
		backend::ref( ).set_ps_constant( m_c_num_grid_cells, float( m_num_cells ) );
		backend::ref( ).set_ps_constant( m_c_propagate_iteration_index, iteration_index );
		backend::ref( ).set_ps_constant( m_c_flux_amplifier, m_flux_amplifier );
		backend::ref( ).set_ps_constant( m_c_cascade_index, cascade_index );
		backend::ref( ).set_ps_constant(
			m_c_occlusion_amplifier,
			options::ref( ).current.m_lpv_occlusion_amplifier
		);
	}

	m_sliced_cube_geometry.draw( );
	end_render_to_cells( );
}

void radiance_volume::propagate_lighting( u32 const cascade_index )
{
	// FUNCTION BODY[0x5f0a30]
	begin_render_to_cells( );

	backend::ref( ).set_render_targets(
		&*m_3d_rt_radiance_intermediate_r,
		&*m_3d_rt_radiance_intermediate_g,
		&*m_3d_rt_radiance_intermediate_b,
		0
	);
	m_lpv_effect->apply( 5, 0 );
	backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_radiance_r );
	backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_radiance_g );
	backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_radiance_b );
	m_sliced_cube_geometry.draw( );

	for ( u32 iteration_index = 0; iteration_index < m_num_propagate_iterations; ++iteration_index )
	{
		if ( iteration_index % 2 == 0 )
		{
			backend::ref( ).set_render_targets(
				&*m_3d_rt_accumulated_propagation_r,
				&*m_3d_rt_accumulated_propagation_g,
				&*m_3d_rt_accumulated_propagation_b,
				0
			);
			m_lpv_effect->apply( 4, 0 );
			backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_radiance_r );
			backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_radiance_g );
			backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_radiance_b );
			backend::ref( ).set_ps_texture( "t_occluders", &*m_3d_t_occluders );
			backend::ref( ).set_ps_constant( m_c_num_grid_cells, float( m_num_cells ) );
		}
		else
		{
			backend::ref( ).set_render_targets(
				&*m_3d_rt_radiance_r,
				&*m_3d_rt_radiance_g,
				&*m_3d_rt_radiance_b,
				0
			);
			m_lpv_effect->apply( 4, 0 );
			backend::ref( ).set_gs_constant( m_c_grid_size, float( m_num_cells ) );
			backend::ref( ).set_ps_texture( "t_radiance_r", &*m_3d_t_radiance_intermediate_r );
			backend::ref( ).set_ps_texture( "t_radiance_g", &*m_3d_t_radiance_intermediate_g );
			backend::ref( ).set_ps_texture( "t_radiance_b", &*m_3d_t_radiance_intermediate_b );
			backend::ref( ).set_ps_texture( "t_occluders", &*m_3d_t_occluders );
			backend::ref( ).set_ps_constant( m_c_num_grid_cells, float( m_num_cells ) );
		}

		backend::ref( ).set_ps_constant( m_c_propagate_iteration_index, iteration_index );
		backend::ref( ).set_ps_constant( m_c_flux_amplifier, m_flux_amplifier );
		backend::ref( ).set_ps_constant( m_c_cascade_index, cascade_index );
		backend::ref( ).set_ps_constant(
			m_c_occlusion_amplifier,
			options::ref( ).current.m_lpv_occlusion_amplifier
		);
		m_sliced_cube_geometry.draw( );
	}

	end_render_to_cells( );
}

void radiance_volume::inject_occluder_geometry(
	renderer_context*		context,
	float3 const&			light_position,
	float3 const&			light_direction,
	vector<float4x4> const&	transforms
)
{
	// FUNCTION BODY[0x5f07c0]
	if ( !transforms.size( ) )
		return;

	backend::ref( ).set_render_targets( &*m_3d_rt_occluders, 0, 0, 0 );
	begin_render_to_cells( );

	m_lpv_effect->apply( 3, 0 );

	backend::ref( ).set_vs_constant( m_c_grid_origin_and_inv_grid_scale, float4( m_bbox.min, 1.0f / get_scale( ) ) );

	backend::ref( ).set_gs_constant( m_c_grid_size, float( m_num_cells ) );
	backend::ref( ).set_gs_constant( m_c_light_position, light_position );
	backend::ref( ).set_gs_constant( m_c_light_direction, light_direction );
	backend::ref( ).set_ps_constant( m_c_light_position, light_position );

	vector<float4x4>::const_iterator it = transforms.begin( );
	vector<float4x4>::const_iterator end = transforms.end( );

	for ( ; it != end; ++it )
	{
		context->set_w( *it );
		m_box_geometry.draw( );
	}

	end_render_to_cells( );
}

void radiance_volume::inject_occluders(
	renderer_context*	context,
	float3 const&		light_position,
	float3 const&		light_direction,
	u32					rsm_size
)
{
	// FUNCTION BODY[0x5f0580]
	VOSTOK_UNREFERENCED_PARAMETER(rsm_size);

	// Inject occluders from light view.
	if (options::ref().current.m_lpv_gather_occluders_from_light_view)
	{
		backend::ref().set_render_targets		(&*m_3d_rt_occluders, 0, 0, 0);
		begin_render_to_cells					();

		m_lpv_effect->apply						(1, 0); // inject_occluders_light_view_stage

		backend::ref().set_vs_constant			(m_c_grid_origin_and_inv_grid_scale, float4(m_bbox.min, 1.0f / get_scale()));

		backend::ref().set_gs_constant			(m_c_grid_size, float(m_num_cells));
		backend::ref().set_gs_constant			(m_c_light_position, light_position);
		backend::ref().set_gs_constant			(m_c_light_direction, light_direction);
		backend::ref().set_ps_constant			(m_c_light_position, light_position);
		backend::ref().set_ps_texture			("t_lpv_rsm_normal", &*m_t_rms_normal);
		backend::ref().set_ps_texture			("t_lpv_rsm_position", &*m_t_rms_position);

		m_injection_geometry.draw				();

		end_render_to_cells						();
	}
/*

	// Inject occluders from camera view.
	if (m_is_position_changed && options::ref().m_lpv_gather_occluders_from_camera_view)
	{
		backend::ref().set_render_targets		(&*m_3d_rt_occluders, 0, 0, 0);
		begin_render_to_cells					();

		m_lpv_effect->apply						(effect_light_propagation_volumes::inject_occluders_camera_view_stage);

		backend::ref().set_vs_constant			(m_c_grid_origin, m_bbox.min);
		backend::ref().set_vs_constant			(m_c_grid_cell_size, m_scale / float(m_num_cells));
		backend::ref().set_vs_constant			(m_c_num_grid_cells, float(m_num_cells));

		backend::ref().set_gs_constant			(m_c_grid_size, float(m_num_cells));
		backend::ref().set_gs_constant			(m_c_light_position, light_position);
		backend::ref().set_gs_constant			(m_c_light_direction, light_direction);

		float3 const* const eye_rays			= context->get_eye_rays();
		backend::ref().set_vs_constant			(m_c_eye_ray_corner,	((float4*)eye_rays)[0]);

		m_injection_geometry_from_camera.draw	();

		end_render_to_cells						();
	}
	*/
#ifndef MASTER_GOLD
	if (options::ref().current.m_lpv_gather_occluders_from_light_view ||
		options::ref().current.m_lpv_gather_occluders_from_camera_view)
	{
		//resource_manager::ref().copy3D			(&*m_3d_t_occluders_lockable, 0, 0, 0, &*m_3d_t_occluders, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
	}
#endif // #ifndef MASTER_GOLD
}

void radiance_volume::inject_camera_occluders( renderer_context* context )
{
	// FUNCTION BODY[0x5f0320]
	// Inject occluders from camera view.
	if (m_is_position_changed && options::ref().current.m_lpv_gather_occluders_from_camera_view)
	{
		backend::ref().set_render_targets		(&*m_3d_rt_occluders, 0, 0, 0);
		begin_render_to_cells					();

		m_lpv_effect->apply						(2, 0); // inject_occluders_camera_view_stage

		backend::ref().set_vs_constant			(m_c_grid_origin, m_bbox.min);
		backend::ref().set_vs_constant			(m_c_grid_cell_size, m_scale / float(m_num_cells));
		backend::ref().set_vs_constant			(m_c_num_grid_cells, float(m_num_cells));

		backend::ref().set_gs_constant			(m_c_grid_size, float(m_num_cells));

		float3 const* const eye_rays			= context->get_eye_rays();
		backend::ref().set_vs_constant			(m_c_eye_ray_corner,	((float4*)eye_rays)[0]);

		context->set_v(context->get_v());
		context->set_p(context->get_p());

		m_injection_geometry_from_camera.prepare(
			context->m_targets->m_family[rt_gbuffer_position_downsampled].texture->width(),
			context->m_targets->m_family[rt_gbuffer_position_downsampled].texture->height()
		);

		m_injection_geometry_from_camera.draw	();

		end_render_to_cells						();
	}

// #ifndef MASTER_GOLD
// 	if (options::ref().m_lpv_gather_occluders_from_light_view ||
// 		options::ref().m_lpv_gather_occluders_from_camera_view)
// 	{
// 		resource_manager::ref().copy3D			(&*m_3d_t_occluders_lockable, 0, 0, 0, &*m_3d_t_occluders, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
// 	}
// #endif // #ifndef MASTER_GOLD
}

void radiance_volume::inject_lighting(
	float3 const&	light_position,
	float3 const&	light_direction,
	float			light_fov,
	u32				rsm_size
)
{
	// FUNCTION BODY[0x5f0060]
	backend::ref().set_render_targets		(&*m_3d_rt_radiance_r, &*m_3d_rt_radiance_g, &*m_3d_rt_radiance_b, 0);

	begin_render_to_cells					();

	m_lpv_effect->apply						(0, 0); // inject_lighting_stage

	backend::ref().set_vs_constant			(m_c_grid_origin_and_inv_grid_scale, float4(m_bbox.min, 1.0f / get_scale()));
	backend::ref().set_gs_constant			(m_c_grid_size, float(m_num_cells));
	backend::ref().set_gs_constant			(m_c_light_position, light_position);
	backend::ref().set_gs_constant			(m_c_light_direction, light_direction);

	// use or not?
	backend::ref().set_ps_constant			(m_c_light_position, light_position);
	backend::ref().set_ps_constant			(m_c_light_direction, light_direction);

	float aspect_ratio						= 1.0f;

	float tan_fovy_half						= math::tan(light_fov / 2.0f);
	float tan_fovx_half						= math::tan(light_fov / 2.0f) * aspect_ratio;

	float flux_weight						= 4.0f * tan_fovy_half * tan_fovx_half / float(rsm_size * rsm_size);

	flux_weight								*= 30.0f;

	backend::ref().set_ps_constant			(m_c_inject_flux_weight, flux_weight);

	m_injection_geometry.draw				();

#ifndef MASTER_GOLD
	// for show VPL
	//resource_manager::ref().copy3D			(&*m_3d_t_radiance_r_lockable, 0, 0, 0, &*m_3d_t_radiance_r, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
	//resource_manager::ref().copy3D			(&*m_3d_t_radiance_g_lockable, 0, 0, 0, &*m_3d_t_radiance_g, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
	//resource_manager::ref().copy3D			(&*m_3d_t_radiance_b_lockable, 0, 0, 0, &*m_3d_t_radiance_b, 0, 0, 0, m_num_cells, m_num_cells, m_num_cells);
#endif // #ifndef MASTER_GOLD

	end_render_to_cells						();
}

} // namespace render
} // namespace vostok
