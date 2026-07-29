////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_CONSTANT_BUFFER_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_CONSTANT_BUFFER_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_buffer const* const;

namespace vostok {
namespace render {

struct resource_manager::constant_buffer_predicate {
	bool	operator()	(
				shader_constant_buffer const* const		left,
				shader_constant_buffer const* const		right
			) const;
}; // struct resource_manager::constant_buffer_predicate

STATIC_SIZE_ASSERT(resource_manager::constant_buffer_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_CONSTANT_BUFFER_PREDICATE_H_INCLUDED
