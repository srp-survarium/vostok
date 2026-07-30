#ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BACKEND_H_INCLUDED

#include <vostok/math_color.h>
#include <vostok/render/core/backend_handlers.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/index_buffer.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/res_render_output.h>
#include <vostok/render/core/shader_constant_defines.h>
#include <vostok/render/core/vertex_buffer.h>

namespace vostok {

class shared_string;

namespace render {

class render_target;
class res_declaration;
class res_input_layout;
class res_sampler_list;
class res_texture;
class res_texture_list;
class shader_constant;
class shader_constant_host;
class shader_constant_table;
class untyped_buffer;

template < typename shader_data >
class res_xs_hw;

typedef res_xs_hw<vs_data> res_vs_hw;
typedef res_xs_hw<gs_data> res_gs_hw;
typedef res_xs_hw<ps_data> res_ps_hw;

enum enum_render_target_enum {
	enum_target_rt0,
	enum_target_rt1,
	enum_target_rt2,
	enum_target_rt3,
	enum_target_count,
	enum_target_zb,
};

#define render_output_default reinterpret_cast< res_render_output* >( 0xFFFFFFFF )

class backend : public quasi_singleton<backend> {
public:
	backend( );
	~backend( );

	void reset( );
	void flush( );

	void flush_rt_shader_resources( );
	void flush_rt_views( );

	void set_rasterizer_state( ID3D11RasterizerState* state );
	void set_depth_stencil_state( ID3D11DepthStencilState* state );
	void set_stencil_ref( u32 stencil_ref );
	void set_blend_state( ID3D11BlendState* state );
	void set_sample_mask( u32 sample_mask );

	shader_constant_host* register_constant_host( shader_constant const& constant );
	shader_constant_host* register_constant_host( shared_string const& name, enum_constant_type type );
	shader_constant_host const* find_constant_host(
		shared_string const& name,
		enum_constant_type type,
		bool create_if_missing = true
	);

	void set_vb( untyped_buffer* buffer, u32 stride, u32 offset = 0 );
	void set_vb_instance_data( untyped_buffer* buffer, u32 stride, u32 offset = 0 );
	void set_vb_stream_1( untyped_buffer* buffer, u32 stride, u32 offset = 0 );
	void set_ib( untyped_buffer* buffer, u32 offset = 0 );

	void set_vs( res_vs_hw* shader );
	void set_ps( res_ps_hw* shader );
	void set_gs( res_gs_hw* shader );

	void set_declaration( res_declaration* declaration );
	void set_input_layout( res_input_layout* layout );

	void set_vs_constants( shader_constant_table* table );
	void set_gs_constants( shader_constant_table* table );
	void set_ps_constants( shader_constant_table* table );

	void set_vs_textures( res_texture_list* textures );
	void set_gs_textures( res_texture_list* textures );
	void set_ps_textures( res_texture_list* textures );

	void set_vs_texture( pcstr name, res_texture* texture );
	void set_gs_texture( pcstr name, res_texture* texture );
	void set_ps_texture( pcstr name, res_texture* texture );

	void set_vs_samplers( res_sampler_list* samplers );
	void set_gs_samplers( res_sampler_list* samplers );
	void set_ps_samplers( res_sampler_list* samplers );

	template < typename T >
	void set_vs_constant( shader_constant_host const* constant, T const& value );

	template < typename T, u32 array_size >
	void set_vs_constant( shader_constant_host const* constant, T const (&values)[array_size] );

	template < typename T >
	void set_vs_constant( shader_constant_host const* constant, T const* values, u32 array_size );

	template < typename T >
	void set_gs_constant( shader_constant_host const* constant, T const& value );

	template < typename T, u32 array_size >
	void set_gs_constant( shader_constant_host const* constant, T const (&values)[array_size] );

	template < typename T >
	void set_gs_constant( shader_constant_host const* constant, T const* values, u32 array_size );

	template < typename T >
	void set_ps_constant( shader_constant_host const* constant, T const& value );

	template < typename T, u32 array_size >
	void set_ps_constant( shader_constant_host const* constant, T const (&values)[array_size] );

	template < typename T >
	void set_ps_constant( shader_constant_host const* constant, T const* values, u32 array_size );

	void get_viewport( D3D11_VIEWPORT& viewport );
	void set_viewport( D3D11_VIEWPORT const& viewport );
	void get_viewports( u32& count, D3D11_VIEWPORT* viewports );
	void set_viewports( u32 count, D3D11_VIEWPORT const* viewports );

	u32 target_width( ) const;
	u32 target_height( ) const;

	void set_render_output( res_render_output const* render_output );
	void set_render_target( enum_render_target_enum target, render_target const* render_target );
	void set_render_targets(
		render_target const* render_target_0,
		render_target const* render_target_1,
		render_target const* render_target_2,
		render_target const* render_target_3
	);
	void set_depth_stencil_target( render_target const* render_target );
	void reset_render_targets( bool only_the_base = false );
	void reset_depth_stencil_target( );

	void clear_render_targets( math::color color );
	void clear_render_targets( float red, float green, float blue, float alpha );
	void clear_render_targets( enum_render_target_enum target, math::color color );
	void clear_render_targets(
		math::color color_0,
		math::color color_1,
		math::color color_2,
		math::color color_3
	);
	void clear_depth_stencil( u32 flags, float depth, u8 stencil );

	void render_indexed(
		D3D_PRIMITIVE_TOPOLOGY topology,
		u32 index_count,
		u32 start_index,
		u32 base_vertex
	);
	void render_indexed_instanced(
		D3D_PRIMITIVE_TOPOLOGY topology,
		u32 index_count,
		u32 start_index,
		u32 base_vertex,
		u32 instance_count,
		u32 start_instance
	);
	void render( D3D_PRIMITIVE_TOPOLOGY topology, u32 vertex_count, u32 base_vertex );

	void on_device_create( ID3D11Device* device );
	void on_device_destroy( );
	void reset_constant_update_markers( );
	u32 constant_update_counter( ) const { return m_constant_update_counter; }
	bool valid_output_window( ) const;
	void set_user_output( bool enabled, u32 width, u32 height );
	ID3D11RenderTargetView* get_base_rt( ) const { return m_base_rt; }

public:
	u32 num_vs_changes;
	u32 num_ps_changes;
	u32 num_il_changes;
	u32 num_vsc_changes;
	u32 num_vst_changes;
	u32 num_vss_changes;
	u32 num_psc_changes;
	u32 num_pst_changes;
	u32 num_pss_changes;
	bool max_triagles_per_dip;
	bool disable_DrawIndexed;
	bool m_set_ps_sources;
	bool allow_debug_profile_dip;
	vertex_buffer vertex;
	index_buffer index;
	u32 num_total_rendered_triangles;
	u32 num_total_rendered_points;
	u32 num_setted_shader_constants;
	bool disabled_shader_constansts_set;
	u32 num_draw_calls;
	bool draw_calls_counting;

private:
	void flush_rt( );
	void update_input_layout( );
	void flush_c_cache( );
	void flush_stages( );

	struct render_dirty_objects {
		render_dirty_objects( ) { reset( ); }
		void reset( ) { ZeroMemory( this, sizeof( *this ) ); }

		bool vertex_buffer;
		bool vertex_buffer_instance_data;
		bool vertex_buffer_stream_1;
		bool index_buffer;
		bool input_declaration;
		bool input_layout;
		bool rasterizer_state;
		bool depth_stencil_state;
		bool effect_state;
		bool vertex_shader;
		bool vertex_constants;
		bool vertex_textures;
		bool vertex_samplers;
		bool geometry_shader;
		bool geometry_constants;
		bool geometry_textures;
		bool geometry_samplers;
		bool pixel_shader;
		bool pixel_constants;
		bool pixel_textures;
		bool pixel_samplers;
		bool bstat;
		bool primitive_topology;
	};

	struct render_dirty_targets {
		render_dirty_targets( ) { reset( ); }
		void reset( ) { ZeroMemory( this, sizeof( *this ) ); }
		bool any( ) const
		{
			return depth_stencil || render_targets[0] || render_targets[1]
				|| render_targets[2] || render_targets[3];
		}

		bool render_targets[enum_target_count];
		bool depth_stencil;
	};

	bool m_user_output;
	u32 m_user_output_width;
	u32 m_user_output_height;
	ID3D11Device* m_device;
	ID3D11RasterizerState* m_rasterizer_state;
	ID3D11DepthStencilState* m_depth_stencils_state;
	ID3D11BlendState* m_effect_state;
	u32 m_stencil_ref;
	u32 m_sample_mask;
	render_dirty_objects m_dirty_objects;
	render_dirty_targets m_dirty_targets;
	untyped_buffer* m_vb;
	untyped_buffer* m_vb_instance_data;
	untyped_buffer* m_vb_stream_1;
	untyped_buffer* m_ib;
	res_vs_hw* m_vs;
	res_ps_hw* m_ps;
	res_gs_hw* m_gs;
	constants_handler<enum_shader_type_vertex> m_vs_constants_handler;
	textures_handler<enum_shader_type_vertex> m_vs_textures_handler;
	samplers_handler<enum_shader_type_vertex> m_vs_samplers_handler;
	constants_handler<enum_shader_type_geometry> m_gs_constants_handler;
	textures_handler<enum_shader_type_geometry> m_gs_textures_handler;
	samplers_handler<enum_shader_type_geometry> m_gs_samplers_handler;
	constants_handler<enum_shader_type_pixel> m_ps_constants_handler;
	textures_handler<enum_shader_type_pixel> m_ps_textures_handler;
	samplers_handler<enum_shader_type_pixel> m_ps_samplers_handler;
	D3D_PRIMITIVE_TOPOLOGY m_primitive_topology;
	vector<shader_constant_host*> m_constant_hosts;
	res_declaration* m_decl;
	res_input_layout* m_input_layout;
	ID3D11RenderTargetView* m_targets[enum_target_count];
	ID3D11DepthStencilView* m_zb;

public:
	res_render_output_const_ptr m_render_output;
	float m_texture_compression_time;
	float m_dxt_rt_tex_creation_time;
	float m_cpu_compression_time;
	u32 m_cpu_num_compressed_textures;
	u32 m_gpu_num_compressed_textures;

private:
	ID3D11RenderTargetView* m_base_rt;
	ID3D11DepthStencilView* m_base_zb;
	u32 m_vb_stride;
	u32 m_vb_offset;
	u32 m_vb_stride_instance_data;
	u32 m_vb_offset_instance_data;
	u32 m_vb_stride_stream_1;
	u32 m_vb_offset_stream_1;
	u32 m_ib_offset;
	ID3D11Texture2D* m_stages[16];
	u32 m_constant_update_counter;
	u32 m_constant_update_markers[enum_shader_types_count];
};

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

inline shader_constant_host* backend::register_constant_host( shader_constant const& )
{
	// STATE[STUB]
	return 0;
}

inline shader_constant_host const* backend::find_constant_host(
	shared_string const&,
	enum_constant_type,
	bool
)
{
	// STATE[STUB]
	return 0;
}

inline void backend::set_vb( untyped_buffer* buffer, u32 stride, u32 offset )
{
	// FUNCTION BODY[0x645b0]
	m_dirty_objects.vertex_buffer |= m_vb != buffer || m_vb_stride != stride || m_vb_offset != offset;
	m_vb = buffer;
	m_vb_stride = stride;
	m_vb_offset = offset;
}

inline void backend::set_vb_instance_data( untyped_buffer* buffer, u32 stride, u32 offset )
{
	m_dirty_objects.vertex_buffer_instance_data |= m_vb_instance_data != buffer
		|| m_vb_stride_instance_data != stride || m_vb_offset_instance_data != offset;
	m_vb_instance_data = buffer;
	m_vb_stride_instance_data = stride;
	m_vb_offset_instance_data = offset;
}

inline void backend::set_vb_stream_1( untyped_buffer* buffer, u32 stride, u32 offset )
{
	// FUNCTION BODY[0x8b7c0]
	m_dirty_objects.vertex_buffer_stream_1 |= m_vb_stream_1 != buffer
		|| m_vb_stride_stream_1 != stride || m_vb_offset_stream_1 != offset;
	m_vb_stream_1 = buffer;
	m_vb_stride_stream_1 = stride;
	m_vb_offset_stream_1 = offset;
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
	m_vs = shader;
}

inline void backend::set_ps( res_ps_hw* shader )
{
	m_dirty_objects.pixel_shader |= m_ps != shader;
	m_ps = shader;
}

inline void backend::set_gs( res_gs_hw* shader )
{
	m_dirty_objects.geometry_shader |= m_gs != shader;
	m_gs = shader;
}

inline void backend::set_declaration( res_declaration* declaration )
{
	if ( m_decl == declaration )
		return;
	m_decl = declaration;
	m_dirty_objects.input_declaration = true;
	m_input_layout = 0;
	m_dirty_objects.input_layout = true;
}

inline void backend::set_input_layout( res_input_layout* layout )
{
	m_dirty_objects.input_layout |= m_input_layout != layout;
	m_input_layout = layout;
}

inline void backend::set_vs_constants( shader_constant_table* table )
{
	m_vs_constants_handler.assign( table );
	m_dirty_objects.vertex_constants = true;
}

inline void backend::set_gs_constants( shader_constant_table* table )
{
	m_gs_constants_handler.assign( table );
	m_dirty_objects.geometry_constants = true;
}

inline void backend::set_ps_constants( shader_constant_table* table )
{
	m_ps_constants_handler.assign( table );
	m_dirty_objects.pixel_constants = true;
}

inline void backend::set_vs_textures( res_texture_list* textures )
{
	m_vs_textures_handler.assign( textures );
	m_dirty_objects.vertex_textures = true;
}

inline void backend::set_gs_textures( res_texture_list* textures )
{
	m_gs_textures_handler.assign( textures );
	m_dirty_objects.geometry_textures = true;
}

inline void backend::set_ps_textures( res_texture_list* textures )
{
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
	// FUNCTION BODY[0x723f0]
	m_dirty_objects.pixel_textures |= m_ps_textures_handler.set_overwrite( name, texture );
}

inline void backend::set_vs_samplers( res_sampler_list* samplers )
{
	m_vs_samplers_handler.assign( samplers );
	m_dirty_objects.vertex_samplers = true;
}

inline void backend::set_gs_samplers( res_sampler_list* samplers )
{
	m_gs_samplers_handler.assign( samplers );
	m_dirty_objects.geometry_samplers = true;
}

inline void backend::set_ps_samplers( res_sampler_list* samplers )
{
	m_ps_samplers_handler.assign( samplers );
	m_dirty_objects.pixel_samplers = true;
}

template < typename T >
inline void backend::set_vs_constant( shader_constant_host const* constant, T const& value )
{
	// FUNCTION BODY[0x67580] for math::float4
	if ( constant )
		m_vs_constants_handler.set_constant( *constant, value );
}

template < typename T, u32 array_size >
inline void backend::set_vs_constant( shader_constant_host const* constant, T const (&values)[array_size] )
{
	set_vs_constant( constant, values, array_size );
}

template < typename T >
inline void backend::set_vs_constant( shader_constant_host const* constant, T const* values, u32 array_size )
{
	if ( constant )
		m_vs_constants_handler.set_constant_array( *constant, values, array_size );
}

template < typename T >
inline void backend::set_gs_constant( shader_constant_host const* constant, T const& value )
{
	if ( constant )
		m_gs_constants_handler.set_constant( *constant, value );
}

template < typename T, u32 array_size >
inline void backend::set_gs_constant( shader_constant_host const* constant, T const (&values)[array_size] )
{
	set_gs_constant( constant, values, array_size );
}

template < typename T >
inline void backend::set_gs_constant( shader_constant_host const* constant, T const* values, u32 array_size )
{
	if ( constant )
		m_gs_constants_handler.set_constant_array( *constant, values, array_size );
}

template < typename T >
inline void backend::set_ps_constant( shader_constant_host const* constant, T const& value )
{
	// FUNCTION BODY[0x672c0] for float, u32, math::float3, and math::float4x4
	if ( constant )
		m_ps_constants_handler.set_constant( *constant, value );
}

template < typename T, u32 array_size >
inline void backend::set_ps_constant( shader_constant_host const* constant, T const (&values)[array_size] )
{
	set_ps_constant( constant, values, array_size );
}

template < typename T >
inline void backend::set_ps_constant( shader_constant_host const* constant, T const* values, u32 array_size )
{
	if ( constant )
		m_ps_constants_handler.set_constant_array( *constant, values, array_size );
}

inline void backend::get_viewport( D3D11_VIEWPORT& viewport )
{
	// FUNCTION BODY[0x668b0]
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

inline void backend::set_render_output( res_render_output const* render_output )
{
	// FUNCTION BODY[0x64700]
	m_render_output = render_output;
	m_base_rt = render_output ? render_output->m_base_rt : 0;
	m_base_zb = render_output ? render_output->m_base_zb : 0;
}

inline void backend::set_render_targets(
	render_target const* render_target_0,
	render_target const* render_target_1,
	render_target const* render_target_2,
	render_target const* render_target_3
)
{
	set_render_target( enum_target_rt0, render_target_0 );
	set_render_target( enum_target_rt1, render_target_1 );
	set_render_target( enum_target_rt2, render_target_2 );
	set_render_target( enum_target_rt3, render_target_3 );
}

inline void backend::reset_render_targets( bool only_the_base )
{
	// FUNCTION BODY[0x68900]
	m_dirty_targets.render_targets[0] |= m_targets[0] != m_base_rt;
	m_targets[0] = m_base_rt;
	for ( u32 index = 1; index < enum_target_count && !only_the_base; ++index ) {
		m_dirty_targets.render_targets[index] |= m_targets[index] != 0;
		m_targets[index] = 0;
	}
}

inline void backend::reset_depth_stencil_target( )
{
	// FUNCTION BODY[0x645f0]
	m_dirty_targets.depth_stencil |= m_zb != m_base_zb;
	m_zb = m_base_zb;
}

inline void backend::set_depth_stencil_target( render_target const* )
{
	// STATE[STUB]
}

inline void backend::clear_render_targets( enum_render_target_enum target, math::color color )
{
	if ( !m_targets[target] )
		return;

	float components[4];
	color.get_RGBA( components[0], components[1], components[2], components[3] );
	device::ref( ).d3d_context( )->ClearRenderTargetView( m_targets[target], components );
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

inline void backend::reset_constant_update_markers( )
{
	ZeroMemory( m_constant_update_markers, sizeof( m_constant_update_markers ) );
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

inline void backend::flush_rt_shader_resources( )
{
	// FUNCTION BODY[0x64610]
	ID3D11ShaderResourceView* resources[12] = { 0 };
	device::ref( ).d3d_context( )->PSSetShaderResources( 0, 12, resources );
	device::ref( ).d3d_context( )->VSSetShaderResources( 0, 12, resources );
	device::ref( ).d3d_context( )->GSSetShaderResources( 0, 12, resources );
}

inline void backend::flush_rt_views( )
{
	// FUNCTION BODY[0x646c0]
	ID3D11RenderTargetView* targets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { 0 };
	device::ref( ).d3d_context( )->OMSetRenderTargets(
		D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, targets, 0
	);
}

inline void backend::flush_rt( )
{
	// FUNCTION BODY[0x1291b0]
	if ( m_dirty_targets.any( ) )
		device::ref( ).d3d_context( )->OMSetRenderTargets( enum_target_count, m_targets, m_zb );
	m_dirty_targets.reset( );
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

inline void backend::reset( )
{
	// FUNCTION BODY[0x64760]
	set_stencil_ref( 0 );
	set_sample_mask( 0xFFFFFFFF );
	set_declaration( 0 );
	set_input_layout( 0 );
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
	set_vb( 0, 0 );
	set_vb_instance_data( 0, 0 );
	set_vb_stream_1( 0, 0 );
	set_ib( 0 );
	set_render_targets( 0, 0, 0, 0 );
	set_depth_stencil_target( 0 );
	++m_constant_update_counter;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_H_INCLUDED
