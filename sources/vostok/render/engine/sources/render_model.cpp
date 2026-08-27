#include "pch.h"
#include <vostok/console_command.h>
#include <vostok/render/core/options.h>
#include "possible_sectors_holder.h"
#include <vostok/render/engine/model_format.h>
#include "material.h"
#include "material_manager.h"
#include "render_model.h"
#include "render_model_instance_impl.h"
#include "render_surface.h"
#include "render_surface_instance.h"

namespace vostok {
namespace render {

static bool s_use_one_material_value = false;
static console_commands::cc_bool s_use_one_material(
	"use_one_material",
	s_use_one_material_value,
	true,
	console_commands::command_type_user_specific

);

render_model::render_model( ) :
	m_aabbox			( math::create_identity_aabb( ) ),
	m_locators			( NULL ),
	m_locators_count	( 0 ),
	m_lods_descriptor	( NULL )
{
}

render_model::~render_model( )
{
	for( u8 i = 0; i < m_childs_count; ++i )
	{
		render_surface* child = m_childs[i];
		DELETE				( child );
	}

	if(m_locators)
		FREE(m_locators);

	FREE(m_lods_descriptor);
}

void render_model::load_properties( configs::binary_config_value const& properties )
{
	m_aabbox.max			= vostok::math::float3(properties["bounding_box"]["max"]);
	m_aabbox.min			= vostok::math::float3(properties["bounding_box"]["min"]);

	m_locators_count		= (u16)properties.value_exists("locators") ? (u16)properties["locators"].size() : 0;

	if(m_locators_count)
	{
		m_locators	= ALLOC(model_locator_item, m_locators_count);
		for(u16 i=0; i<m_locators_count; ++i)
		{
			configs::binary_config_value const& v = properties["locators"][i];
			model_locator_item& item	= m_locators[i];
			pcstr name					= v["name"];
			int bone_idx				= v["bone_idx"];
			item.m_bone					= u16(bone_idx);
			strings::copy				( item.m_name, name );
			item.m_offset				= math::create_rotation(v["rotation"]) * math::create_translation(v["position"]);
		}
	}
}

bool render_model::get_locator( pcstr locator_name, model_locator_item& result ) const
{
	for( u16 i=0; i<m_locators_count; ++i)
	{

		model_locator_item const& item = m_locators[i];
		if(strings::equal(item.m_name, locator_name))
		{
			result = item;
			return true;
		}
	}
	return false;
}

void render_model::set_children( render_surface** children_in, u8 count, model_lods_descriptor* lods )
{
	m_childs = children_in;
	m_childs_count = count;
	m_lods_descriptor = lods;

	for (u8 i = 0; i < count; ++i)
		m_aabbox.modify(m_childs[i]->m_aabbox);
}

material_effects& render_surface::get_material_effects( )
{
	if ( !m_materail_effects_instance.c_ptr( ) || s_use_one_material_value )
	{

		return material::nomaterial_material( get_vertex_input_type( ) );
	}

	return m_materail_effects_instance->get_material_effects( );

}

render_surface::~render_surface( )
{
	material_manager::ref().remove_material_effects(m_materail_effects_instance);
}

void render_surface::set_default_material( )
{
	m_materail_effects_instance = 0;
}

void render_surface::set_material_effects( material_effects_instance_ptr mtl_instance_ptr, pcstr material_name )
{
	if (mtl_instance_ptr)
	{
		material_manager::ref().remove_material_effects(m_materail_effects_instance);

		m_materail_effects_instance	= mtl_instance_ptr;

		material_manager::ref().add_material_effects(
			m_materail_effects_instance,
			material_name
		);
	}
}

void render_surface::load( configs::binary_config_value const& properties, memory::chunk_reader& chunk )
{

	m_aabbox.max			= vostok::math::float3(properties["bounding_box"]["max"]);
	m_aabbox.min			= vostok::math::float3(properties["bounding_box"]["min"]);

	float3 sphere_origin = (m_aabbox.max + m_aabbox.min) * .5f;
	float const sphere_radius =
		((m_aabbox.max - m_aabbox.min) * .5f).length();

	m_bounding_sphere = math::sphere( sphere_origin, sphere_radius );
	mesh_type_enum type = (mesh_type_enum)(u16)properties["type"];

	m_vertex_input_type = null_vertex_input_type;

	switch (type)
	{
	case mt_skinned_submesh_1w:
		m_vertex_input_type = skeletal_1_bones_mesh_vertex_input_type;
		break;
	case mt_skinned_submesh_2w:
		m_vertex_input_type = skeletal_2_bones_mesh_vertex_input_type;
		break;
	case mt_skinned_submesh_3w:
		m_vertex_input_type = skeletal_3_bones_mesh_vertex_input_type;
		break;
	case mt_skinned_submesh_4w:
		m_vertex_input_type = skeletal_4_bones_mesh_vertex_input_type;
		break;

	case mt_static_submesh:
		m_vertex_input_type = static_mesh_vertex_input_type;
		break;
	case mt_static_submesh_colored:
		m_vertex_input_type = static_mesh_vertex_colored_input_type;
		break;
	}
}

void render_surface_instance::set_constants( ) {
	m_parent->set_constants	( );
}

bool render_surface_instance::is_occluded( ) const
{
	return options::ref( ).current.m_use_hiz_occlusion_culling && m_occluded;
}

render_model_instance_impl::render_model_instance_impl( ) :
	m_collision_object( this )
{

}

} // namespace render
} // namespace vostok
