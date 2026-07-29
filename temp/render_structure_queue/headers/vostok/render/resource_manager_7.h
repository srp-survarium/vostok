////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_H_INCLUDED

/* INCLUDES */
class fastdelegate::FastDelegate<void __cdecl(enum vostok::render::enum_shader_type,char const *,char const *)>;
struct ID3D11DeviceContext;
class vostok::quasi_singleton<vostok::render::resource_manager>;
class vostok::render::map<vostok::fs_new::virtual_path_string,vostok::render::render_target *,vostok::render::resource_manager::str_pred>;
class vostok::render::map<vostok::fs_new::virtual_path_string,vostok::render::res_texture *,vostok::render::resource_manager::str_pred>;
class vostok::render::map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::gs_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >;
class vostok::render::map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::ps_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >;
class vostok::render::map<vostok::render::resource_manager::shader_name_config_pair,vostok::render::res_xs_hw<vostok::render::vs_data> *,stlp_std::less<vostok::render::resource_manager::shader_name_config_pair> >;
class vostok::render::set<vostok::render::res_declaration *,vostok::render::resource_manager::compare_predicate<vostok::render::res_declaration> >;
class vostok::render::set<vostok::render::res_geometry *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_geometry> >;
class vostok::render::set<vostok::render::res_input_layout *,vostok::render::resource_manager::compare_predicate<vostok::render::res_input_layout> >;
class vostok::render::set<vostok::render::res_sampler_list *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_sampler_list> >;
class vostok::render::set<vostok::render::res_signature *,vostok::render::resource_manager::compare_predicate<vostok::render::res_signature> >;
class vostok::render::set<vostok::render::res_texture_list *,vostok::render::resource_manager::compare_member_predicate<vostok::render::res_texture_list> >;
class vostok::render::set<vostok::render::res_xs<vostok::render::gs_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::gs_data> >;
class vostok::render::set<vostok::render::res_xs<vostok::render::ps_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::ps_data> >;
class vostok::render::set<vostok::render::res_xs<vostok::render::vs_data> *,vostok::render::resource_manager::compare_shader_predicate<vostok::render::vs_data> >;
class vostok::render::set<vostok::render::shader_constant_buffer *,vostok::render::resource_manager::constant_buffer_predicate>;
class vostok::render::set<vostok::render::shader_constant_table *,vostok::render::resource_manager::constant_table_predicate>;
class vostok::render::shader_constant_bindings;
class vostok::render::state_cache<ID3D11BlendState,D3D11_BLEND_DESC>;
class vostok::render::state_cache<ID3D11DepthStencilState,D3D11_DEPTH_STENCIL_DESC>;
class vostok::render::state_cache<ID3D11RasterizerState,D3D11_RASTERIZER_DESC>;
class vostok::render::state_cache<ID3D11SamplerState,D3D11_SAMPLER_DESC>;
class vostok::render::texture_storage;
class vostok::render::vector<stlp_std::pair<vostok::fixed_string<64>,ID3D11SamplerState *> >;
class vostok::render::vector<unsigned int>;
class vostok::render::vector<vostok::fs_new::virtual_path_string>;
class vostok::render::vector<vostok::render::res_render_output *>;
class vostok::render::vector<vostok::render::res_state *>;
class vostok::render::vector<vostok::render::untyped_buffer *>;
class vostok::tasks::task;
class vostok::tasks::task_type;
class vostok::fs_new::virtual_path_string;
class vostok::math::rectangle<vostok::math::int2>;
class vostok::render::binary_shader_source;
class vostok::render::enum_buffer_type;
class vostok::render::enum_rt_usage;
class vostok::render::enum_shader_type;
class vostok::render::shader_configuration;
class vostok::resources::managed_resource;
typedef vostok::intrusive_ptr<vostok::render::res_texture,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_ptr;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
class vostok::configs::binary_config;
class vostok::render::res_texture;

/* FORWARD REFS */
class vostok::fixed_string<64>;
class vostok::fixed_vector<vostok::render::sampler_slot,16>;
class vostok::fixed_vector<vostok::render::texture_slot,128>;
class vostok::render::render_target;
class vostok::render::res_declaration;
class vostok::render::res_geometry;
class vostok::render::res_input_layout;
class vostok::render::res_render_output;
class vostok::render::res_sampler_list;
class vostok::render::res_signature;
class vostok::render::res_state;
class vostok::render::res_texture_list;
class vostok::render::res_xs<vostok::render::gs_data>;
class vostok::render::res_xs<vostok::render::ps_data>;
class vostok::render::res_xs<vostok::render::vs_data>;
class vostok::render::res_xs_hw<vostok::render::gs_data>;
class vostok::render::res_xs_hw<vostok::render::ps_data>;
class vostok::render::res_xs_hw<vostok::render::vs_data>;
class vostok::render::sampler_state_descriptor;
class vostok::render::shader_constant_binding;
class vostok::render::shader_constant_buffer;
class vostok::render::shader_constant_host;
class vostok::render::shader_constant_table;
class vostok::render::shader_include_getter;
class vostok::render::state_descriptor;
class vostok::render::untyped_buffer;
class vostok::render::xs_descriptor<vostok::render::gs_data>;
class vostok::render::xs_descriptor<vostok::render::ps_data>;
class vostok::render::xs_descriptor<vostok::render::vs_data>;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::vfs::vfs_iterator;
class vostok::vfs::vfs_notification;

namespace vostok {
namespace render {

class resource_manager : public quasi_singleton< resource_manager > {
public:
			explicit						resource_manager				( configs::binary_config_ptr const& in_config );
											~resource_manager				( );

			res_texture*					create_texture					(
												pcstr									physical_name,
												resources::query_result_for_cook*		parent,
												u32										mip_level_cut,
												bool									use_pool,
												bool									load_async,
												bool									use_converter,
												u32										num_last_mips_used
											);
			res_texture*					create_texture2d				(
												pcstr								user_name,
												u32									width,
												u32									height,
												D3D11_SUBRESOURCE_DATA const*		data,
												DXGI_FORMAT							format,
												D3D11_USAGE							usage,
												u32									mip_levels,
												u32									array_size,
												bool								use_as_render_target
											);
			res_texture*					create_texture3d				(
												pcstr								user_name,
												u32									width,
												u32									height,
												u32									depth,
												D3D11_SUBRESOURCE_DATA const*		data,
												DXGI_FORMAT							format,
												u32									usage,
												u32									mip_levels
											);
	inline	res_texture*					create_texture_cube				(
												pcstr								arg_0,
												u32									arg_1,
												u32									arg_2,
												D3D11_SUBRESOURCE_DATA const*		arg_3,
												DXGI_FORMAT							arg_4,
												u32									arg_5,
												u32									arg_6,
												bool								arg_7
											) { /* no source */ }

			res_texture*					load_texture					(
												pcstr									texture_name,
												resources::query_result_for_cook*		parent,
												u32										mip_level_cut,
												bool									use_pool,
												bool									load_async,
												bool									use_converter,
												u32										num_last_mips_used
											);
			res_texture*					find_texture					( pcstr name );

			void							reload_all_textures				( );

			untyped_buffer*					create_buffer					(
												u32					size,
												pcvoid				data,
												enum_buffer_type	type,
												bool				dynamic,
												bool				staging
											);
			shader_constant_table*			create_const_table				( shader_constant_table const& proto );

			shader_constant_host const*		register_constant_binding		( shader_constant_binding const& binding );

			shader_constant_buffer*			create_constant_buffer			(
												fixed_string< 64 > const&		name,
												enum_shader_type				dest,
												_D3D_CBUFFER_TYPE				type,
												u32								size
											);
			ID3D11SamplerState*				create_sampler_state			( sampler_state_descriptor const& sampler_props );

			void							register_sampler				( pcstr name, ID3D11SamplerState* sampler );
			ID3D11SamplerState*				find_registered_sampler			( pcstr name );

			res_xs_hw< vs_data >*			create_vs_hw					(
												pcstr						name,
												shader_configuration		shader_config,
												shader_include_getter*		include_getter,
												associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > >*	shader_sources
											);
			res_xs_hw< gs_data >*			create_gs_hw					(
												pcstr						name,
												shader_configuration		shader_config,
												shader_include_getter*		include_getter,
												associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > >*	shader_sources
											);
			res_xs_hw< ps_data >*			create_ps_hw					(
												pcstr						name,
												shader_configuration		shader_config,
												shader_include_getter*		include_getter,
												associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > >*	shader_sources
											);
			render_target*					create_render_target			(
												pcstr				name,
												u32					w,
												u32					h,
												DXGI_FORMAT			fmt,
												enum_rt_usage		usage,
												res_texture_ptr		texture,
												u32					first_array_slice_index,
												D3D11_USAGE			memory_usage,
												u32					sample_count,
												u32					mip_level_slice
											);
			render_target*					create_volume_render_target		(
												pcstr				arg_0 /* u32 w */,
												u32					h,
												u32					d,
												u32					arg_3 /* DXGI_FORMAT fmt */,
												DXGI_FORMAT			arg_4 /* enum_rt_usage usage */,
												enum_rt_usage		arg_5 /* D3D11_USAGE memory_usage */,
												D3D11_USAGE			arg_6
											);
			res_xs< vs_data >*				create_vs						( xs_descriptor< vs_data > const& binder );
			res_xs< gs_data >*				create_gs						( xs_descriptor< gs_data > const& binder );
			res_xs< ps_data >*				create_ps						( xs_descriptor< ps_data > const& binder );
			res_state*						create_state					( state_descriptor& descriptor );
			res_declaration*				create_declaration				( D3D11_INPUT_ELEMENT_DESC const* dcl, u32 count );
			res_signature*					create_signature				( ID3D10Blob* signature );
			res_input_layout*				create_input_layout				( res_declaration const* decl, res_signature const* signature );
			res_texture_list*				create_texture_list				( fixed_vector< texture_slot, 128 > const& tex_list );
			res_sampler_list*				create_sampler_list				( fixed_vector< sampler_slot, 16 > const& smp_list );
			res_geometry*					create_geometry					(
												D3D11_INPUT_ELEMENT_DESC const*		decl,
												u32									decl_size,
												u32									vertex_stride,
												untyped_buffer&						vb,
												untyped_buffer&						ib
											);
			res_geometry*					create_geometry					(
												res_declaration*	dcl,
												u32					vertex_stride,
												untyped_buffer&		vb,
												untyped_buffer&		ib
											);
			res_render_output*				create_render_output			( HWND__* window, bool windowed );

			res_texture_ptr					get_color_grading_base_lut		( );

			void							release							( shader_constant_table const* const_table );
			void							release							( shader_constant_buffer const* cbuffer );
			void							release							( res_xs_hw< vs_data > const* vs );
			void							release							( res_xs_hw< gs_data > const* gs );
			void							release							( res_xs_hw< ps_data > const* ps );
			void							release							( render_target const* rt );
			void							release							( res_xs< vs_data > const* vs );
			void							release							( res_xs< gs_data > const* gs );
			void							release							( res_xs< ps_data > const* ps );
			void							release							( res_state const* state );
			void							release							( res_declaration const* dcl );
			void							release							( res_signature const* signature );
			void							release							( res_input_layout const* layout );
			void							release							( res_texture_list const* tex_list );
			void							release							( res_sampler_list const* smp_list );
			void							release							( res_texture const* texture );
			void							release							( untyped_buffer const* buffer );
			void							release							( res_geometry const* geom );
			void							release							( res_render_output const* render_output );

			void							copy							( untyped_buffer* dest, untyped_buffer* source );
	inline	void							copy							(
												untyped_buffer*		arg_0,
												u32					arg_1,
												untyped_buffer*		arg_2,
												u32					arg_3,
												u32					arg_4
											) { /* no source */ }

			void							copy2D							(
												res_texture*	dest,
												u32				dest_x,
												u32				dest_y,
												res_texture*	source,
												u32				src_x,
												u32				src_y,
												u32				size_x,
												u32				size_y,
												u32				dest_mip,
												u32				src_mip
											);

	inline	void							copy3D							(
												res_texture*	arg_0,
												u32				arg_1,
												u32				arg_2,
												u32				arg_3,
												res_texture*	arg_4,
												u32				arg_5,
												u32				arg_6,
												u32				arg_7,
												u32				arg_8,
												u32				arg_9,
												u32				arg_10,
												u32				arg_11,
												u32				arg_12
											) { /* no source */ }

	inline	void							set_compile_error_handler		(
												fastdelegate::FastDelegate< void( enum enum_shader_type, pcstr, pcstr ) > const&	arg_0
											) { /* no source */ }

	inline	bool							add_shader_code					( pcstr arg_0, pcstr arg_1, bool arg_2 ) { /* no source */ }

	inline	bool							copy_texture_from_file			(
												res_texture*						arg_0,
												math::rectangle< math::int2 >		arg_1,
												u32									arg_2,
												pcstr								arg_3
											) { /* no source */ }

	inline	pcstr							get_shader_path					( ) const { /* no source */ }
			pcstr							get_converted_shader_path		( ) const;

			void							reload_shader_sources			( bool is_recompile_shaders );
			void							reload_modified_textures		( );
	inline	void							reload_textures					( ) { /* no source */ }

private:
	inline	void							on_texture_source_changed		( vfs::vfs_notification const& arg_0 ) { /* no source */ }

	inline	void							update_texture_task				(
												ID3D11DeviceContext*	arg_0,
												ID3D11Resource*			arg_1,
												u32						arg_2,
												pbyte					arg_3,
												u32						arg_4,
												u32						arg_5
											) { /* no source */ }

	inline	void							process_files					( vfs::vfs_iterator const& arg_0 ) { /* no source */ }

	inline	void							on_raw_files_load				( resources::queries_result& arg_0 ) { /* no source */ }
public:
			void							on_texture_loaded				(
												resources::queries_result&		data,
												u32								mip_level_cut,
												bool							use_converter,
												u32								num_last_mips_used
											);
			void							on_texture_loaded				( resources::managed_resource_ptr data, pcstr in_name, u32 num_last_mips_used );

	inline	void							tick							( ) { /* no source */ }

private:
			void							on_texture_loaded_staging		( resources::queries_result& data, u32 mip_level_cut, bool use_converter );

	inline	void							copy_texture_from_file_cb		( resources::queries_result& arg_0 ) { /* no source */ }

			res_texture*					create_texture2d_impl			(
												u32									width,
												u32									height,
												D3D11_SUBRESOURCE_DATA const*		data,
												DXGI_FORMAT							format,
												D3D11_USAGE							usage,
												u32									mip_levels,
												u32									array_size,
												bool								use_for_render_target
											);
			void							release_impl					( res_texture const* texture );

public:
			u32								get_texture_video_memory_size	( );
	inline	u32								get_buffers_video_memory_size	( ) const { /* no source */ }
	inline	u32								get_render_target_video_memory	( ) const { /* no source */ }
	inline	fs_new::virtual_path_string		get_full_shader_path			( pcstr arg_0, enum_shader_type arg_1 ) const { /* no source */ }

	inline	void							reload_texture					( pcstr arg_0 ) { /* no source */ }

	inline	void							unregister_all_samplers			( ) { /* no source */ }

			void							bind_samplers_to_shaders		( );

	/* 0x0000 */	/* quasi_singleton< resource_manager > */
	/* 0x0000 */	u32								sh_created;
	/* 0x0004 */	u32								sh_returned;
	/* 0x0008 */	u32								tl_created;
	/* 0x000c */	u32								cb_created;
	/* 0x0010 */	u32								sl_created;
private:
	/* 0x0014 */	ID3D11DeviceContext*			m_deferred_context;
public:
	/* 0x0018 */	u32								m_render_target_video_memory;
	/* 0x001c */	configs::binary_config_ptr		shader_name_to_mask_config;
private:
	/* 0x0020 */	u64								m_num_bytes_of_texture_video_memory;
	/* 0x0028 */	u32								m_num_bytes_of_buffers_video_memory;
	/* 0x002c */	map< resource_manager::shader_name_config_pair, res_xs_hw< vs_data >*, std::less< resource_manager::shader_name_config_pair > >	m_vs_hw_registry;
	/* 0x0044 */	map< resource_manager::shader_name_config_pair, res_xs_hw< gs_data >*, std::less< resource_manager::shader_name_config_pair > >	m_gs_hw_registry;
	/* 0x005c */	map< resource_manager::shader_name_config_pair, res_xs_hw< ps_data >*, std::less< resource_manager::shader_name_config_pair > >	m_ps_hw_registry;
	/* 0x0074 */	map< fs_new::virtual_path_string, render_target*, resource_manager::str_pred >	m_rt_registry;
	/* 0x008c */	map< fs_new::virtual_path_string, res_texture*, resource_manager::str_pred >	m_texture_registry;
	/* 0x00a4 */	set< shader_constant_table*, resource_manager::constant_table_predicate >	m_const_tables;
	/* 0x00bc */	set< shader_constant_buffer*, resource_manager::constant_buffer_predicate >	m_const_buffers;
	/* 0x00d4 */	vector< untyped_buffer* >		m_buffers;
	/* 0x00e0 */	tasks::task_type*				m_tasks_type;
	/* 0x00e8 */	tasks::task						m_parent_task;
public:
	/* 0x0148 */	set< res_xs< vs_data >*, resource_manager::compare_shader_predicate< vs_data > >	m_v_shaders;
	/* 0x0160 */	set< res_xs< gs_data >*, resource_manager::compare_shader_predicate< gs_data > >	m_g_shaders;
	/* 0x0178 */	set< res_xs< ps_data >*, resource_manager::compare_shader_predicate< ps_data > >	m_p_shaders;
private:
	/* 0x0190 */	vector< res_state* >			m_states;
	/* 0x019c */	state_cache< ID3D11RasterizerState, D3D11_RASTERIZER_DESC >	m_rs_cache;
	/* 0x01a8 */	state_cache< ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC >	m_dss_cache;
	/* 0x01b4 */	state_cache< ID3D11BlendState, D3D11_BLEND_DESC >	m_bs_cache;
	/* 0x01c0 */	state_cache< ID3D11SamplerState, D3D11_SAMPLER_DESC >	m_sampler_cache;
	/* 0x01cc */	set< res_declaration*, resource_manager::compare_predicate< res_declaration > >	m_declarations;
	/* 0x01e4 */	set< res_signature*, resource_manager::compare_predicate< res_signature > >	m_signatures;
	/* 0x01fc */	set< res_input_layout*, resource_manager::compare_predicate< res_input_layout > >	m_input_layouts;
	/* 0x0214 */	set< res_texture_list*, resource_manager::compare_member_predicate< res_texture_list > >	m_texture_lists;
	/* 0x022c */	set< res_sampler_list*, resource_manager::compare_member_predicate< res_sampler_list > >	m_sampler_lists;
	/* 0x0244 */	vector< res_render_output* >	m_render_outputs;
	/* 0x0250 */	vector< std::pair< fixed_string< 64 >, ID3D11SamplerState* > >	m_samplers_registry;
	/* 0x025c */	vector< u32 >					m_vs_ids;
	/* 0x0268 */	vector< u32 >					m_ps_ids;
	/* 0x0274 */	vector< u32 >					m_gs_ids;
	/* 0x0280 */	set< res_geometry*, resource_manager::compare_member_predicate< res_geometry > >	m_geometries;
	/* 0x0298 */	bool							m_loading_incomplete;
	/* 0x029c */	shader_constant_bindings		m_const_bindings;
	/* 0x02a8 */	texture_storage*				m_texture_storage;
	/* 0x02ac */	texture_storage*				m_texture_storage_staging;
	/* 0x02b0 */	fastdelegate::FastDelegate< void( enum enum_shader_type, pcstr, pcstr ) >	m_compile_error_handler;
	/* 0x02b8 */	bool							m_is_shader_reloading;
	/* 0x02b9 */	bool							m_need_recompile_shader_if_source_reloaded;
	/* 0x02bc */	vector< fs_new::virtual_path_string >	m_textures_to_reload;
	/* 0x02c8 */	res_texture_ptr					m_color_grading_base_lut;
	/* 0x02cc */	u32								m_watcher_subscribe_id;
}; // class resource_manager

STATIC_SIZE_ASSERT(resource_manager, 0x2D0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_H_INCLUDED
