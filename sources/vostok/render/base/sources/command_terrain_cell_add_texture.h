////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_TERRAIN_CELL_ADD_TEXTURE_H_INCLUDED
#define COMMAND_TERRAIN_CELL_ADD_TEXTURE_H_INCLUDED

#include <vostok/render/base/engine_wrapper.h>
#include <vostok/render/base/common_types.h>
#include <vostok/render/base/visual.h>

namespace vostok {
namespace render {

class base_world;

class command_terrain_cell_add_texture :
	public engine::command,
	private boost::noncopyable
{
public:
	command_terrain_cell_add_texture( base_world& world, visual_ptr v, texture_string const& texture, u32 tex_user_id);
	virtual					~command_terrain_cell_add_texture	(  );
	virtual	void			execute				( );

private:
	base_world&						m_world;
	texture_string const			m_texture;
	visual_ptr						m_visual;
	u32								m_tex_user_id;

}; // class command_terrain_cell_add_texture

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_TERRAIN_CELL_ADD_TEXTURE_H_INCLUDED