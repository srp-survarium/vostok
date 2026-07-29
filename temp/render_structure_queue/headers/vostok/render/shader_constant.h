////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_H_INCLUDED
#define RENDER_SHADER_CONSTANT_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::shader_constant_slot;
class vostok::render::shader_constant_source;

namespace vostok {
namespace render {

class shader_constant {
public:
	inline	explicit							shader_constant	( shader_constant_host const& arg_0 ) { /* no source */ }

	inline	void								set_source		( shader_constant_source const& arg_0 ) { /* no source */ }

	inline	shader_constant_slot const&			slot			( ) const { /* no source */ }

	inline	shader_constant_source const&		source			( ) const { /* no source */ }

	inline	shader_constant_host const&			host			( ) const { /* no source */ }

	inline	shader_constant_host&				get_host		( ) const { /* no source */ }

	/* 0x0000 */	shader_constant_slot			m_slot;
	/* 0x0008 */	shader_constant_source			m_source;
	/* 0x0010 */	shader_constant_host const&		m_host;
}; // class shader_constant

STATIC_SIZE_ASSERT(shader_constant, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_H_INCLUDED
