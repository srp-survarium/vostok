#include "pch.h"
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

backend::backend( ) :
	vertex( 1536 * 1024 ),
	index( 512 * 1024 ),
	m_vs_textures_handler( m_vs ),
	m_vs_samplers_handler( m_vs ),
	m_gs_textures_handler( m_gs ),
	m_gs_samplers_handler( m_gs ),
	m_ps_textures_handler( m_ps ),
	m_ps_samplers_handler( m_ps )
{
	// FUNCTION BODY[0x560260]
	// claude@NOTE: legacy init-list zeroed ~20 more members (m_device via
	// device::ref().d3d_device(), state/buffer/shader pointers, counters);
	// reconcile the canonical member inits against 0x560260 at matcher phase.
	for ( int i = 0; i < enum_shader_types_count; ++i )
		m_constant_update_markers[i] = 1;

	m_device->AddRef			( );
	reset						( );
}

backend::~backend( )
{
	// FUNCTION BODY[0x55fa10]
	for( vector< shader_constant_host* >::iterator it = m_constant_hosts.begin(); it< m_constant_hosts.end(); ++it)
		DELETE( *it);

	on_device_destroy();
}

void backend::clear_depth_stencil( u32, float, u8 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f5b0]
}

void backend::clear_render_targets( float, float, float, float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f530]
}

void backend::clear_render_targets( math::color )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f860]
}

void backend::clear_render_targets(
	math::color,
	math::color,
	math::color,
	math::color
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f5f0]
}

void backend::on_device_destroy( )
{
	// FUNCTION BODY[0x55f510]
	safe_release(m_device);
	m_device = 0;

// 	vertex.destroy();
// 	index.destroy();
}

bool sorted_vector_predicate( shader_constant_host const* first, shared_string const& second )
{
	// FUNCTION BODY[0x55f4f0]
	return first->name() < second;
}

shader_constant_host* backend::register_constant_host( shared_string const& name, enum_constant_type const type )
{
	// FUNCTION BODY[0x55f930]
	vector< shader_constant_host* >::iterator it = std::lower_bound( m_constant_hosts.begin(), m_constant_hosts.end(), name, sorted_vector_predicate);

	if ( it == m_constant_hosts.end() || !((*it)->name() == name))
		return *m_constant_hosts.insert( it, NEW (shader_constant_host)(name, type));
	else
		return *it;
}

void backend::update_input_layout( )
{
	// FUNCTION BODY[0x55f9b0]
	// Set always
	//if( m_dirty_objects.input_layout)
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
	// FUNCTION BODY[0x55f4c0]
	ID3DRenderTargetView * rt_view = (rt == NULL) ? NULL : rt->get_target_view();

	if( m_targets[target] != rt_view)
	{
		m_targets[target] = rt_view;
		m_dirty_targets.render_targets[target] = true;
	}
}

void start_profiling( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f4b0]
}

double end_profiling( pcstr, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x55f4a0]
	return 0.0;
}

void backend::flush( )
{
	// FUNCTION BODY[0x55fd70]
	//Here may be used caching to prevent reseting the same state.
	if( m_dirty_objects.rasterizer_state)
		device::ref().d3d_context()->RSSetState				( m_rasterizer_state);

	if( m_dirty_objects.depth_stencil_state)
		device::ref().d3d_context()->OMSetDepthStencilState	( m_depth_stencils_state, m_stencil_ref);

	if( m_dirty_objects.effect_state)
	{
		// --Porting to DX10_
		// give correct blend factors
		float blend_factor[4]  = {0.f,0.f,0.f,0.f} ;
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
		
		m_constant_update_counter++;

		m_dirty_objects.reset();
//	}
}

void backend::render_indexed( D3D_PRIMITIVE_TOPOLOGY, u32, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5601b0]
}

void backend::render( D3D_PRIMITIVE_TOPOLOGY, u32, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x560130]
}

} // namespace render
} // namespace vostok
