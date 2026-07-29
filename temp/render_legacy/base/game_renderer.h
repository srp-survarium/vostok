////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_GAME_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_BASE_GAME_RENDERER_H_INCLUDED

#include <vostok/resources.h>
#include <vostok/render/base/common_types.h>
#include <vostok/render/base/light_props.h>
#include <vostok/render/base/visual.h>

namespace vostok {
namespace render {

namespace engine	{	struct command;		}
namespace debug		{	struct renderer;	}

namespace game {

struct VOSTOK_NOVTABLE renderer {
	virtual	debug::renderer& debug							( ) = 0;
	virtual	void			add_static						( resources::managed_resource_ptr const& collision, resources::managed_resource_ptr const& geometry, resources::managed_resource_ptr const& visuals ) = 0;
	virtual	void			draw_static						( ) = 0;
	virtual	void			draw_frame						( ) = 0;
//	virtual	void			draw_visual						( visual_ptr v, math::float4x4 const& transform, bool selected = false ) = 0;

	virtual void			add_visual						( u32 id, vostok::render::visual_ptr v, vostok::math::float4x4 const& transform, bool selected = false, bool system_object = false ) = 0;
	virtual void			update_visual					( u32 id, vostok::math::float4x4 const& transform, bool selected = false ) = 0;
	virtual void			remove_visual					( u32 id ) = 0;
	virtual void			render_visuals					( ) = 0;

	virtual void			add_light						( u32 id, vostok::render::light_props const& props) = 0;
	virtual void			update_light					( u32 id, vostok::render::light_props const& props) = 0;
	virtual void			remove_light					( u32 id) = 0;

	virtual	void			terrain_add_cell				( visual_ptr v ) = 0;
	virtual	void			terrain_remove_cell				( visual_ptr v ) = 0;

	virtual void			update_visual_vertex_buffer		( vostok::render::visual_ptr const& object, vectora<buffer_fragment> const& fragments ) = 0;
	virtual void			change_visual_shader			( vostok::render::visual_ptr const& object, char const* shader, char const* texture ) = 0;

	virtual void			set_command_push_thread_id		( ) = 0;
	virtual void			set_command_processor_frame_id	( u32 frame_id ) = 0;
	virtual	void			initialize_command_queue		( engine::command* null_command ) = 0;

	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( renderer )
}; // class world

} // namespace game
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_GAME_RENDERER_H_INCLUDED