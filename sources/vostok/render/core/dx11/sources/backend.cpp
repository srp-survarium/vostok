// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/shader_constant_host.h>
#include <vostok/render/core/dx11/render_target.h>
#include <vostok/render/core/dx11/res_declaration.h>
#include <vostok/render/core/dx11/res_input_layout.h>
#include <vostok/render/core/dx11/res_xs_hw.h>
#include <vostok/render/core/untyped_buffer.h>
#include "com_utils.h"
namespace vostok {
namespace render {

static u32 s_max_triagles_per_dip_value = 1000000;
static console_commands::cc_u32 s_max_triagles_per_dip(
	"max_triagles_per_dip",
	s_max_triagles_per_dip_value,
	0,
	100000,
	false,
	console_commands::command_type_user_specific
);

static bool s_debug_enabled_ds_clearing_value = true;
static console_commands::cc_bool s_debug_enabled_ds_clearing_value_cc(
	"r_debug_enabled_ds_clearing",
	s_debug_enabled_ds_clearing_value,
	false,
	console_commands::command_type_user_specific
);

static bool s_debug_enabled_rt_clearing_value = true;
static console_commands::cc_bool s_debug_enabled_rt_clearing_cc(
	"s_debug_enabled_rt_clearing",
	s_debug_enabled_rt_clearing_value,
	false,
	console_commands::command_type_user_specific
);

backend::backend( ) :
	num_vs_changes					( 0 ),
	num_ps_changes					( 0 ),
	num_il_changes					( 0 ),
	num_vsc_changes					( 0 ),
	num_vst_changes					( 0 ),
	num_vss_changes					( 0 ),
	num_psc_changes					( 0 ),
	num_pst_changes					( 0 ),
	num_pss_changes					( 0 ),
	max_triagles_per_dip			( false ),
	disable_DrawIndexed				( false ),
	m_set_ps_sources				( true ),
	allow_debug_profile_dip			( false ),
	vertex							( 256 * 1024 ),
	index							( 256 * 1024 ),
	num_total_rendered_triangles	( 0 ),
	num_total_rendered_points		( 0 ),
	num_setted_shader_constants		( 0 ),
	disabled_shader_constansts_set	( false ),
	num_draw_calls					( 0 ),
	draw_calls_counting				( true ),
	m_user_output					( false ),
	m_user_output_width				( 1 ),
	m_user_output_height			( 1 ),
	m_device						( device::ref().d3d_device() ),
	m_rasterizer_state				( NULL ),
	m_depth_stencils_state			( NULL ),
	m_effect_state					( NULL ),
	m_stencil_ref					( 0x00 ),
	m_sample_mask					( 0xFFFFFFFF ),
	m_vb							( NULL ),
	m_ib							( NULL ),
	m_vs							( NULL ),
	m_ps							( NULL ),
	m_gs							( NULL ),
	m_vs_textures_handler			( m_vs ),
	m_vs_samplers_handler			( m_vs ),
	m_gs_textures_handler			( m_gs ),
	m_gs_samplers_handler			( m_gs ),
	m_ps_textures_handler			( m_ps ),
	m_ps_samplers_handler			( m_ps ),
	m_decl							( NULL ),
	m_input_layout					( NULL ),
	m_render_output					( NULL ),
	m_texture_compression_time		( 0.f ),
	m_cpu_compression_time			( 0.f ),
	m_cpu_num_compressed_textures	( 0 ),
	m_gpu_num_compressed_textures	( 0 ),
	m_vb_stride						( 0 ),
	m_vb_offset						( 0 ),
	m_vb_stride_instance_data		( 0 ),
	m_vb_offset_instance_data		( 0 ),
	m_vb_stride_stream_1			( 0 ),
	m_vb_offset_stream_1			( 0 ),
	m_constant_update_counter		( 1 )
{
	for ( int i = 0; i < enum_shader_types_count; ++i )
		m_constant_update_markers[i] = 1;

	m_device->AddRef			( );
	reset						( );
}

backend::~backend( )
{
	for( vector< shader_constant_host* >::iterator it = m_constant_hosts.begin(); it< m_constant_hosts.end(); ++it)
		DELETE( *it);

	on_device_destroy();
}

void backend::clear_depth_stencil( u32 flags, float z_value, u8 stencil_value )
{
 	//flush_rt();
	if( s_debug_enabled_ds_clearing_value)
	{
		if (m_zb)
		{
			device::ref().d3d_context()->ClearDepthStencilView( m_zb, flags, z_value, stencil_value);
		}
	}
}

void backend::clear_render_targets( float r, float g, float b, float a )
{
	if( s_debug_enabled_rt_clearing_value)
	{
		float color_elements[4] = {r, g, b, a};

		for( int i = 0; i< enum_target_count; ++i)
			if( m_targets[i])
				device::ref().d3d_context()->ClearRenderTargetView( m_targets[i], color_elements );
	}
}

// The target reads channels in union order; math_color accessors currently reverse R/B.
void backend::clear_render_targets( math::color color )
{
	if( s_debug_enabled_rt_clearing_value)
	{
		float color_elements[4];
		color.get_RGBA(color_elements[0], color_elements[1], color_elements[2], color_elements[3] );

		for( int i = 0; i< enum_target_count; ++i)
			if( m_targets[i])
				device::ref().d3d_context()->ClearRenderTargetView( m_targets[i], color_elements );
	}
}

void backend::clear_render_targets(
	math::color color0,
	math::color color1,
	math::color color2,
	math::color color3
)
{
	if( s_debug_enabled_rt_clearing_value)
	{
		COMPILE_ASSERT	( enum_target_count == 4, Unknown_render_targets_count );

		float color_elements[4];

		if( m_targets[0])
		{
			color0.get_RGBA(color_elements[0], color_elements[1], color_elements[2], color_elements[3] );
			device::ref().d3d_context()->ClearRenderTargetView( m_targets[0], color_elements);
		}

		if( m_targets[1])
		{
			color1.get_RGBA(color_elements[0], color_elements[1], color_elements[2], color_elements[3] );
			device::ref().d3d_context()->ClearRenderTargetView( m_targets[1], color_elements);
		}

		if( m_targets[2])
		{
			color2.get_RGBA(color_elements[0], color_elements[1], color_elements[2], color_elements[3] );
			device::ref().d3d_context()->ClearRenderTargetView( m_targets[2], color_elements);
		}

		if( m_targets[3])
		{
			color3.get_RGBA(color_elements[0], color_elements[1], color_elements[2], color_elements[3] );
			device::ref().d3d_context()->ClearRenderTargetView( m_targets[3], color_elements);
		}
	}
}

void backend::on_device_destroy( )
{
	safe_release(m_device);
	m_device = 0;

// 	vertex.destroy();
// 	index.destroy();
}

static bool sorted_vector_predicate( shader_constant_host const* first, shared_string const& second )
{
	return first->name() < second;
}

shader_constant_host* backend::register_constant_host( shared_string const& name, enum_constant_type const type )
{
	vector< shader_constant_host* >::iterator it = std::lower_bound( m_constant_hosts.begin(), m_constant_hosts.end(), name, sorted_vector_predicate);

	if ( it == m_constant_hosts.end() || !((*it)->name() == name))
		return *m_constant_hosts.insert( it, NEW (shader_constant_host)(name, type));
	else
		return *it;
}

shader_constant_host const* backend::find_constant_host( shared_string const& name, enum_constant_type const type, bool const create_if_missing )
{
	vector< shader_constant_host* >::iterator it = std::lower_bound( m_constant_hosts.begin(), m_constant_hosts.end(), name, sorted_vector_predicate);

	if ( it == m_constant_hosts.end() || !(**it == name) )
	{
		if( !create_if_missing)
			return  NULL;
		else
			return register_constant_host( name, type );
	}
	else
		return *it;
}

void backend::reset_constant_update_markers( )
{
	LOG_INFO("reset_constant_update_markers called !!!");
	vector< shader_constant_host* >::iterator		it =	m_constant_hosts.begin();
	vector< shader_constant_host* >::const_iterator	end =	m_constant_hosts.end();

	for( ; it!= end; ++it)
		(*it)->reset_update_markers	( );

	m_constant_update_counter = 1;
}

void backend::update_input_layout( )
{
	if( m_dirty_objects.input_layout && m_vs)
	{
		if( !m_input_layout)
		{
			ASSERT( m_decl, "It is needed to define either input layout or a vertex declaration.");
			m_input_layout = m_decl->get( &*m_vs->data().signature);
		}

		ASSERT( m_input_layout);
		device::ref().d3d_context()->IASetInputLayout( m_input_layout->hardware_layout());
	}
}

void backend::set_render_target( enum_render_target_enum target, render_target const* rt )
{
	ID3DRenderTargetView * rt_view = (rt == NULL) ? NULL : rt->get_target_view();

	if( m_targets[target] != rt_view)
	{
		m_targets[target] = rt_view;
		m_dirty_targets.render_targets[target] = true;
	}
}

static timing::timer s_timer;

void start_profiling( )
{
}

double end_profiling( pcstr, bool )
{
	return 0.0f;
}

void backend::flush( )
{
	//Here may be used caching to prevent reseting the same state.
	if( m_dirty_objects.rasterizer_state)
		device::ref().d3d_context()->RSSetState				( m_rasterizer_state);

	if( m_dirty_objects.depth_stencil_state)
		device::ref().d3d_context()->OMSetDepthStencilState	( m_depth_stencils_state, m_stencil_ref);

	if( m_dirty_objects.effect_state)
	{
		// --Porting to DX10_
		// give correct blend factors
		float blend_factor[4]  = {1.f,1.f,1.f,1.f} ;
		device::ref().d3d_context()->OMSetBlendState			( m_effect_state, blend_factor, m_sample_mask);
	}

	flush_rt();

// 	if( m_dirty_objects.any())
// 	{
		if( m_dirty_objects.vertex_shader)
		{
			ID3DVertexShader * vs	= (m_vs == (res_vs_hw*)NULL) ? NULL : m_vs->hardware_shader();
			device::ref().d3d_context()->VSSetShader( vs SHADER_PARAM_STUB);
		}
		
		if( m_dirty_objects.geometry_shader)
		{
			ID3DGeometryShader * gs	= (m_gs == (res_gs_hw*)NULL) ? NULL : m_gs->hardware_shader();
			device::ref().d3d_context()->GSSetShader( gs SHADER_PARAM_STUB);
		}
		
		if( m_dirty_objects.pixel_shader)
		{
			ID3DPixelShader * ps	= (m_ps == (res_ps_hw*)NULL) ? NULL : m_ps->hardware_shader();
			device::ref().d3d_context()->PSSetShader( ps SHADER_PARAM_STUB);
		}

		if( m_dirty_objects.vertex_buffer || m_dirty_objects.vertex_buffer_instance_data)
		{
			if (m_dirty_objects.vertex_buffer_instance_data)
			{
				ID3DVertexBuffer*	buffer0		=	(m_vb == (untyped_buffer*)NULL) ? NULL : m_vb->hardware_buffer();
				ID3DVertexBuffer*	buffer1		=	(m_vb_instance_data == (untyped_buffer*)NULL) ? NULL : m_vb_instance_data->hardware_buffer();
				
				ID3DVertexBuffer*	buffers[2]	=	{buffer0, buffer1};
				u32					strides[2]	=	{m_vb_stride, m_vb_stride_instance_data};
				u32					offsets[2]	=	{m_vb_offset, m_vb_offset_instance_data};
				
				device::ref().d3d_context()->IASetVertexBuffers(0, 2, buffers, strides, offsets);
			}
			else
			{
				ID3DVertexBuffer * buffer = (m_vb == (untyped_buffer*)NULL) ? NULL : m_vb->hardware_buffer();
				device::ref().d3d_context()->IASetVertexBuffers( 0, 1, &buffer, &m_vb_stride, &m_vb_offset);
			}
		}

		if( m_dirty_objects.vertex_buffer_stream_1)
		{
			ID3DVertexBuffer * buffer = (m_vb_stream_1 == (untyped_buffer*)NULL) ? NULL : m_vb_stream_1->hardware_buffer();
			device::ref().d3d_context()->IASetVertexBuffers( 1, 1, &buffer, &m_vb_stride_stream_1, &m_vb_offset_stream_1);
		}

		if( m_dirty_objects.index_buffer)
		{
			ID3DIndexBuffer * buffer = (m_ib == (untyped_buffer*)NULL) ? NULL : m_ib->hardware_buffer();
			device::ref().d3d_context()->IASetIndexBuffer( buffer, DXGI_FORMAT_R16_UINT, m_ib_offset);
		}

		m_vs_constants_handler.update_buffers();
		m_gs_constants_handler.update_buffers();
		m_ps_constants_handler.update_buffers();

		if( m_dirty_objects.vertex_constants)
			m_vs_constants_handler.apply();

		if( m_dirty_objects.vertex_textures)
			m_vs_textures_handler.apply();

		if( m_dirty_objects.vertex_samplers)
			m_vs_samplers_handler.apply();

		if( m_dirty_objects.geometry_constants)
			m_gs_constants_handler.apply();

		if( m_dirty_objects.geometry_textures)
			m_gs_textures_handler.apply();

		if( m_dirty_objects.geometry_samplers)
			m_gs_samplers_handler.apply();
		
		if( m_dirty_objects.pixel_constants)
			m_ps_constants_handler.apply();

		if( m_dirty_objects.pixel_textures)
			m_ps_textures_handler.apply();

		if( m_dirty_objects.pixel_samplers)
			m_ps_samplers_handler.apply();

		if( m_dirty_objects.primitive_topology)
			device::ref().d3d_context()->IASetPrimitiveTopology( m_primitive_topology);

		m_constant_update_counter++;

		update_input_layout();

		m_dirty_objects.reset();
//	}
}

void backend::render_indexed( D3D_PRIMITIVE_TOPOLOGY type, u32 index_count, u32 start_index, u32 base_vertex )
{
	m_dirty_objects.primitive_topology	= type != m_primitive_topology;
	if( m_dirty_objects.primitive_topology)
		m_primitive_topology			= type;

	flush();

	if( draw_calls_counting)
		num_draw_calls			++;

	//stat.calls			++;
	//stat.verts			+= 3*PC;
	//stat.polys			+= PC;

	if( draw_calls_counting)
		index_count			= math::min( s_max_triagles_per_dip_value * 3, index_count);

	if( !disable_DrawIndexed)
		device::ref().d3d_context()->DrawIndexed(index_count, start_index, base_vertex);

	// TODO
	switch (type)
	{
		case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			num_total_rendered_triangles += index_count / 3;
			break;

		case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
			num_total_rendered_points += index_count;
			break;

		case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
			num_total_rendered_triangles += index_count / 2;
			break;
	}
}

void backend::render( D3D_PRIMITIVE_TOPOLOGY type, u32 vertex_count, u32 base_vertex )
{
	m_dirty_objects.primitive_topology	= type != m_primitive_topology;
	if( m_dirty_objects.primitive_topology)
		m_primitive_topology			= type;

	flush();

	if( draw_calls_counting)
		num_draw_calls			++;

	//stat.calls			++;
	//stat.verts			+= 3*PC;
	//stat.polys			+= PC;

	device::ref().d3d_context()->Draw( vertex_count,  base_vertex);

	// TODO
	switch (type)
	{
		case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
			num_total_rendered_triangles += vertex_count / 3;
			break;

		case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
			num_total_rendered_points += vertex_count;
			break;

		case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
			num_total_rendered_triangles += vertex_count / 2;
			break;
	}
}

} // namespace render
} // namespace vostok
