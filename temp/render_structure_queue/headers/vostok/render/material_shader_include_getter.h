////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_SHADER_INCLUDE_GETTER_H_INCLUDED
#define RENDER_MATERIAL_SHADER_INCLUDE_GETTER_H_INCLUDED

/* INCLUDES */
struct vostok::render::shader_include_getter;

namespace vostok {
namespace render {

class material_shader_include_getter : public shader_include_getter {
public:
	inline	explicit	material_shader_include_getter	( u32 arg_0 ) { /* no source */ }

	virtual	pcstr		get_shader_include				( pcstr arg_0 ) const override { /* no source */ }

	/* 0x0000 */	/* shader_include_getter */
private:
	/* 0x0004 */	u32		m_vertex_type;
}; // class material_shader_include_getter

STATIC_SIZE_ASSERT(material_shader_include_getter, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_SHADER_INCLUDE_GETTER_H_INCLUDED
