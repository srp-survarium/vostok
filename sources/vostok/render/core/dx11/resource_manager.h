#ifndef VOSTOK_RENDER_CORE_DX11_RESOURCE_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RESOURCE_MANAGER_H_INCLUDED

#include <cstring>
#include <functional>
#include <fastdelegate/fastdelegate.h>
#include <vostok/configs_binary_config.h>
#include <vostok/fixed_string.h>
#include <vostok/fixed_vector.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/math_int2.h>
#include <vostok/math_rectangle.h>
#include <vostok/render/core/binary_shader_key_type.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/render_target.h>
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/shader_configuration.h>
#include <vostok/render/core/shader_constant_bindings.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/render/core/state_cache.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/tasks_task.h>
#include <vostok/vectora.h>

namespace vostok {

namespace resources {
class queries_result;
class query_result_for_cook;
} // namespace resources

namespace vfs {
class vfs_iterator;
struct vfs_notification;
} // namespace vfs

namespace render {

class res_declaration;
class res_geometry;
class res_input_layout;
class res_render_output;
class res_signature;
class res_state;
class sampler_state_descriptor;
class shader_constant_binding;
class shader_constant_buffer;
class shader_constant_host;
class shader_constant_table;
// claude@NOTE: shader_include_getter is a struct (see core/shader_include_getter.h);
// the class-key is part of the MSVC mangling, and the target spells it
// PAUshader_include_getter, so the forward declaration must say struct.
struct shader_include_getter;
class state_descriptor;
class texture_slot;
class texture_storage;

template < typename shader_data > class res_xs;
template < typename shader_data > class res_xs_hw;
template < typename shader_data > class xs_descriptor;

struct gs_data;
struct ps_data;
struct vs_data;

typedef fastdelegate::FastDelegate<
	void( enum_shader_type, pcstr, pcstr )
> shader_compile_error_handler;

class resource_manager : public quasi_singleton<resource_manager> {
	friend class texture_pool;

public:
	struct str_pred {
		bool operator()(
			fs_new::virtual_path_string const& left,
			fs_new::virtual_path_string const& right
		) const
		{
			return left < right;
		}
	};

	struct shader_source {
		shader_source( )
#ifdef MASTER_GOLD
			: data( memory::g_mt_allocator ),
			  time( 0 ),
			  includes( memory::g_mt_allocator )
#else // #ifdef MASTER_GOLD
			: data( debug::g_mt_allocator ),
			  time( 0 ),
			  includes( debug::g_mt_allocator )
#endif // #ifdef MASTER_GOLD
		{
		}

		vectora<u8> data;
		u32 time;
		vectora<pcstr> includes;
	};

	struct shader_name_config_pair {
		shader_name_config_pair(
			pcstr name,
			shader_configuration const& config
		)
			: name( name ), config( config )
		{
		}

		shader_name_config_pair& operator=(
			shader_name_config_pair const& other
		)
		{
			name = other.name;
			config = other.config;
			return *this;
		}

		bool operator<( shader_name_config_pair const& other ) const
		{
			// FUNCTION BODY[0x124800]
			int const comparison = std::strcmp( name, other.name );
			return comparison < 0
				|| (comparison == 0
					&& union_base::operator<( config, other.config ));
		}

		bool operator==( shader_name_config_pair const& other ) const
		{
			return std::strcmp( name, other.name ) == 0
				&& config.configuration[0] == other.config.configuration[0]
				&& config.configuration[1] == other.config.configuration[1];
		}

		pcstr name;
		shader_configuration config;
	};

	template < typename resource_type >
	struct compare_predicate {
		bool operator()(
			resource_type const* const left,
			resource_type const* const right
		) const
		{
			// FUNCTION BODY[0x126aa0] for res_signature
			return *left < *right;
		}
	};

	template < typename resource_type >
	struct compare_member_predicate {
		bool operator()(
			resource_type* const left,
			resource_type* const right
		) const
		{
			// FUNCTION BODY[0x126b70] for res_geometry
			return left->compare( *right ) < 0;
		}

		template < typename descriptor_type >
		bool operator()(
			descriptor_type const& left,
			resource_type const* const right
		) const
		{
			return right->compare( left ) > 0;
		}

		template < typename descriptor_type >
		bool operator()(
			resource_type const* const left,
			descriptor_type const& right
		) const
		{
			return left->compare( right ) < 0;
		}
	};

	template < typename shader_data >
	struct compare_shader_predicate {
		bool operator()(
			res_xs<shader_data> const* const left,
			res_xs<shader_data> const* const right
		) const
		{
			return left->compare( *right ) < 0;
		}

		bool operator()(
			xs_descriptor<shader_data> const& left,
			res_xs<shader_data> const* const right
		) const
		{
			return right->compare( left ) > 0;
		}

		bool operator()(
			res_xs<shader_data> const* const left,
			xs_descriptor<shader_data> const& right
		) const
		{
			return left->compare( right ) < 0;
		}
	};

	struct constant_buffer_predicate {
		bool operator()(
			shader_constant_buffer const* const left,
			shader_constant_buffer const* const right
		) const;
	};

	struct constant_table_predicate {
		bool operator()(
			shader_constant_table const* const left,
			shader_constant_table const* const right
		) const
		{
			return left < right;
		}
	};

	typedef map<shader_name_config_pair, res_xs_hw<vs_data>*> map_vs_hw;
	typedef map<shader_name_config_pair, res_xs_hw<gs_data>*> map_gs_hw;
	typedef map<shader_name_config_pair, res_xs_hw<ps_data>*> map_ps_hw;
	typedef map<fs_new::virtual_path_string, render_target*, str_pred> map_rt;
	typedef map<fs_new::virtual_path_string, res_texture*, str_pred> map_texture;

public:
	explicit resource_manager( configs::binary_config_ptr const& config );
	~resource_manager( );

	res_texture* create_texture(
		pcstr physical_name,
		resources::query_result_for_cook* parent,
		u32 mip_level_cut,
		bool use_pool,
		bool load_async,
		bool use_converter,
		u32 num_last_mips_used
	);
	res_texture* create_texture2d(
		pcstr user_name,
		u32 width,
		u32 height,
		D3D11_SUBRESOURCE_DATA const* data,
		DXGI_FORMAT format,
		D3D11_USAGE usage,
		u32 mip_levels,
		u32 array_size,
		bool use_as_render_target
	);
	res_texture* create_texture3d(
		pcstr user_name,
		u32 width,
		u32 height,
		u32 depth,
		D3D11_SUBRESOURCE_DATA const* data,
		DXGI_FORMAT format,
		u32 usage,
		u32 mip_levels
	);
	res_texture* create_texture_cube(
		pcstr, u32, u32, D3D11_SUBRESOURCE_DATA const*,
		DXGI_FORMAT, u32, u32, bool
	)
	{
		return 0;
	}
	res_texture* load_texture(
		pcstr texture_name,
		resources::query_result_for_cook* parent,
		u32 mip_level_cut,
		bool use_pool,
		bool load_async,
		bool use_converter,
		u32 num_last_mips_used
	);
	res_texture* find_texture( pcstr name );
	void reload_all_textures( );

	untyped_buffer* create_buffer(
		u32 size,
		pcvoid data,
		enum_buffer_type type,
		bool dynamic,
		bool staging
	);
	shader_constant_table* create_const_table( shader_constant_table const& proto );
	shader_constant_host const* register_constant_binding(
		shader_constant_binding const& binding
	);
	shader_constant_buffer* create_constant_buffer(
		fixed_string<64> const& name,
		enum_shader_type dest,
		_D3D_CBUFFER_TYPE type,
		u32 size
	);
	ID3D11SamplerState* create_sampler_state(
		sampler_state_descriptor const& sampler_props
	);
	void register_sampler( pcstr name, ID3D11SamplerState* sampler );
	ID3D11SamplerState* find_registered_sampler( pcstr name );

	res_xs_hw<vs_data>* create_vs_hw(
		pcstr name,
		shader_configuration shader_config,
		shader_include_getter* include_getter,
		binary_shader_sources_type* shader_sources
	);
	res_xs_hw<gs_data>* create_gs_hw(
		pcstr name,
		shader_configuration shader_config,
		shader_include_getter* include_getter,
		binary_shader_sources_type* shader_sources
	);
	res_xs_hw<ps_data>* create_ps_hw(
		pcstr name,
		shader_configuration shader_config,
		shader_include_getter* include_getter,
		binary_shader_sources_type* shader_sources
	);

	render_target* create_render_target(
		pcstr name,
		u32 width,
		u32 height,
		DXGI_FORMAT format,
		enum_rt_usage usage,
		res_texture_ptr texture,
		u32 first_array_slice_index,
		D3D11_USAGE memory_usage,
		u32 sample_count,
		u32 mip_level_slice
	);
	render_target* create_volume_render_target(
		pcstr name,
		u32 width,
		u32 height,
		u32 depth,
		DXGI_FORMAT format,
		enum_rt_usage usage,
		D3D11_USAGE memory_usage
	);
	res_xs<vs_data>* create_vs( xs_descriptor<vs_data> const& binder );
	res_xs<gs_data>* create_gs( xs_descriptor<gs_data> const& binder );
	res_xs<ps_data>* create_ps( xs_descriptor<ps_data> const& binder );
	res_state* create_state( state_descriptor& descriptor );
	res_declaration* create_declaration(
		D3D11_INPUT_ELEMENT_DESC const* declaration,
		u32 count
	);
	res_signature* create_signature( ID3D10Blob* signature );
	res_input_layout* create_input_layout(
		res_declaration const* declaration,
		res_signature const* signature
	);
	res_texture_list* create_texture_list(
		fixed_vector<texture_slot, 128> const& textures
	);
	res_sampler_list* create_sampler_list(
		fixed_vector<sampler_slot, 16> const& samplers
	);
	res_geometry* create_geometry(
		D3D11_INPUT_ELEMENT_DESC const* declaration,
		u32 declaration_size,
		u32 vertex_stride,
		untyped_buffer& vertex_buffer,
		untyped_buffer& index_buffer
	);
	template < int size >
	res_geometry* create_geometry(
		D3D11_INPUT_ELEMENT_DESC const (&declaration)[size],
		u32 vertex_stride,
		untyped_buffer& vertex_buffer,
		untyped_buffer& index_buffer
	)
	{
		return create_geometry(
			(D3D11_INPUT_ELEMENT_DESC const*)&declaration,
			size,
			vertex_stride,
			vertex_buffer,
			index_buffer
		);
	}
	res_geometry* create_geometry(
		res_declaration* declaration,
		u32 vertex_stride,
		untyped_buffer& vertex_buffer,
		untyped_buffer& index_buffer
	);
	res_render_output* create_render_output( HWND__* window, bool windowed );

	res_texture_ptr get_color_grading_base_lut( );

	void release( shader_constant_table const* table );
	void release( shader_constant_buffer const* buffer );
	void release( res_xs_hw<vs_data> const* shader );
	void release( res_xs_hw<gs_data> const* shader );
	void release( res_xs_hw<ps_data> const* shader );
	void release( render_target const* target );
	void release( res_xs<vs_data> const* shader );
	void release( res_xs<gs_data> const* shader );
	void release( res_xs<ps_data> const* shader );
	void release( res_state const* state );
	void release( res_declaration const* declaration );
	void release( res_signature const* signature );
	void release( res_input_layout const* layout );
	void release( res_texture_list const* textures );
	void release( res_sampler_list const* samplers );
	void release( res_texture const* texture );
	void release( untyped_buffer const* buffer );
	void release( res_geometry const* geometry );
	void release( res_render_output const* output );

	void copy( untyped_buffer* dest, untyped_buffer* source );
	void copy( untyped_buffer*, u32, untyped_buffer*, u32, u32 ) { }
	void copy2D(
		res_texture* dest,
		u32 dest_x,
		u32 dest_y,
		res_texture* source,
		u32 source_x,
		u32 source_y,
		u32 size_x,
		u32 size_y,
		u32 dest_mip,
		u32 source_mip
	);
	void copy3D(
		res_texture*, u32, u32, u32,
		res_texture*, u32, u32, u32,
		u32, u32, u32, u32, u32
	)
	{
	}

	void set_compile_error_handler( shader_compile_error_handler const& handler )
	{
		m_compile_error_handler = handler;
	}
	bool add_shader_code( pcstr, pcstr, bool ) { return false; }
	bool copy_texture_from_file(
		res_texture*,
		math::rectangle<math::int2>,
		u32,
		pcstr
	)
	{
		return false;
	}
	pcstr get_shader_path( ) const { return "shaders/"; }
	pcstr get_converted_shader_path( ) const;
	void reload_shader_sources( bool is_recompile_shaders );
	void reload_modified_textures( );
	void reload_textures( ) { reload_all_textures( ); }

private:
	void on_texture_source_changed( vfs::vfs_notification const& ) { }
	void update_texture_task(
		ID3D11DeviceContext*, ID3D11Resource*, u32, pbyte, u32, u32
	)
	{
	}
	void process_files( vfs::vfs_iterator const& ) { }
	void on_raw_files_load( resources::queries_result& ) { }

public:
	void on_texture_loaded(
		resources::queries_result& data,
		u32 mip_level_cut,
		bool use_converter,
		u32 num_last_mips_used
	);
	void on_texture_loaded(
		resources::managed_resource_ptr data,
		pcstr name,
		u32 num_last_mips_used
	);
	void tick( ) { }

private:
	void on_texture_loaded_staging(
		resources::queries_result& data,
		u32 mip_level_cut,
		bool use_converter
	);
	void copy_texture_from_file_cb( resources::queries_result& ) { }
	res_texture* create_texture2d_impl(
		u32 width,
		u32 height,
		D3D11_SUBRESOURCE_DATA const* data,
		DXGI_FORMAT format,
		D3D11_USAGE usage,
		u32 mip_levels,
		u32 array_size,
		bool use_for_render_target
	);
	void release_impl( res_texture const* texture );

	template < typename shader_data >
	res_xs_hw<shader_data>* create_xs_hw_impl(
		pcstr name,
		shader_configuration shader_config,
		shader_include_getter* include_getter,
		binary_shader_sources_type* shader_sources
	);
	template < typename shader_data >
	void release_impl( res_xs_hw<shader_data> const* shader );

public:
	template < typename T >
	static void call_resource_destructor( T* resource )
	{
		resource->~T( );
	}

	u32 get_texture_video_memory_size( );
	u32 get_buffers_video_memory_size( ) const
	{
		return m_num_bytes_of_buffers_video_memory;
	}
	u32 get_render_target_video_memory( ) const
	{
		return m_render_target_video_memory;
	}
	fs_new::virtual_path_string get_full_shader_path(
		pcstr short_path,
		enum_shader_type
	) const
	{
		return fs_new::virtual_path_string( short_path );
	}
	void reload_texture( pcstr name ) { m_textures_to_reload.push_back( name ); }
	void unregister_all_samplers( ) { m_samplers_registry.clear( ); }
	void bind_samplers_to_shaders( );

public:
	u32 sh_created;
	u32 sh_returned;
	u32 tl_created;
	u32 cb_created;
	u32 sl_created;

private:
	ID3D11DeviceContext* m_deferred_context;

public:
	u32 m_render_target_video_memory;
	configs::binary_config_ptr shader_name_to_mask_config;

private:
	u64 m_num_bytes_of_texture_video_memory;
	u32 m_num_bytes_of_buffers_video_memory;
	map_vs_hw m_vs_hw_registry;
	map_gs_hw m_gs_hw_registry;
	map_ps_hw m_ps_hw_registry;
	map_rt m_rt_registry;
	map_texture m_texture_registry;
	set<shader_constant_table*, constant_table_predicate> m_const_tables;
	set<shader_constant_buffer*, constant_buffer_predicate> m_const_buffers;
	vector<untyped_buffer*> m_buffers;
	tasks::task_type* m_tasks_type;
	tasks::task m_parent_task;

public:
	set<res_xs<vs_data>*, compare_shader_predicate<vs_data> > m_v_shaders;
	set<res_xs<gs_data>*, compare_shader_predicate<gs_data> > m_g_shaders;
	set<res_xs<ps_data>*, compare_shader_predicate<ps_data> > m_p_shaders;

private:
	vector<res_state*> m_states;
	state_cache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC> m_rs_cache;
	state_cache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC> m_dss_cache;
	state_cache<ID3D11BlendState, D3D11_BLEND_DESC> m_bs_cache;
	state_cache<ID3D11SamplerState, D3D11_SAMPLER_DESC> m_sampler_cache;
	set<res_declaration*, compare_predicate<res_declaration> > m_declarations;
	set<res_signature*, compare_predicate<res_signature> > m_signatures;
	set<res_input_layout*, compare_predicate<res_input_layout> > m_input_layouts;
	set<res_texture_list*, compare_member_predicate<res_texture_list> > m_texture_lists;
	set<res_sampler_list*, compare_member_predicate<res_sampler_list> > m_sampler_lists;
	vector<res_render_output*> m_render_outputs;
	vector<std::pair<fixed_string<64>, ID3D11SamplerState*> > m_samplers_registry;
	vector<u32> m_vs_ids;
	vector<u32> m_ps_ids;
	vector<u32> m_gs_ids;
	set<res_geometry*, compare_member_predicate<res_geometry> > m_geometries;
	bool m_loading_incomplete;
	shader_constant_bindings m_const_bindings;
	texture_storage* m_texture_storage;
	texture_storage* m_texture_storage_staging;
	shader_compile_error_handler m_compile_error_handler;
	bool m_is_shader_reloading;
	bool m_need_recompile_shader_if_source_reloaded;
	vector<fs_new::virtual_path_string> m_textures_to_reload;
	res_texture_ptr m_color_grading_base_lut;
	u32 m_watcher_subscribe_id;
};

STATIC_SIZE_ASSERT( resource_manager::str_pred, 0x1 );
STATIC_SIZE_ASSERT( resource_manager::shader_source, 0x24 );
STATIC_SIZE_ASSERT( resource_manager::shader_name_config_pair, 0x18 );
STATIC_SIZE_ASSERT( resource_manager, 0x2D0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RESOURCE_MANAGER_H_INCLUDED
