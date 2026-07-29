////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED
#define RENDER_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
class vostok::resources::unmanaged_resource;
struct vostok::render::material_effects;

namespace vostok {
namespace render {

struct material_effects_instance : public resources::unmanaged_resource {
	inline	material_effects&						get_material_effects		( ) { /* no source */ }
	inline	void									set_material_effects		( material_effects const& arg_0 ) { /* no source */ }

	inline	fs_new::virtual_path_string const&		get_material_name			( ) const { /* no source */ }

	inline											material_effects_instance	( ) { /* no source */ }
	virtual											~material_effects_instance	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
private:
	/* 0x0108 */	material_effects				m_material_effects;
	/* 0x0498 */	fs_new::virtual_path_string		m_material_name;
}; // struct material_effects_instance

STATIC_SIZE_ASSERT(material_effects_instance, 0x5B0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED
