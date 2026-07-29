////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_MANAGER_COMPARE_PREDICATE_H_INCLUDED
#define RENDER_EFFECT_MANAGER_COMPARE_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::res_effect const* const;

namespace vostok {
namespace render {

struct effect_manager::compare_predicate< res_effect > {
	inline	bool	operator()	( res_effect const* const arg_0, res_effect const* const arg_1 ) const { /* no source */ }
}; // struct effect_manager::compare_predicate< res_effect >

STATIC_SIZE_ASSERT(effect_manager::compare_predicate< res_effect >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_MANAGER_COMPARE_PREDICATE_H_INCLUDED
