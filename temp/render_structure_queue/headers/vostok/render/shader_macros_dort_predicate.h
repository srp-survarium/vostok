////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_MACROS_DORT_PREDICATE_H_INCLUDED
#define RENDER_SHADER_MACROS_DORT_PREDICATE_H_INCLUDED

namespace vostok {
namespace render {

struct shader_macros_dort_predicate {
	inline	bool	operator()	( pcstr arg_0, pcstr arg_1 ) { /* no source */ }
}; // struct shader_macros_dort_predicate

STATIC_SIZE_ASSERT(shader_macros_dort_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_MACROS_DORT_PREDICATE_H_INCLUDED
