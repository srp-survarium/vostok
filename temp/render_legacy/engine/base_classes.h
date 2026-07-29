////////////////////////////////////////////////////////////////////////////
//	Created		: 17.06.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_BASE_CLASSES_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_BASE_CLASSES_H_INCLUDED

#include <vostok/resources_resource_ptr.h>
#include <vostok/resources_intrusive_base.h>
#include <vostok/render/engine/api.h>

namespace vostok {
namespace render {

namespace debug { class renderer; }

class base_command;
struct base_scene;
struct base_scene_view;
struct base_output_window;

typedef resources::resource_ptr<
	base_scene,
	resources::unmanaged_intrusive_base
> base_scene_ptr;

typedef resources::resource_ptr<
	base_scene_view,
	resources::unmanaged_intrusive_base
> base_scene_view_ptr;

struct base_scene : public resources::unmanaged_resource {
	inline	base_scene	( ) :
		first_command	( 0 ),
		last_command	( 0 ),
		fixed_lod_value	( 0 )
	{
	}

	virtual	~base_scene	( ) { }

	/* 0x0108 */	base_command*		first_command;
	/* 0x010c */	base_command*		last_command;
	/* 0x0110 */	base_scene_ptr		next_scene;
	/* 0x0114 */	volatile long		fixed_lod_value;
}; // struct base_scene

STATIC_SIZE_ASSERT( base_scene, 0x118 );

typedef base_scene* base_scene_pointer;

struct base_scene_view : public resources::unmanaged_resource {
	inline	base_scene_view	( ) :
		first_command		( 0 ),
		last_command		( 0 ),
		editor_debug_mode	( 0 )
	{
	}

	virtual	~base_scene_view	( ) { }

	/* 0x0108 */	base_command*			first_command;
	/* 0x010c */	base_command*			last_command;
	/* 0x0110 */	base_scene_view_ptr		next_scene_view;
	/* 0x0114 */	volatile long			editor_debug_mode;
}; // struct base_scene_view

STATIC_SIZE_ASSERT( base_scene_view, 0x118 );

typedef base_scene_view* base_scene_view_pointer;

struct base_output_window : public resources::unmanaged_resource {
	inline	math::uint2 const&	current_size		( ) const { return m_current_size; }

	inline						base_output_window	( ) { }
	virtual						~base_output_window	( ) { }

protected:
	/* 0x0108 */	math::uint2		m_current_size;
}; // struct base_output_window

STATIC_SIZE_ASSERT( base_output_window, 0x110 );

typedef	base_scene_ptr			scene_ptr;
typedef	base_scene_view_ptr		scene_view_ptr;

typedef resources::resource_ptr<
	base_output_window,
	resources::unmanaged_intrusive_base
> base_output_window_ptr;

typedef base_output_window_ptr render_output_window_ptr;
typedef base_output_window* render_output_window_pointer;

} // namespace render
} // namespace vostok

template class VOSTOK_RENDER_ENGINE_API
	vostok::resources::resource_ptr<
		vostok::render::base_scene,
		vostok::resources::unmanaged_intrusive_base
	>;

template class VOSTOK_RENDER_ENGINE_API
	vostok::resources::resource_ptr<
		vostok::render::base_scene_view,
		vostok::resources::unmanaged_intrusive_base
	>;

template class VOSTOK_RENDER_ENGINE_API
	vostok::resources::resource_ptr<
		vostok::render::base_output_window,
		vostok::resources::unmanaged_intrusive_base
	>;

#endif // VOSTOK_RENDER_ENGINE_BASE_CLASSES_H_INCLUDED
