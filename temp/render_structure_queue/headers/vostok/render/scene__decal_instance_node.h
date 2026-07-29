////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_DECAL_INSTANCE_NODE_H_INCLUDED
#define RENDER_SCENE_DECAL_INSTANCE_NODE_H_INCLUDED

/* INCLUDES */
struct vostok::render::scene::decal_instance_node;
typedef vostok::intrusive_ptr<vostok::render::decal_instance,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::decal_instance_ptr;
class vostok::render::decal_instance;

namespace vostok {
namespace render {

struct scene::decal_instance_node {
	inline	explicit	decal_instance_node	( decal_instance* arg_0 ) { /* no source */ }
	inline				~decal_instance_node( ) { /* no source */ }

	/* 0x0000 */	decal_instance_ptr				decal;
	/* 0x0004 */	scene::decal_instance_node*		next;
}; // struct scene::decal_instance_node

STATIC_SIZE_ASSERT(scene::decal_instance_node, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_DECAL_INSTANCE_NODE_H_INCLUDED
