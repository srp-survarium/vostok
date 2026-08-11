#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_H_INCLUDED

#include <boost/function.hpp>
#include <vostok/configs_binary_config_value.h>
#include <vostok/fixed_string.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/quasi_singleton.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/core/shader_configuration.h>
#include <vostok/vectora.h>

namespace vostok {

namespace resources {
class queries_result;
} // namespace resources

namespace render {

class effect_descriptor;
struct effect_loader;

class effect_manager : public quasi_singleton<effect_manager> {
public:
	typedef boost::function<void( )> creation_callback_type;

	effect_manager( );
	~effect_manager( );

	struct effect_holder_struct {
		effect_holder_struct( ) : descriptor( 0 ), effect( 0 ) { }

		effect_descriptor* descriptor;
		custom_config_ptr config;
		res_effect* effect;
	};

	void add_effect(
		effect_descriptor* descriptor,
		custom_config_ptr const& config,
		res_effect* effect
	);
	void remove_effect( res_effect* effect );
	res_pass* create_pass( res_pass const& pass );
	void delete_pass( res_pass const* pass );
	res_shader_technique* create_effect_technique(
		res_shader_technique const& technique
	);
	void delete_effect_technique( res_shader_technique const* technique );
	template < typename effect_descriptor_type >
	res_effect_ptr create_effect( );
	template < typename effect_descriptor_type >
	res_effect_ptr create_effect( effect_options_descriptor const& options );
	template < typename effect_descriptor_type >
	void create_effect( res_effect_ptr* out_effect );
	template < typename effect_descriptor_type >
	void create_effect(
		res_effect_ptr* out_effect,
		effect_options_descriptor const& options
	);
	template < typename effect_descriptor_type >
	void create_effect(
		effect_loader* loader,
		effect_options_descriptor const& options
	);

	res_effect_ptr create_effect( pcstr, effect_options_descriptor const& )
	{
		return res_effect_ptr( );
	}
	res_effect_ptr create_effect(
		pcstr,
		configs::binary_config_value const&,
		bool
	)
	{
		return res_effect_ptr( );
	}
	res_effect_ptr create_effect( pcstr, custom_config_value const&, bool )
	{
		return res_effect_ptr( );
	}
	res_effect_ptr create_effect( pcstr, pcstr ) { return res_effect_ptr( ); }
	res_effect_ptr create_effect( effect_descriptor*, pcstr )
	{
		return res_effect_ptr( );
	}

private:
	void on_effect_created(
		res_effect_ptr* out_effect,
		resources::queries_result& data
	);
	void on_async_effect_created(
		resources::queries_result& data,
		res_effect_ptr* out_effect,
		effect_descriptor* descriptor
	);
	void on_async_effect_created_callback(
		resources::queries_result& data,
		res_effect_ptr* out_effect,
		effect_descriptor* descriptor,
		creation_callback_type callback
	);
	res_effect_ptr create_new_effect(
		effect_descriptor& descriptor,
		custom_config_ptr const& config,
		u32 crc
	);
	void create_new_effect(
		res_effect_ptr* out_effect,
		effect_descriptor* descriptor,
		custom_config_ptr const& config,
		u32 crc
	);
	void create_new_effect(
		res_effect_ptr*,
		effect_descriptor*,
		custom_config_ptr const&,
		u32,
		creation_callback_type
	)
	{
	}
	void create_new_effect(
		effect_loader*,
		effect_descriptor*,
		custom_config_ptr const&,
		u32
	)
	{
	}

public:
	void delete_effect( res_effect const* effect )
	{
		remove_effect( const_cast<res_effect*>( effect ) );
	}
	void register_effect_desctiptor( pcstr name, effect_descriptor* descriptor );
	void recompile_shaders( vector<fs_new::virtual_path_string> const& ) { }
	void recompile_shaders_async(
		vector<fs_new::virtual_path_string> const& in_changed_defines
	);

private:
	typedef map<fixed_string<128>, effect_descriptor*> map_effect_descriptors;
	typedef map_effect_descriptors::iterator map_effect_descriptors_it;
	typedef vector<effect_holder_struct> effects_vector_type;
	typedef map<u32, effect_descriptor*> map_effect_descriptors_by_id;

	struct shader_cache_info {
		shader_cache_info( ) { }

		fs_new::virtual_path_string vertex_shader_name;
		fs_new::virtual_path_string pixel_shader_name;
		shader_configuration configuration;
	};

	bool m_is_effects_query_processing;
	typedef vectora<shader_cache_info> shader_cache_info_vector;
	shader_cache_info_vector m_shader_cache_info;

	struct effect_to_recompile_struct {
		effect_to_recompile_struct(
			res_effect_ptr effect,
			effect_descriptor* descriptor,
			custom_config_ptr config,
			u32 crc
		)
			: effect( effect ),
			  descriptor( descriptor ),
			  config( config ),
			  crc( crc )
		{
		}

		res_effect_ptr effect;
		effect_descriptor* descriptor;
		custom_config_ptr config;
		u32 crc;
	};

	typedef vectora<effect_to_recompile_struct> effects_to_recompile_type;

	void on_effects_recompiled(
		effects_to_recompile_type* effects,
		resources::queries_result& data
	);

public:
	template <typename T>
	static void call_resource_destructor( T* resource) { resource->~T();};

	effect_descriptor* get_effect_descriptor_by_name( pcstr name );
	void delete_pending_effect( res_effect_ptr* effect )
	{
		m_effects_deleted_in_pending.push_back( effect );
	}
	bool force_sync;

private:
	effect_descriptor* find_effect( pcstr, pcstr ) { return 0; }
	void load_effects( ) { }
	void load_raw_file( resources::queries_result& ) { }
	effect_descriptor* make_effect( u64 ) { return 0; }

	template < typename resource_type >
	struct compare_predicate {
		bool operator()(
			resource_type const* const left,
			resource_type const* const right
		) const
		{
			return left < right;
		}
	};

	typedef set<res_pass*, compare_predicate<res_pass> > passes_type;
	typedef set<res_effect*, compare_predicate<res_effect> > effects_type;
	typedef set<
		res_shader_technique*,
		compare_predicate<res_shader_technique>
	> techniques_type;

	passes_type m_passes;
	effects_type m_shaders;
	techniques_type m_techniques;
	effects_vector_type m_effects;
	map_effect_descriptors m_effect_descriptors;
	map_effect_descriptors m_effect_descriptors_by_texture;
	bool m_loading_incomplete;
	typedef vector<res_effect_ptr*> effects_deleted_in_pending_type;
	effects_deleted_in_pending_type m_effects_deleted_in_pending;
};

STATIC_SIZE_ASSERT( effect_manager::effect_holder_struct, 0xC );
STATIC_SIZE_ASSERT( effect_manager, 0xAC );

} // namespace render
} // namespace vostok

#include <vostok/render/core/effect_manager_inline.h>

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_H_INCLUDED
