////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_DECALS_ACCUMULATE_EXECUTE___L4_SORT_BY_PRIORITY_PREDICATE_H_INCLUDED
#define RENDER_STAGE_DECALS_ACCUMULATE_EXECUTE___L4_SORT_BY_PRIORITY_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::decal_instance const* const;

namespace vostok {
namespace render {

struct stage_decals_accumulate::execute::__l4::sort_by_priority_predicate {
	inline	bool	operator()	( decal_instance const* const arg_0, decal_instance const* const arg_1 ) { /* no source */ }
}; // struct stage_decals_accumulate::execute::__l4::sort_by_priority_predicate

STATIC_SIZE_ASSERT(stage_decals_accumulate::execute::__l4::sort_by_priority_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_DECALS_ACCUMULATE_EXECUTE___L4_SORT_BY_PRIORITY_PREDICATE_H_INCLUDED
