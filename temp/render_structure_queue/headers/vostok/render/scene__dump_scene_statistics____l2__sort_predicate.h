////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_DUMP_SCENE_STATISTICS___L2_SORT_PREDICATE_H_INCLUDED
#define RENDER_SCENE_DUMP_SCENE_STATISTICS___L2_SORT_PREDICATE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_model_instance_impl;

namespace vostok {
namespace render {

struct scene::dump_scene_statistics::__l2::sort_predicate {
	inline	bool	operator()	(
						render_model_instance_impl_ptr const&	arg_0,
						render_model_instance_impl_ptr const&	arg_1
					) const { /* no source */ }
}; // struct scene::dump_scene_statistics::__l2::sort_predicate

STATIC_SIZE_ASSERT(scene::dump_scene_statistics::__l2::sort_predicate, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_DUMP_SCENE_STATISTICS___L2_SORT_PREDICATE_H_INCLUDED
