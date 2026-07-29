////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BASE_SCENE_H_INCLUDED
#define RENDER_BASE_SCENE_H_INCLUDED

/* INCLUDES */
class vostok::render::base_command;
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_ptr;
class vostok::render::base_scene;

namespace vostok {
namespace render {

struct base_scene : public resources::unmanaged_resource {
	inline		base_scene	( ) { /* no source */ }
	virtual		~base_scene	( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	base_command*		first_command;
	/* 0x010c */	base_command*		last_command;
	/* 0x0110 */	base_scene_ptr		next_scene;
	/* 0x0114 */	volatile long		fixed_lod_value;
}; // struct base_scene

STATIC_SIZE_ASSERT(base_scene, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BASE_SCENE_H_INCLUDED
