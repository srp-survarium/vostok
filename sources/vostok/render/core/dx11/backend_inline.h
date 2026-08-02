#ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BACKEND_INLINE_H_INCLUDED

namespace vostok {
namespace render {

inline void backend::set_rasterizer_state( ID3D11RasterizerState* state )
{
	m_dirty_objects.rasterizer_state |= m_rasterizer_state != state;
	m_rasterizer_state = state;
}

inline void backend::set_depth_stencil_state( ID3D11DepthStencilState* state )
{
	m_dirty_objects.depth_stencil_state |= m_depth_stencils_state != state;
	m_depth_stencils_state = state;
}

inline void backend::set_stencil_ref( u32 stencil_ref )
{
	m_dirty_objects.depth_stencil_state |= m_stencil_ref != stencil_ref;
	m_stencil_ref = stencil_ref;
}

inline void backend::set_blend_state( ID3D11BlendState* state )
{
	m_dirty_objects.effect_state |= m_effect_state != state;
	m_effect_state = state;
}

inline void backend::set_sample_mask( u32 sample_mask )
{
	m_dirty_objects.effect_state |= m_sample_mask != sample_mask;
	m_sample_mask = sample_mask;
}

inline shader_constant_host* backend::register_constant_host( shader_constant const& constant )
{
	return register_constant_host( constant.host().name(), constant.host().type() );
}

inline void backend::set_vb( untyped_buffer* vb, u32 vb_stride, u32 vb_offset )
{
	m_dirty_objects.vertex_buffer |= m_vb != vb || m_vb_stride != vb_stride || m_vb_offset != vb_offset;
	m_vb = vb;
	m_vb_stride = vb_stride;
	m_vb_offset = vb_offset;
}

inline void backend::set_vb_instance_data( untyped_buffer* vb, u32 vb_stride, u32 vb_offset )
{
	m_dirty_objects.vertex_buffer_instance_data |= m_vb_instance_data != vb
		|| m_vb_stride_instance_data != vb_stride || m_vb_offset_instance_data != vb_offset;
	m_vb_instance_data = vb;
	m_vb_stride_instance_data = vb_stride;
	m_vb_offset_instance_data = vb_offset;
}

inline void backend::set_vb_stream_1( untyped_buffer* vb, u32 vb_stride, u32 vb_offset )
{
	m_dirty_objects.vertex_buffer_stream_1 |= m_vb_stream_1 != vb
		|| m_vb_stride_stream_1 != vb_stride || m_vb_offset_stream_1 != vb_offset;
	m_vb_stream_1 = vb;
	m_vb_stride_stream_1 = vb_stride;
	m_vb_offset_stream_1 = vb_offset;
}

inline void backend::set_ib( untyped_buffer* buffer, u32 offset )
{
	m_dirty_objects.index_buffer |= m_ib != buffer || m_ib_offset != offset;
	m_ib = buffer;
	m_ib_offset = offset;
}

inline void backend::set_vs( res_vs_hw* shader )
{
	m_dirty_objects.vertex_shader |= m_vs != shader;
	if ( m_vs != shader )
		++num_vs_changes;
	m_vs = shader;
	if ( m_dirty_objects.vertex_shader )
		m_input_layout = 0;
	m_dirty_objects.input_layout = m_dirty_objects.vertex_shader;
}

inline void backend::set_ps( res_ps_hw* shader )
{
	m_dirty_objects.pixel_shader |= m_ps != shader;
	if ( m_ps != shader )
		++num_ps_changes;
	m_ps = shader;
}

inline void backend::set_gs( res_gs_hw* shader )
{
	m_dirty_objects.geometry_shader |= m_gs != shader;
	m_gs = shader;
}

inline void backend::set_declaration( res_declaration* declaration )
{
	bool const changed = m_decl != declaration;
	if ( changed ) {
		m_decl = declaration;
		m_dirty_objects.input_declaration = true;
		m_input_layout = 0;
	}
	m_dirty_objects.input_layout = changed;
}

inline void backend::set_input_layout( res_input_layout* layout )
{
	m_dirty_objects.input_layout |= m_input_layout != layout;
	m_input_layout = layout;
}

inline void backend::set_vs_constants( shader_constant_table* table )
{
	if ( m_vs_constants_handler.m_current == table )
		return;

	++num_vsc_changes;
	m_vs_constants_handler.assign( table );
	m_dirty_objects.vertex_constants = true;
	m_constant_update_markers[enum_shader_type_vertex] = m_constant_update_counter;
}

inline void backend::set_gs_constants( shader_constant_table* table )
{
	if ( m_gs_constants_handler.m_current == table )
		return;

	m_gs_constants_handler.assign( table );
	m_dirty_objects.geometry_constants = true;
	m_constant_update_markers[enum_shader_type_geometry] = m_constant_update_counter;
}

inline void backend::set_ps_constants( shader_constant_table* table )
{
	if ( m_ps_constants_handler.m_current == table )
		return;

	++num_psc_changes;
	m_ps_constants_handler.assign( table );
	m_dirty_objects.pixel_constants = true;
	m_constant_update_markers[enum_shader_type_pixel] = m_constant_update_counter;
}

inline void backend::set_vs_textures( res_texture_list* textures )
{
	if ( m_vs_textures_handler.m_current == textures )
		return;

	++num_vst_changes;
	m_vs_textures_handler.assign( textures );
	m_dirty_objects.vertex_textures = true;
}

inline void backend::set_gs_textures( res_texture_list* textures )
{
	if ( m_gs_textures_handler.m_current == textures )
		return;

	m_gs_textures_handler.assign( textures );
	m_dirty_objects.geometry_textures = true;
}

inline void backend::set_ps_textures( res_texture_list* textures )
{
	if ( m_ps_textures_handler.m_current == textures )
		return;

	++num_pst_changes;
	m_ps_textures_handler.assign( textures );
	m_dirty_objects.pixel_textures = true;
}

inline void backend::set_vs_texture( pcstr name, res_texture* texture )
{
	m_dirty_objects.vertex_textures |= m_vs_textures_handler.set_overwrite( name, texture );
}

inline void backend::set_gs_texture( pcstr name, res_texture* texture )
{
	m_dirty_objects.geometry_textures |= m_gs_textures_handler.set_overwrite( name, texture );
}

inline void backend::set_ps_texture( pcstr name, res_texture* texture )
{
	m_dirty_objects.pixel_textures = m_ps_textures_handler.set_overwrite( name, texture );
}

inline void backend::set_vs_samplers( res_sampler_list* samplers )
{
	if ( m_vs_samplers_handler.m_current == samplers )
		return;

	++num_vss_changes;
	m_vs_samplers_handler.assign( samplers );
	m_dirty_objects.vertex_samplers = true;
}

inline void backend::set_gs_samplers( res_sampler_list* samplers )
{
	if ( m_gs_samplers_handler.m_current == samplers )
		return;

	m_gs_samplers_handler.assign( samplers );
	m_dirty_objects.geometry_samplers = true;
}

inline void backend::set_ps_samplers( res_sampler_list* samplers )
{
	if ( m_ps_samplers_handler.m_current == samplers )
		return;

	++num_pss_changes;
	m_ps_samplers_handler.assign( samplers );
	m_dirty_objects.pixel_samplers = true;
}

inline void backend::set_render_targets(
	render_target const* rt0,
	render_target const* rt1,
	render_target const* rt2,
	render_target const* rt3
)
{
	set_render_target( enum_target_rt0, rt0 );
	set_render_target( enum_target_rt1, rt1 );
	set_render_target( enum_target_rt2, rt2 );
	set_render_target( enum_target_rt3, rt3 );
}

inline void backend::reset_render_targets( bool only_the_base )
{
	m_dirty_targets.render_targets[0] |= m_targets[0] != m_base_rt;
	m_targets[0] = m_base_rt;

	for ( u32 index = 1; index < enum_target_count && !only_the_base; ++index ) {
		m_dirty_targets.render_targets[index] |= m_targets[index] != 0;
		m_targets[index] = 0;
	}
}

inline void backend::reset_depth_stencil_target( )
{
	m_dirty_targets.depth_stencil |= m_zb != m_base_zb;
	m_zb = m_base_zb;
}

inline void backend::flush_rt_shader_resources( )
{
	ID3D11ShaderResourceView* rv[12] = { 0 };
	device::ref( ).d3d_context( )->PSSetShaderResources( 0, 12, rv );
	device::ref( ).d3d_context( )->VSSetShaderResources( 0, 12, rv );
	device::ref( ).d3d_context( )->GSSetShaderResources( 0, 12, rv );

	vostok::memory::zero( m_ps_textures_handler.m_tmp_buffer );
}

inline void backend::flush_rt_views( )
{
	ID3D11RenderTargetView* tv[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { 0 };
	device::ref( ).d3d_context( )->OMSetRenderTargets(
		D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, tv, 0
	);
}

inline void backend::set_depth_stencil_target( render_target const* zrt )
{
	ID3D11DepthStencilView* zrt_view = (zrt == NULL) ? NULL : zrt->get_depth_stencil_view();

	m_dirty_targets.depth_stencil |=	m_zb != zrt_view;
	m_zb = zrt_view;
}

inline void backend::flush_rt( )
{
	if ( m_dirty_targets.any( ) )
		device::ref( ).d3d_context( )->OMSetRenderTargets( enum_target_count, m_targets, m_zb );
	m_dirty_targets.reset( );
}

inline void backend::set_render_output( res_render_output const* render_output )
{
	m_render_output = render_output;
	m_base_rt = m_render_output ? m_render_output->m_base_rt : 0;
	m_base_zb = m_render_output ? m_render_output->m_base_zb : 0;
}

inline u32 backend::target_width( ) const
{
	return m_user_output ? m_user_output_width
		: (m_render_output ? m_render_output->width( ) : 0);
}

inline u32 backend::target_height( ) const
{
	return m_user_output ? m_user_output_height
		: (m_render_output ? m_render_output->height( ) : 0);
}

template < enum_shader_type shader_type >
inline u32 backend::get_constants_marker( )
{
	return m_constant_update_markers[shader_type];
}

template < typename T >
inline void backend::set_vs_constant( shader_constant_host const* c, T const& arg )
{
	COMPILE_ASSERT( constant_type_traits<T>::value == true, Only_types_described_in_constant_type_traits_are_supported );
	ASSERT( c != NULL );
	m_vs_constants_handler.set_constant( *c, arg );
	++num_setted_shader_constants;
}

template < typename T, u32 array_size >
inline void backend::set_vs_constant( shader_constant_host const* c, T const (&arg)[array_size] )
{
	set_vs_constant( c, &arg[0], array_size );
}

template < typename T >
inline void backend::set_vs_constant( shader_constant_host const* c, T const* arg, u32 array_size )
{
	COMPILE_ASSERT( constant_type_traits<T>::value == true, Only_types_described_in_constant_type_traits_are_supported_ );
	ASSERT( c != NULL );
	m_vs_constants_handler.set_constant_array( *c, arg, array_size );
	++num_setted_shader_constants;
}

template < typename T >
inline void backend::set_gs_constant( shader_constant_host const* c, T const& arg )
{
	COMPILE_ASSERT( constant_type_traits<T>::value == true, Only_types_described_in_constant_type_traits_are_supported );
	ASSERT( c != NULL );
	m_gs_constants_handler.set_constant( *c, arg );
	++num_setted_shader_constants;
}

template < typename T, u32 array_size >
inline void backend::set_gs_constant( shader_constant_host const* c, T const (&arg)[array_size] )
{
	set_gs_constant( c, &arg[0], array_size );
}

template < typename T >
inline void backend::set_gs_constant( shader_constant_host const* c, T const* arg, u32 array_size )
{
	COMPILE_ASSERT( constant_type_traits<T>::value == true, Only_types_described_in_constant_type_traits_are_supported_ );
	ASSERT( c != NULL );
	m_gs_constants_handler.set_constant_array( *c, arg, array_size );
	++num_setted_shader_constants;
}

template < typename T >
inline void backend::set_ps_constant( shader_constant_host const* c, T const& arg )
{
	COMPILE_ASSERT( constant_type_traits<T>::value == true, Only_types_described_in_constant_type_traits_are_supported );
	ASSERT( c != NULL );
	m_ps_constants_handler.set_constant( *c, arg );
	++num_setted_shader_constants;
}

template < typename T, u32 array_size >
inline void backend::set_ps_constant( shader_constant_host const* c, T const (&arg)[array_size] )
{
	set_ps_constant( c, &arg[0], array_size );
}

template < typename T >
inline void backend::set_ps_constant( shader_constant_host const* c, T const* arg, u32 array_size )
{
	COMPILE_ASSERT( constant_type_traits<T>::value == true, Only_types_described_in_constant_type_traits_are_supported_ );
	ASSERT( c != NULL );
	m_ps_constants_handler.set_constant_array( *c, arg, array_size );
	++num_setted_shader_constants;
}

inline void backend::clear_render_targets( enum_render_target_enum target, math::color color )
{
	if ( !m_targets[target] )
		return;

	float components[4];
	color.get_RGBA( components[0], components[1], components[2], components[3] );
	device::ref( ).d3d_context( )->ClearRenderTargetView( m_targets[target], components );
}

inline void backend::reset( )
{
	set_stencil_ref( 0 );
	set_sample_mask( 0xFFFFFFFF );

	set_declaration( 0 );
	set_rasterizer_state( 0 );

	set_depth_stencil_state( 0 );
	set_blend_state( 0 );
	set_vs( 0 );
	set_vs_constants( 0 );
	set_vs_samplers( 0 );
	set_vs_textures( 0 );

	set_gs( 0 );
	set_gs_constants( 0 );
	set_gs_samplers( 0 );
	set_gs_textures( 0 );

	set_ps( 0 );
	set_ps_constants( 0 );
	set_ps_samplers( 0 );
	set_ps_textures( 0 );

	set_vs_constants( 0 );
	set_vb( 0, 0 );
	set_ib( 0 );

	set_render_targets( 0, 0, 0, 0 );
	set_depth_stencil_target( 0 );

	flush_rt_shader_resources( );
	flush_rt_views( );

	++m_constant_update_counter;

	m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	device::ref( ).d3d_context( )->IASetPrimitiveTopology( m_primitive_topology );
}

inline void backend::get_viewport( D3D11_VIEWPORT& viewport )
{
	u32 count = 1;
	device::ref( ).d3d_context( )->RSGetViewports( &count, &viewport );
}

inline void backend::set_viewport( D3D11_VIEWPORT const& viewport )
{
	device::ref( ).d3d_context( )->RSSetViewports( 1, &viewport );
}

inline void backend::get_viewports( u32& count, D3D11_VIEWPORT* viewports )
{
	device::ref( ).d3d_context( )->RSGetViewports( &count, viewports );
}

inline void backend::set_viewports( u32 count, D3D11_VIEWPORT const* viewports )
{
	device::ref( ).d3d_context( )->RSSetViewports( count, viewports );
}

inline void backend::render_indexed_instanced(
	D3D_PRIMITIVE_TOPOLOGY topology,
	u32 index_count,
	u32 start_index,
	u32 base_vertex,
	u32 instance_count,
	u32 start_instance
)
{
	flush( );
	update_input_layout( );
	++num_draw_calls;
	device::ref( ).d3d_context( )->IASetPrimitiveTopology( topology );
	device::ref( ).d3d_context( )->DrawIndexedInstanced(
		index_count, instance_count, start_index, base_vertex, start_instance
	);
}

inline void backend::on_device_create( ID3D11Device* device )
{
	m_device = device;
}

inline bool backend::valid_output_window( ) const
{
	return m_render_output && m_render_output->valid_present( );
}

inline void backend::set_user_output( bool enabled, u32 width, u32 height )
{
	m_user_output = enabled;
	m_user_output_width = width;
	m_user_output_height = height;
}

inline void backend::flush_c_cache( )
{
	m_vs_constants_handler.apply( );
	m_gs_constants_handler.apply( );
	m_ps_constants_handler.apply( );
}

inline void backend::flush_stages( )
{
	m_vs_textures_handler.apply( );
	m_gs_textures_handler.apply( );
	m_ps_textures_handler.apply( );
	m_vs_samplers_handler.apply( );
	m_gs_samplers_handler.apply( );
	m_ps_samplers_handler.apply( );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_INLINE_H_INCLUDED
