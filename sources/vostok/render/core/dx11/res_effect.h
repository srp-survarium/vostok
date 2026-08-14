#ifndef VOSTOK_RENDER_CORE_DX11_RES_EFFECT_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_EFFECT_H_INCLUDED

#include <vostok/associative_vector.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/render/core/custom_config.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/res_shader_technique.h>
#include <vostok/render/core/shader_configuration.h>
#include <vostok/render/core/shader_defines.h>
#include <vostok/render/core/texture_named_instance.h>
#include <vostok/resources_resource_ptr.h>
#include <vostok/resources_unmanaged_resource.h>

struct ID3D10Blob;

namespace vostok {
namespace render {

class effect_compiler;
class effect_cook;
class effect_descriptor;
class effect_manager;
class res_declaration;
class res_effect;

struct binary_shader_source : public resources::unmanaged_resource {
	binary_shader_source( )
		: m_shader_byte_code( 0 ),
		  m_compiled_shader_byte_code( 0 ),
		  m_shader_byte_code_size( 0 ),
		  error_code( 0 ),
		  shader_type( enum_shader_type_vertex )
	{
	}

	virtual ~binary_shader_source( ) { }

	resources::managed_resource_ptr shader_source;
	void* m_shader_byte_code;
	void* m_compiled_shader_byte_code;
	u32 m_shader_byte_code_size;
	ID3D10Blob* error_code;
	shader_configuration configuration;
	fs_new::virtual_path_string shader_name;
	enum_shader_type shader_type;
};

typedef resources::resource_ptr<
	binary_shader_source,
	resources::unmanaged_intrusive_base
> binary_shader_source_ptr;

struct binary_shader_key_type {
	binary_shader_key_type(
		pcstr shader_name,
		enum_shader_type type,
		shader_configuration configuration
	)
		: configuration( configuration ),
		  shader_name( shader_name ),
		  type( type )
	{
	}

	shader_configuration configuration;
	fs_new::virtual_path_string shader_name;
	enum_shader_type type;
};

inline bool operator<(
	binary_shader_key_type const& left,
	binary_shader_key_type const& right
)
{
	return left.shader_name < right.shader_name
		|| (left.shader_name == right.shader_name
			&& (union_base::operator<( left.configuration, right.configuration )
				|| (left.configuration.configuration[0]
						== right.configuration.configuration[0]
					&& left.configuration.configuration[1]
						== right.configuration.configuration[1]
					&& left.type < right.type)));
}

inline bool operator==(
	binary_shader_key_type const& left,
	binary_shader_key_type const& right
)
{
	return left.shader_name == right.shader_name
		&& left.configuration.configuration[0]
			== right.configuration.configuration[0]
		&& left.configuration.configuration[1]
			== right.configuration.configuration[1]
		&& left.type == right.type;
}

typedef associative_vector<
	binary_shader_key_type,
	binary_shader_source_ptr,
	render::vector
> binary_shader_sources_type;

struct binary_shader_cook_data {
	binary_shader_cook_data(
		shader_configuration in_configuration,
		fs_new::virtual_path_string in_shader_name,
		enum_shader_type in_shader_type,
		bool in_is_need_check_time,
		res_effect* in_effect_resource
	)
		: configuration( in_configuration ),
		  shader_name( in_shader_name ),
		  shader_type( in_shader_type ),
		  is_need_check_time( in_is_need_check_time ),
		  effect_resource( in_effect_resource )
	{
	}

	res_effect* effect_resource;
	shader_configuration configuration;
	fs_new::virtual_path_string shader_name;
	enum_shader_type shader_type;
	bool is_need_check_time;
};

struct effect_compile_data {
	effect_compile_data(
		effect_descriptor* descriptor,
		custom_config_ptr config,
		u32 crc,
		bool add_to_array = true
	)
		: descriptor( descriptor ),
		  config( config ),
		  crc( crc ),
		  add_to_array( add_to_array )
	{
	}

	effect_descriptor* descriptor;
	custom_config_ptr config;
	u32 crc;
	bool add_to_array;
};

class res_effect : public resources::unmanaged_resource {
private:
	friend class effect_compiler;
	friend class effect_cook;
	friend class effect_manager;
	virtual ~res_effect( );
	void destroy_impl( ) const { }

public:
	vector<texture_named_instance> m_used_textures;

	void push_texture_unique( res_texture* texture, pcstr path );

	res_effect( ) : m_cur_technique( 0 ), m_registered( false )
	{
	}

	void get_max_used_texture_dimension( u32& width, u32& height )
	{
		width = 0;
		height = 0;
	}
	u32 get_max_pixel_shader_instruction_count( ) const { return 0; }
	void mark_registered( ) { m_registered = true; }
	bool is_registered( ) const { return m_registered; }
	void init_layouts( res_declaration const& declaration )
	{
		for ( u32 index = 0; index < m_techniques.size( ); ++index )
			m_techniques[index]->init_layouts( declaration );
	}
	bool equal( res_effect const& other ) const
	{
		return m_techniques == other.m_techniques;
	}
	bool equal( res_effect const* other ) const { return equal( *other ); }
	bool apply( u32 technique_id, u32 pass_id )
	{
		return select_technique( technique_id ) && apply_pass( pass_id );
	}
	bool select_technique( u32 technique_id )
	{
		if ( technique_id >= m_techniques.size( ) )
			return false;
		m_cur_technique = technique_id;
		return true;
	}
	bool apply_pass( u32 id );
	res_shader_technique const* get_technique( u32 index ) const
	{
		return m_techniques[index].c_ptr( );
	}

private:
	typedef vector<res_shader_technique_ptr> techniques_array_type;

	u32 m_cur_technique;

public:
	techniques_array_type m_techniques;

private:
	bool m_registered;
};

typedef resources::resource_ptr<
	res_effect,
	resources::unmanaged_intrusive_base
> res_effect_ptr;

typedef res_effect_ptr ref_effect;

STATIC_SIZE_ASSERT( binary_shader_source, 0x248 );
STATIC_SIZE_ASSERT( binary_shader_key_type, 0x128 );
STATIC_SIZE_ASSERT( binary_shader_cook_data, 0x138 );
STATIC_SIZE_ASSERT( effect_compile_data, 0x10 );
STATIC_SIZE_ASSERT( res_effect, 0x128 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_EFFECT_H_INCLUDED
