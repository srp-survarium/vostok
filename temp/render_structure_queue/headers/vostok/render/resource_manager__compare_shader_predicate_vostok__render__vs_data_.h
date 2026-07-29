////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_COMPARE_SHADER_PREDICATE_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_COMPARE_SHADER_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_xs<vostok::render::vs_data> const* const;

/* FORWARD REFS */
class vostok::render::xs_descriptor<vostok::render::vs_data>;

namespace vostok {
namespace render {

struct resource_manager::compare_shader_predicate< vs_data > {
	inline	bool	operator()	( res_xs< vs_data > const* const arg_0, res_xs< vs_data > const* const arg_1 ) const { /* no source */ }
	inline	bool	operator()	( xs_descriptor< vs_data > const& arg_0, res_xs< vs_data > const* const arg_1 ) const { /* no source */ }
	inline	bool	operator()	( res_xs< vs_data > const* const arg_0, xs_descriptor< vs_data > const& arg_1 ) const { /* no source */ }
}; // struct resource_manager::compare_shader_predicate< vs_data >

STATIC_SIZE_ASSERT(resource_manager::compare_shader_predicate< vs_data >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_COMPARE_SHADER_PREDICATE_H_INCLUDED
