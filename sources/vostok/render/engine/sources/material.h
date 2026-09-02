// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_H_INCLUDED
#include <boost/noncopyable.hpp>
#include <vostok/configs_binary_config.h>
#include <vostok/fixed_string.h>
#include <vostok/render/facade/vertex_input_type.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace render {

struct material_effects;

class material :
	public resources::unmanaged_resource,
	public boost::noncopyable
{
public:
	explicit material( configs::binary_config_ptr in_config ) :
		m_config( in_config )
	{
	}

	static void initialize_nomaterial_material( );
	static bool is_nomaterial_material_ready( );
	static material_effects& nomaterial_material( enum_vertex_input_type vi );
	static void finalize_nomaterial_material( );

	pcstr get_material_name( ) const
	{
		return m_material_name.c_str( );
	}

	configs::binary_config_value const& get_config( )
	{
		return m_config->get_root( );
	}

private:
	friend struct material_cook;
	friend class material_manager;
	friend class material_effects_instance_cook;

	fixed_string< 128 >		m_material_name;
	configs::binary_config_ptr	m_config;
};

STATIC_SIZE_ASSERT( material, 0x198 );

typedef resources::resource_ptr<
	material,
	resources::unmanaged_intrusive_base
> material_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_H_INCLUDED
