////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BACKEND_H_INCLUDED
#define RENDER_BACKEND_H_INCLUDED

/* INCLUDES */
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
class vostok::quasi_singleton<vostok::render::backend>;
class vostok::render::constants_handler<0>;
class vostok::render::constants_handler<1>;
class vostok::render::constants_handler<2>;
class vostok::render::index_buffer;
class vostok::render::res_declaration;
class vostok::render::res_input_layout;
class vostok::render::res_xs_hw<vostok::render::gs_data>;
class vostok::render::res_xs_hw<vostok::render::ps_data>;
class vostok::render::res_xs_hw<vostok::render::vs_data>;
class vostok::render::samplers_handler<0>;
class vostok::render::samplers_handler<1>;
class vostok::render::samplers_handler<2>;
class vostok::render::textures_handler<0>;
class vostok::render::textures_handler<1>;
class vostok::render::textures_handler<2>;
class vostok::render::untyped_buffer;
class vostok::render::vector<vostok::render::shader_constant_host *>;
class vostok::render::vertex_buffer;
struct vostok::render::backend::render_dirty_objects;
struct vostok::render::backend::render_dirty_targets;
enum vostok::render::enum_render_target_enum;
class vostok::math::color;
class vostok::render::enum_constant_type;
typedef vostok::intrusive_ptr<vostok::render::res_render_output const ,vostok::render::resource_intrusive_base const ,vostok::threading::single_threading_policy>
	vostok::render::res_render_output const _ptr;
class vostok::render::res_render_output const ;

/* FORWARD REFS */
class vostok::render::render_target;
class vostok::render::res_render_output;
class vostok::render::res_sampler_list;
class vostok::render::res_texture;
class vostok::render::res_texture_list;
class vostok::render::shader_constant;
class vostok::render::shader_constant_host;
class vostok::render::shader_constant_table;
class vostok::shared_string;

namespace vostok {
namespace render {

enum D3D_PRIMITIVE_TOPOLOGY
{
	D3D_PRIMITIVE_TOPOLOGY_UNDEFINED		= 0x00,
	D3D_PRIMITIVE_TOPOLOGY_POINTLIST		= 0x01,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST			= 0x02,
	D3D_PRIMITIVE_TOPOLOGY_LINESTRIP		= 0x03,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST		= 0x04,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	= 0x05,
	D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ		= 0x0a,
	D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ	= 0x0b,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	= 0x0c,
	D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ	= 0x0d,
	D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST	= 0x21,
	D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST	= 0x22,
	D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST	= 0x23,
	D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST	= 0x24,
	D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST	= 0x25,
	D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST	= 0x26,
	D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST	= 0x27,
	D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST	= 0x28,
	D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST	= 0x29,
	D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST	= 0x2a,
	D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST	= 0x2b,
	D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST	= 0x2c,
	D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST	= 0x2d,
	D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST	= 0x2e,
	D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST	= 0x2f,
	D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST	= 0x30,
	D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST	= 0x31,
	D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST	= 0x32,
	D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST	= 0x33,
	D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST	= 0x34,
	D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST	= 0x35,
	D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST	= 0x36,
	D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST	= 0x37,
	D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST	= 0x38,
	D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST	= 0x39,
	D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST	= 0x3a,
	D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST	= 0x3b,
	D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST	= 0x3c,
	D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST	= 0x3d,
	D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST	= 0x3e,
	D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST	= 0x3f,
	D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST	= 0x40,
	D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED		= 0x00,
	D3D10_PRIMITIVE_TOPOLOGY_POINTLIST		= 0x01,
	D3D10_PRIMITIVE_TOPOLOGY_LINELIST		= 0x02,
	D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP		= 0x03,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST	= 0x04,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	= 0x05,
	D3D10_PRIMITIVE_TOPOLOGY_LINELIST_ADJ	= 0x0a,
	D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ	= 0x0b,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	= 0x0c,
	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ	= 0x0d,
	D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED		= 0x00,
	D3D11_PRIMITIVE_TOPOLOGY_POINTLIST		= 0x01,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST		= 0x02,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP		= 0x03,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST	= 0x04,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP	= 0x05,
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ	= 0x0a,
	D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ	= 0x0b,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	= 0x0c,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ	= 0x0d,
	D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST	= 0x21,
	D3D11_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST	= 0x22,
	D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST	= 0x23,
	D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST	= 0x24,
	D3D11_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST	= 0x25,
	D3D11_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST	= 0x26,
	D3D11_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST	= 0x27,
	D3D11_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST	= 0x28,
	D3D11_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST	= 0x29,
	D3D11_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST	= 0x2a,
	D3D11_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST	= 0x2b,
	D3D11_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST	= 0x2c,
	D3D11_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST	= 0x2d,
	D3D11_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST	= 0x2e,
	D3D11_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST	= 0x2f,
	D3D11_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST	= 0x30,
	D3D11_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST	= 0x31,
	D3D11_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST	= 0x32,
	D3D11_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST	= 0x33,
	D3D11_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST	= 0x34,
	D3D11_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST	= 0x35,
	D3D11_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST	= 0x36,
	D3D11_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST	= 0x37,
	D3D11_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST	= 0x38,
	D3D11_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST	= 0x39,
	D3D11_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST	= 0x3a,
	D3D11_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST	= 0x3b,
	D3D11_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST	= 0x3c,
	D3D11_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST	= 0x3d,
	D3D11_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST	= 0x3e,
	D3D11_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST	= 0x3f,
	D3D11_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST	= 0x40,
};

class backend : public quasi_singleton< backend > {
public:
											backend							( );
											~backend						( );

	inline	void							reset							( ) { /* no source */ }

			void							flush							( );

	inline	void							flush_rt_shader_resources		( ) { /* no source */ }
	inline	void							flush_rt_views					( ) { /* no source */ }

	inline	void							set_rasterizer_state			( ID3D11RasterizerState* arg_0 ) { /* no source */ }
	inline	void							set_depth_stencil_state			( ID3D11DepthStencilState* arg_0 ) { /* no source */ }
	inline	void							set_stencil_ref					( u32 arg_0 ) { /* no source */ }
	inline	void							set_blend_state					( ID3D11BlendState* arg_0 ) { /* no source */ }
	inline	void							set_sample_mask					( u32 arg_0 ) { /* no source */ }

	inline	shader_constant_host*			register_constant_host			( shader_constant const& arg_0 ) { /* no source */ }
			shader_constant_host*			register_constant_host			( shared_string const& name, enum_constant_type type );
	inline	shader_constant_host const*		find_constant_host				( shared_string const& arg_0, enum_constant_type arg_1, bool arg_2 ) { /* no source */ }

	inline	void							set_vb							( untyped_buffer* vb, u32 vb_stride, u32 offset ) { /* no source */ }
	inline	void							set_vb_instance_data			( untyped_buffer* arg_0, u32 arg_1, u32 arg_2 ) { /* no source */ }
	inline	void							set_vb_stream_1					( untyped_buffer* vb, u32 vb_stride, u32 offset ) { /* no source */ }
	inline	void							set_ib							( untyped_buffer* arg_0, u32 arg_1 ) { /* no source */ }
	inline	void							set_vs							( res_xs_hw< vs_data >* arg_0 ) { /* no source */ }
	inline	void							set_ps							( res_xs_hw< ps_data >* arg_0 ) { /* no source */ }
	inline	void							set_gs							( res_xs_hw< gs_data >* arg_0 ) { /* no source */ }
	inline	void							set_declaration					( res_declaration* arg_0 ) { /* no source */ }
	inline	void							set_input_layout				( res_input_layout* arg_0 ) { /* no source */ }
	inline	void							set_vs_constants				( shader_constant_table* arg_0 ) { /* no source */ }
	inline	void							set_gs_constants				( shader_constant_table* arg_0 ) { /* no source */ }
	inline	void							set_ps_constants				( shader_constant_table* arg_0 ) { /* no source */ }
	inline	void							set_vs_textures					( res_texture_list* arg_0 ) { /* no source */ }
	inline	void							set_gs_textures					( res_texture_list* arg_0 ) { /* no source */ }
	inline	void							set_ps_textures					( res_texture_list* arg_0 ) { /* no source */ }
	inline	void							set_vs_texture					( pcstr arg_0, res_texture* arg_1 ) { /* no source */ }
	inline	void							set_gs_texture					( pcstr arg_0, res_texture* arg_1 ) { /* no source */ }
	inline	void							set_ps_texture					( pcstr name, res_texture* texture ) { /* no source */ }
	inline	void							set_vs_samplers					( res_sampler_list* arg_0 ) { /* no source */ }
	inline	void							set_gs_samplers					( res_sampler_list* arg_0 ) { /* no source */ }
	inline	void							set_ps_samplers					( res_sampler_list* arg_0 ) { /* no source */ }

	inline	void							get_viewport					( D3D11_VIEWPORT& viewport ) { /* no source */ }
	inline	void							set_viewport					( D3D11_VIEWPORT const& arg_0 ) { /* no source */ }

	inline	void							get_viewports					( u32& arg_0, D3D11_VIEWPORT* arg_1 ) { /* no source */ }
	inline	void							set_viewports					( u32 arg_0, D3D11_VIEWPORT const* arg_1 ) { /* no source */ }

	inline	u32								target_width					( ) const { /* no source */ }
	inline	u32								target_height					( ) const { /* no source */ }

	inline	void							set_render_output				( res_render_output const* render_output ) { /* no source */ }
			void							set_render_target				( enum_render_target_enum target, render_target const* rt );
	inline	void							set_render_targets				(
												render_target const*	arg_0,
												render_target const*	arg_1,
												render_target const*	arg_2,
												render_target const*	arg_3
											) { /* no source */ }
	inline	void							set_depth_stencil_target		( render_target const* arg_0 ) { /* no source */ }

	inline	void							reset_render_targets			( bool only_the_base ) { /* no source */ }
	inline	void							reset_depth_stencil_target		( ) { /* no source */ }

			void							clear_render_targets			( math::color color );
			void							clear_render_targets			(
												float		r,
												float		g,
												float		b,
												float		a
											);
	inline	void							clear_render_targets			( enum_render_target_enum arg_0, math::color arg_1 ) { /* no source */ }
			void							clear_render_targets			(
												math::color		color0,
												math::color		color1,
												math::color		color2,
												math::color		color3
											);
			void							clear_depth_stencil				( u32 flags, float z_value, u8 stencil_value );

			void							render_indexed					(
												D3D_PRIMITIVE_TOPOLOGY		type,
												u32							index_count,
												u32							start_index,
												u32							base_vertex
											);
	inline	void							render_indexed_instanced		(
												D3D_PRIMITIVE_TOPOLOGY		arg_0,
												u32							arg_1,
												u32							arg_2,
												u32							arg_3,
												u32							arg_4,
												u32							arg_5
											) { /* no source */ }

			void							render							( D3D_PRIMITIVE_TOPOLOGY type, u32 vertex_count, u32 base_vertex );

	inline	void							on_device_create				( ID3D11Device* arg_0 ) { /* no source */ }
			void							on_device_destroy				( );

	inline	void							reset_constant_update_markers	( ) { /* no source */ }

	inline	u32								constant_update_counter			( ) { /* no source */ }

	inline	bool							valid_output_window				( ) const { /* no source */ }

	inline	void							set_user_output					( bool arg_0, const u32 arg_1, const u32 arg_2 ) { /* no source */ }

	inline	ID3D11RenderTargetView*			get_base_rt						( ) const { /* no source */ }
private:
	inline	void							flush_rt						( ) { /* no source */ }

			void							update_input_layout				( );

	inline	void							flush_c_cache					( ) { /* no source */ }
	inline	void							flush_stages					( ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< backend > */
public:
	/* 0x0000 */	u32									num_vs_changes;
	/* 0x0004 */	u32									num_ps_changes;
	/* 0x0008 */	u32									num_il_changes;
	/* 0x000c */	u32									num_vsc_changes;
	/* 0x0010 */	u32									num_vst_changes;
	/* 0x0014 */	u32									num_vss_changes;
	/* 0x0018 */	u32									num_psc_changes;
	/* 0x001c */	u32									num_pst_changes;
	/* 0x0020 */	u32									num_pss_changes;
	/* 0x0024 */	bool								max_triagles_per_dip;
	/* 0x0025 */	bool								disable_DrawIndexed;
	/* 0x0026 */	bool								m_set_ps_sources;
	/* 0x0027 */	bool								allow_debug_profile_dip;
	/* 0x0028 */	vertex_buffer						vertex;
	/* 0x0040 */	index_buffer						index;
	/* 0x0054 */	u32									num_total_rendered_triangles;
	/* 0x0058 */	u32									num_total_rendered_points;
	/* 0x005c */	u32									num_setted_shader_constants;
	/* 0x0060 */	bool								disabled_shader_constansts_set;
	/* 0x0064 */	u32									num_draw_calls;
	/* 0x0068 */	bool								draw_calls_counting;
private:
	/* 0x0069 */	bool								m_user_output;
	/* 0x006c */	u32									m_user_output_width;
	/* 0x0070 */	u32									m_user_output_height;
	/* 0x0074 */	ID3D11Device*						m_device;
	/* 0x0078 */	ID3D11RasterizerState*				m_rasterizer_state;
	/* 0x007c */	ID3D11DepthStencilState*			m_depth_stencils_state;
	/* 0x0080 */	ID3D11BlendState*					m_effect_state;
	/* 0x0084 */	u32									m_stencil_ref;
	/* 0x0088 */	u32									m_sample_mask;
	/* 0x008c */	backend::render_dirty_objects		m_dirty_objects;
	/* 0x00a3 */	backend::render_dirty_targets		m_dirty_targets;
	/* 0x00a8 */	untyped_buffer*						m_vb;
	/* 0x00ac */	untyped_buffer*						m_vb_instance_data;
	/* 0x00b0 */	untyped_buffer*						m_vb_stream_1;
	/* 0x00b4 */	untyped_buffer*						m_ib;
	/* 0x00b8 */	res_xs_hw< vs_data >*				m_vs;
	/* 0x00bc */	res_xs_hw< ps_data >*				m_ps;
	/* 0x00c0 */	res_xs_hw< gs_data >*				m_gs;
	/* 0x00c4 */	constants_handler< 0 >				m_vs_constants_handler;
	/* 0x00d0 */	textures_handler< 0 >				m_vs_textures_handler;
	/* 0x02f4 */	samplers_handler< 0 >				m_vs_samplers_handler;
	/* 0x0344 */	constants_handler< 2 >				m_gs_constants_handler;
	/* 0x0350 */	textures_handler< 2 >				m_gs_textures_handler;
	/* 0x0574 */	samplers_handler< 2 >				m_gs_samplers_handler;
	/* 0x05c4 */	constants_handler< 1 >				m_ps_constants_handler;
	/* 0x05d0 */	textures_handler< 1 >				m_ps_textures_handler;
	/* 0x07f4 */	samplers_handler< 1 >				m_ps_samplers_handler;
	/* 0x0844 */	D3D_PRIMITIVE_TOPOLOGY				m_primitive_topology;
	/* 0x0848 */	vector< shader_constant_host* >		m_constant_hosts;
	/* 0x0854 */	res_declaration*					m_decl;
	/* 0x0858 */	res_input_layout*					m_input_layout;
	/* 0x085c */	ID3D11RenderTargetView*				m_targets[4];
	/* 0x086c */	ID3D11DepthStencilView*				m_zb;
public:
	/* 0x0870 */	res_render_output const _ptr		m_render_output;
	/* 0x0874 */	float								m_texture_compression_time;
	/* 0x0878 */	float								m_dxt_rt_tex_creation_time;
	/* 0x087c */	float								m_cpu_compression_time;
	/* 0x0880 */	u32									m_cpu_num_compressed_textures;
	/* 0x0884 */	u32									m_gpu_num_compressed_textures;
private:
	/* 0x0888 */	ID3D11RenderTargetView*				m_base_rt;
	/* 0x088c */	ID3D11DepthStencilView*				m_base_zb;
	/* 0x0890 */	u32									m_vb_stride;
	/* 0x0894 */	u32									m_vb_offset;
	/* 0x0898 */	u32									m_vb_stride_instance_data;
	/* 0x089c */	u32									m_vb_offset_instance_data;
	/* 0x08a0 */	u32									m_vb_stride_stream_1;
	/* 0x08a4 */	u32									m_vb_offset_stream_1;
	/* 0x08a8 */	u32									m_ib_offset;
	/* 0x08ac */	ID3D11Texture2D*					m_stages[16];
	/* 0x08ec */	u32									m_constant_update_counter;
	/* 0x08f0 */	u32									m_constant_update_markers[3];
}; // class backend

STATIC_SIZE_ASSERT(backend, 0x8FC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BACKEND_H_INCLUDED
