////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BASE_SCENE_VIEW_H_INCLUDED
#define RENDER_BASE_SCENE_VIEW_H_INCLUDED

/* INCLUDES */
class vostok::render::base_command;
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::base_scene_view,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_view_ptr;
class vostok::render::base_scene_view;

namespace vostok {
namespace render {

struct base_scene_view : public resources::unmanaged_resource {
	inline		base_scene_view	( ) { /* no source */ }
	virtual		~base_scene_view( ) { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	base_command*			first_command;
	/* 0x010c */	base_command*			last_command;
	/* 0x0110 */	base_scene_view_ptr		next_scene_view;
	/* 0x0114 */	volatile long			editor_debug_mode;
}; // struct base_scene_view

STATIC_SIZE_ASSERT(base_scene_view, 0x118);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BASE_SCENE_VIEW_H_INCLUDED
