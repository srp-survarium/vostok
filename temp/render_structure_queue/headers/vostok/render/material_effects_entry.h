////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_EFFECTS_ENTRY_H_INCLUDED
#define RENDER_MATERIAL_EFFECTS_ENTRY_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
typedef vostok::resources::resource_ptr<vostok::render::material_effects_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_effects_instance_ptr;
class vostok::render::material_effects_instance;

namespace vostok {
namespace render {

struct material_effects_entry {
	inline		material_effects_entry	( material_effects_instance_ptr* arg_0, fs_new::virtual_path_string arg_1 ) { /* no source */ }

	/* 0x0000 */	material_effects_instance_ptr*		m_material_effects_instance_ptr;
	/* 0x0004 */	fs_new::virtual_path_string			m_material_name;
}; // struct material_effects_entry

STATIC_SIZE_ASSERT(material_effects_entry, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_EFFECTS_ENTRY_H_INCLUDED
