////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_MODEL_INSTANCE_COOK_DATA_H_INCLUDED
#define RENDER_SKELETON_MODEL_INSTANCE_COOK_DATA_H_INCLUDED

/* INCLUDES */
class vostok::resources::query_result_for_cook;
typedef vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base>
	vostok::animation::skeleton_ptr;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
class vostok::animation::skeleton;
class vostok::render::render_model_instance;

namespace vostok {
namespace render {

struct skeleton_model_instance_cook_data {
	inline	explicit	skeleton_model_instance_cook_data	( resources::query_result_for_cook* arg_0 ) { /* no source */ }
	inline				~skeleton_model_instance_cook_data	( ) { /* no source */ }

	/* 0x0000 */	bool									render_model_ready;
	/* 0x0001 */	bool									skeleton_ready;
	/* 0x0004 */	resources::query_result_for_cook*		parent_query;
	/* 0x0008 */	render_model_instance_ptr				render_model;
	/* 0x000c */	animation::skeleton_ptr					skeleton;
}; // struct skeleton_model_instance_cook_data

STATIC_SIZE_ASSERT(skeleton_model_instance_cook_data, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_MODEL_INSTANCE_COOK_DATA_H_INCLUDED
