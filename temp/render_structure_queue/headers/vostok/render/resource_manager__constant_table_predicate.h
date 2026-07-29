////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_CONSTANT_TABLE_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_CONSTANT_TABLE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_table const* const;

namespace vostok {
namespace render {

struct resource_manager::constant_table_predicate {
	inline	bool	operator()	(
						shader_constant_table const* const		arg_0,
						shader_constant_table const* const		arg_1
					) const { /* no source */ }
}; // struct resource_manager::constant_table_predicate

STATIC_SIZE_ASSERT(resource_manager::constant_table_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_CONSTANT_TABLE_PREDICATE_H_INCLUDED
