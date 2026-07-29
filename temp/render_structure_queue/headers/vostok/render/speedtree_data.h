////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_DATA_H_INCLUDED
#define RENDER_SPEEDTREE_DATA_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
class vostok::resources::query_result_for_cook;
typedef vostok::resources::resource_ptr<vostok::render::material,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_ptr;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::render::speedtree_tree_base,vostok::resources::unmanaged_intrusive_base>
	vostok::render::speedtree_tree_base_ptr;
class vostok::render::material;
class vostok::render::material_effects_instance;
class vostok::render::speedtree_tree_base;

namespace vostok {
namespace render {

struct speedtree_data {
	inline	bool	all_assests_ready	( ) { /* no source */ }

	inline			speedtree_data		( ) { /* no source */ }
	inline			~speedtree_data		( ) { /* no source */ }

	/* 0x0000 */	resources::query_result_for_cook*		m_parent_query;
	/* 0x0004 */	material_ptr							m_materials[5];
	/* 0x0018 */	material_effects_instance_ptr			m_material_effects[5];
	/* 0x002c */	fs_new::virtual_path_string				m_material_path[5];
	/* 0x0590 */	speedtree_tree_base_ptr					m_model;
	/* 0x0594 */	bool									m_failed;
}; // struct speedtree_data

STATIC_SIZE_ASSERT(speedtree_data, 0x598);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_DATA_H_INCLUDED
