////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_SHADER_BASE_H_INCLUDED
#define RENDER_MATERIAL_SHADER_BASE_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_material_base;
class vostok::render::enum_vertex_input_type;
typedef vostok::resources::resource_ptr<vostok::render::material,vostok::resources::unmanaged_intrusive_base>
	vostok::render::material_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::material;
class vostok::render::res_effect;

namespace vostok {
namespace render {

class material_shader_base : public effect_material_base {
	inline		material_shader_base	( enum_vertex_input_type arg_0, material_ptr arg_1 ) { /* no source */ }
public:
	virtual		~material_shader_base	( ) { /* no source */ }

	/* 0x0000 */	/* effect_material_base */
private:
	/* 0x0004 */	u32					m_vertex_type;
	/* 0x0008 */	material_ptr		m_mtl;
	/* 0x000c */	res_effect_ptr		m_shader;
}; // class material_shader_base

STATIC_SIZE_ASSERT(material_shader_base, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_SHADER_BASE_H_INCLUDED
