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

class base_command;

struct base_scene : public resources::unmanaged_resource {
	inline	base_scene	( ) :
		first_command	( 0 ),
		last_command	( 0 )
	{
	}

	typedef	resources::resource_ptr <
		base_scene,
		resources::unmanaged_intrusive_base
	> scene_ptr;

	base_command*		first_command;
	base_command*		last_command;
	scene_ptr			next_scene;
}; // struct base_scene

typedef base_scene* scene_pointer;

struct base_scene_view : public resources::unmanaged_resource {
	inline	base_scene_view	( ) :
		first_command	( 0 ),
		last_command	( 0 )
	{
	}

	typedef	resources::resource_ptr <
		base_scene_view,
		resources::unmanaged_intrusive_base
	> scene_view_ptr;

	base_command*		first_command;
	base_command*		last_command;
	scene_view_ptr		next_scene_view;
}; // struct base_scene_view

typedef base_scene_view* scene_view_pointer;

struct base_output_window : public resources::unmanaged_resource { };

typedef	base_scene::scene_ptr			scene_ptr;
typedef	base_scene_view::scene_view_ptr	scene_view_ptr;

typedef	resources::resource_ptr <
	base_output_window,
	resources::unmanaged_intrusive_base
> render_output_window_ptr;

typedef base_output_window* render_output_window_pointer;

} // namespace render
} // namespace vostok

template class VOSTOK_RENDER_ENGINE_API
	vostok::resources::resource_ptr <
		vostok::render::base_scene,
		vostok::resources::unmanaged_intrusive_base
	>;

template class VOSTOK_RENDER_ENGINE_API
	vostok::resources::resource_ptr <
		vostok::render::base_scene_view,
		vostok::resources::unmanaged_intrusive_base
	>;

template class VOSTOK_RENDER_ENGINE_API
	vostok::resources::resource_ptr <
		vostok::render::base_output_window,
		vostok::resources::unmanaged_intrusive_base
	>;

#endif // #ifndef VOSTOK_RENDER_ENGINE_BASE_CLASSES_H_INCLUDED