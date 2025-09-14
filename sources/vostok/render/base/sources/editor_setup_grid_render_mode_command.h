////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef EDITOR_SETUP_GRID_RENDER_MODE_COMMAND_H_INCLUDED
#define EDITOR_SETUP_GRID_RENDER_MODE_COMMAND_H_INCLUDED

#include <vostok/render/base/engine_wrapper.h>
#include <vostok/render/base/editor_renderer.h>
#include <fastdelegate/fastdelegate.h>

namespace vostok {
namespace render {
namespace editor {

class editor_renderer;

class setup_grid_render_mode_command :
	public engine::command,
	private boost::noncopyable
{
public:
	inline	setup_grid_render_mode_command	( editor_renderer& renderer, u32 density ): 
		m_renderer		( renderer ),
		m_density		( density )
	{
	}

	virtual				~setup_grid_render_mode_command	( );
	virtual	void		execute				( );

private:
	editor_renderer&	m_renderer;
	u32 const 			m_density;
}; // struct setup_grid_render_mode_command

} // namespace editor
} // namespace render
} // namespace vostok

#endif // #ifndef EDITOR_SETUP_GRID_RENDER_MODE_COMMAND_H_INCLUDED