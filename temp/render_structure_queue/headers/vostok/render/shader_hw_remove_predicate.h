////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_HW_REMOVE_PREDICATE_H_INCLUDED
#define RENDER_SHADER_HW_REMOVE_PREDICATE_H_INCLUDED

namespace vostok {
namespace render {

struct shader_hw_remove_predicate {
	inline	explicit	shader_hw_remove_predicate	( pcstr arg_0 ) { /* no source */ }

private:
	/* 0x0000 */	pcstr		m_name;
}; // struct shader_hw_remove_predicate

STATIC_SIZE_ASSERT(shader_hw_remove_predicate, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_HW_REMOVE_PREDICATE_H_INCLUDED
