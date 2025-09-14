////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_TERRAIN_CELL_UPDATE_BUFFER_H_INCLUDED
#define COMMAND_TERRAIN_CELL_UPDATE_BUFFER_H_INCLUDED

#include <vostok/render/facade/base_command.h>
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/model.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/editor_renderer.h>

namespace vostok {
namespace render {

class world;

class terrain_cell_update_buffer_command :
	public base_command,
	private boost::noncopyable
{
public:
	typedef render::editor::on_command_finished_functor_type	on_command_finished_functor_type;

public:
					terrain_cell_update_buffer_command	(
						render::engine::world& render_engine_world,
						scene_ptr const& in_scene,
						render_model_instance_ptr const& v, 
						vostok::vectora< terrain_buffer_fragment >& fragments,
						float4x4 const& transform,
						on_command_finished_functor_type const& on_command_finished_functor
					);
	virtual			~terrain_cell_update_buffer_command	( );
	virtual	void	execute								( );

private:
	vectora<terrain_buffer_fragment>	m_fragments;
	math::float4x4						m_transform;
	on_command_finished_functor_type	m_on_command_finished_functor;
	scene_ptr							m_scene_ptr;
	render_model_instance_ptr		m_model;
	render::engine::world&				m_render_engine_world;
}; // class terrain_cell_update_buffer_command

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_TERRAIN_CELL_UPDATE_BUFFER_H_INCLUDED
