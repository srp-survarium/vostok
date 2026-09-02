// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BACKEND_H_INCLUDED
#include <vostok/math_color.h>
#include <vostok/render/core/backend_handlers.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/index_buffer.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/render_target.h>
#include <vostok/render/core/res_render_output.h>
#include <vostok/render/core/shader_constant.h>
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
	template < enum_shader_type shader_type >
	friend class constants_handler;

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

	void set_vb( untyped_buffer* vb, u32 vb_stride, u32 vb_offset = 0 );
	void set_vb_instance_data( untyped_buffer* vb, u32 vb_stride, u32 vb_offset = 0 );
	void set_vb_stream_1( untyped_buffer* vb, u32 vb_stride, u32 vb_offset = 0 );
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
	void set_vs_constant( shader_constant_host const* constant, T const& arg );

	template < typename T, u32 array_size >
	void set_vs_constant( shader_constant_host const* constant, T const (&values)[array_size] );

	template < typename T >
	void set_vs_constant( shader_constant_host const* constant, T const* values, u32 array_size );

	template < typename T >
	void set_gs_constant( shader_constant_host const* constant, T const& arg );

	template < typename T, u32 array_size >
	void set_gs_constant( shader_constant_host const* constant, T const (&values)[array_size] );

	template < typename T >
	void set_gs_constant( shader_constant_host const* constant, T const* values, u32 array_size );

	template < typename T >
	void set_ps_constant( shader_constant_host const* constant, T const& arg );

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
		render_target const* rt0,
		render_target const* rt1,
		render_target const* rt2,
		render_target const* rt3
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
	u32 constant_update_counter( ) { return m_constant_update_counter; }

	template < enum_shader_type shader_type >
	u32 get_constants_marker( );
	bool valid_output_window( ) const;
	vertex_buffer vertex;
	index_buffer index;
	u32 num_total_rendered_triangles;
	u32 num_total_rendered_points;
	u32 num_setted_shader_constants;
	bool disabled_shader_constansts_set;
	u32 num_draw_calls;
	bool draw_calls_counting;
	void set_user_output( bool enabled, u32 const width, u32 const height );
	ID3D11RenderTargetView* get_base_rt( ) const { return m_base_rt; }

private:
	void flush_rt( );
	void update_input_layout( );
	void flush_c_cache( );
	void flush_stages( );

	bool m_user_output;
	u32 m_user_output_width;
	u32 m_user_output_height;
	ID3D11Device* m_device;
	ID3D11RasterizerState* m_rasterizer_state;
	ID3D11DepthStencilState* m_depth_stencils_state;
	ID3D11BlendState* m_effect_state;
	u32 m_stencil_ref;
	u32 m_sample_mask;

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
	render_dirty_objects m_dirty_objects;

	struct render_dirty_targets {
		render_dirty_targets( ) { reset( ); }
		void reset( ) { ZeroMemory( this, sizeof( *this ) ); }
		bool any( )
		{
			return render_targets[0] | render_targets[1] | render_targets[2]
				| render_targets[3] | depth_stencil;
		}

		bool render_targets[enum_target_count];
		bool depth_stencil;
	};
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
	typedef vector<shader_constant_host*> constant_hosts;
	constant_hosts m_constant_hosts;
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
	enum { texture_stage_count = 16 };
	ID3D11Texture2D* m_stages[texture_stage_count];
	u32 m_constant_update_counter;
	u32 m_constant_update_markers[enum_shader_types_count];
};

} // namespace render
} // namespace vostok

#include <vostok/render/core/backend_handlers_inline.h>
#include <vostok/render/core/backend_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_H_INCLUDED
