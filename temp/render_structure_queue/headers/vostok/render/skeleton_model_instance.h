////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SKELETON_MODEL_INSTANCE_H_INCLUDED
#define RENDER_SKELETON_MODEL_INSTANCE_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base>
	vostok::animation::skeleton_ptr;
typedef vostok::resources::resource_ptr<vostok::render::render_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::render_model_instance_ptr;
class vostok::animation::skeleton;
class vostok::render::render_model_instance;

namespace vostok {
namespace render {

class skeleton_model_instance : public resources::unmanaged_resource {
public:
	inline	void	get_bind_pose			( float4x4* arg_0, u32 arg_1 ) const { /* no source */ }

	inline			skeleton_model_instance	( ) { /* no source */ }
	virtual			~skeleton_model_instance( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	render_model_instance_ptr		m_render_model;
	/* 0x010c */	animation::skeleton_ptr			m_skeleton;
}; // class skeleton_model_instance

STATIC_SIZE_ASSERT(skeleton_model_instance, 0x110);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SKELETON_MODEL_INSTANCE_H_INCLUDED
