////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_TERRAIN_ADD_CELL_H_INCLUDED
#define COMMAND_TERRAIN_ADD_CELL_H_INCLUDED

#include <vostok/render/base/engine_wrapper.h>
#include <vostok/render/base/common_types.h>
#include <vostok/render/base/command_common.h>
#include <vostok/render/base/visual.h>

namespace vostok {
namespace render {

class base_world;

class command_terrain_add_cell :
	public engine::command,
	private boost::noncopyable
{
public:
	typedef vectora<buffer_fragment> Fragments;
							command_terrain_add_cell	( base_world& world, visual_ptr v, bool beditor );

	virtual					~command_terrain_add_cell	( );
	virtual	void			execute						( );

private:

	base_world&							m_world;
	visual_ptr							m_visual;
	bool								m_beditor;
}; // class command_terrain_add_cell

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_TERRAIN_ADD_CELL_H_INCLUDED