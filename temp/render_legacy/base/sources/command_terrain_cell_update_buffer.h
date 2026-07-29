////////////////////////////////////////////////////////////////////////////
//	Created		: 11.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_TERRAIN_CELL_UPDATE_BUFFER_H_INCLUDED
#define COMMAND_TERRAIN_CELL_UPDATE_BUFFER_H_INCLUDED


#include <vostok/render/base/engine_wrapper.h>
#include <vostok/render/base/common_types.h>
#include <vostok/render/base/command_common.h>
#include <vostok/render/base/visual.h>

namespace vostok {
namespace render {

class base_world;

class command_terrain_cell_update_buffer :
	public engine::command,
	private boost::noncopyable
{
public:
	typedef vectora<buffer_fragment> Fragments;
							command_terrain_cell_update_buffer(		base_world& world, 
																	visual_ptr v, 
																	vostok::vectora<buffer_fragment_NEW> & fragments,
																	float4x4 const& transform,
																	command_finished_callback const& finish_callback,
																	u32 finish_callback_argument);
	virtual					~command_terrain_cell_update_buffer	(  );
	virtual	void			execute				( );

private:
	base_world&									m_world;
	visual_ptr									m_visual;
	vostok::vectora<buffer_fragment_NEW>			m_fragments;
	math::float4x4								m_transform;
	command_finished_callback					m_finish_callback;
	u32											m_finish_callback_argument;

}; // class command_terrain_cell_update_buffer

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_TERRAIN_CELL_UPDATE_BUFFER_H_INCLUDED