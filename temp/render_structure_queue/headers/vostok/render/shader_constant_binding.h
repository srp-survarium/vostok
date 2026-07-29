////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_BINDING_H_INCLUDED
#define RENDER_SHADER_CONSTANT_BINDING_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_source;
class vostok::shared_string;
class vostok::render::enum_constant_class;
class vostok::render::enum_constant_type;

namespace vostok {
namespace render {

enum vostok::render::enum_constant_type
{
	rc_float		= 0x0000,
	rc_int			= 0x0001,
	rc_bool			= 0x0002,
	rc_INVALID		= 0xffff,
};
enum vostok::render::enum_constant_class
{
	rc_1x1		= 0x004,
	rc_1x2		= 0x008,
	rc_1x3		= 0x00c,
	rc_1x4		= 0x010,
	rc_2x4		= 0x120,
	rc_3x4		= 0x130,
	rc_4x4		= 0x140,
	rc_1x4a		= 0x210,
	rc_3x4a		= 0x230,
	rc_4x4a		= 0x240,
};

class shader_constant_binding {
public:
	inline	shared_string const&				name					( ) const { /* no source */ }

	inline	shader_constant_source const&		source					( ) const { /* no source */ }

	inline	enum_constant_class					class_id				( ) const { /* no source */ }

	inline	enum_constant_type					type					( ) const { /* no source */ }

	inline										~shader_constant_binding( ) { /* no source */ }

private:
	/* 0x0000 */	shader_constant_source		m_source;
	/* 0x0008 */	shared_string				m_name;
	/* 0x000c */	enum_constant_type			m_type;
	/* 0x0010 */	enum_constant_class			m_class_id;
}; // class shader_constant_binding

STATIC_SIZE_ASSERT(shader_constant_binding, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_BINDING_H_INCLUDED
