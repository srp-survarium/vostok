#ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_H_INCLUDED

#include <boost/function.hpp>
#include <vostok/configs_binary_config_value.h>
#include <vostok/fixed_string.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/custom_config.h>
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
class effect_loader;
struct effect_options_descriptor;

class effect_manager : public quasi_singleton<effect_manager> {
public:
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

	struct effect_holder_struct {
		effect_holder_struct( ) : descriptor( 0 ), effect( 0 ) { }

		effect_descriptor* descriptor;
		custom_config_ptr config;
		res_effect* effect;
	};

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
			// STATE[STUB]
			// FUNCTION BODY[0x12a100]
		}

		res_effect_ptr effect;
		effect_descriptor* descriptor;
		custom_config_ptr config;
		u32 crc;
	};

	struct shader_cache_info {
		shader_cache_info( ) { }

		fs_new::virtual_path_string vertex_shader_name;
		fs_new::virtual_path_string pixel_shader_name;
		shader_configuration configuration;
	};

public:
	effect_manager( );
	~effect_manager( );

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
		boost::function<void( )> callback
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
		boost::function<void( )>
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
		vector<fs_new::virtual_path_string> const& changed_defines
	);

private:
	void on_effects_recompiled(
		vectora<effect_to_recompile_struct>* effects,
		resources::queries_result& data
	);

public:
	effect_descriptor* get_effect_descriptor_by_name( pcstr name );
	void delete_pending_effect( res_effect_ptr* effect )
	{
		m_effects_deleted_in_pending.push_back( effect );
	}

private:
	effect_descriptor* find_effect( pcstr, pcstr ) { return 0; }
	void load_effects( ) { }
	void load_raw_file( resources::queries_result& ) { }
	effect_descriptor* make_effect( u64 ) { return 0; }

private:
	bool m_is_effects_query_processing;
	vectora<shader_cache_info> m_shader_cache_info;

public:
	bool force_sync;

private:
	set<res_pass*, compare_predicate<res_pass> > m_passes;
	set<res_effect*, compare_predicate<res_effect> > m_shaders;
	set<
		res_shader_technique*,
		compare_predicate<res_shader_technique>
	> m_techniques;
	vector<effect_holder_struct> m_effects;
	map<fixed_string<128>, effect_descriptor*> m_effect_descriptors;
	map<fixed_string<128>, effect_descriptor*> m_effect_descriptors_by_texture;
	bool m_loading_incomplete;
	vector<res_effect_ptr*> m_effects_deleted_in_pending;
};

STATIC_SIZE_ASSERT( effect_manager::compare_predicate<res_effect>, 0x1 );
STATIC_SIZE_ASSERT( effect_manager::effect_holder_struct, 0xC );
STATIC_SIZE_ASSERT( effect_manager::effect_to_recompile_struct, 0x10 );
STATIC_SIZE_ASSERT( effect_manager::shader_cache_info, 0x238 );
STATIC_SIZE_ASSERT( effect_manager, 0xAC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_EFFECT_MANAGER_H_INCLUDED
