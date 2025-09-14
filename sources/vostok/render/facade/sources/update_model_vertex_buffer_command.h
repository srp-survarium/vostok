////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_UPDATE_VISUAL_VERTEX_BUFFER_H_INCLUDED
#define COMMAND_UPDATE_VISUAL_VERTEX_BUFFER_H_INCLUDED

#include <vostok/render/facade/base_command.h>
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/model.h>
#include <vostok/vectora.h>

namespace vostok {
namespace render {

class world;

class update_model_vertex_buffer_command :
	public base_command,
	private boost::noncopyable
{
public:
	typedef vectora<buffer_fragment> Fragments;
							update_model_vertex_buffer_command		(	engine::world& world, 
																		render_model_instance_ptr object, 
																		vectora<buffer_fragment> const& fragments,  
																		memory::base_allocator& allocator);
							~update_model_vertex_buffer_command	(  );
	virtual	void			execute				( );

private:
	Fragments						m_fragments;
	render_model_instance_ptr	m_object;  
	engine::world&					m_world;
	memory::base_allocator&			m_allocator;
}; // class update_model_vertex_buffer_command

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_UPDATE_VISUAL_VERTEX_BUFFER_H_INCLUDED