////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STAGE_LIGHTS_EXECUTE___L15_SORT_BY_SIZE_PREDICATE_H_INCLUDED
#define RENDER_STAGE_LIGHTS_EXECUTE___L15_SORT_BY_SIZE_PREDICATE_H_INCLUDED

/* FORWARD REFS */
class vostok::render::environment_probe;

namespace vostok {
namespace render {

struct stage_lights::execute::__l15::sort_by_size_predicate {
	inline	bool	operator()	( environment_probe const* arg_0, environment_probe const* arg_1 ) const { /* no source */ }
}; // struct stage_lights::execute::__l15::sort_by_size_predicate

STATIC_SIZE_ASSERT(stage_lights::execute::__l15::sort_by_size_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STAGE_LIGHTS_EXECUTE___L15_SORT_BY_SIZE_PREDICATE_H_INCLUDED
