////////////////////////////////////////////////////////////////////////////
//	Created		: 28.01.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_UPDATE_VISUAL_VERTEX_BUFFER_H_INCLUDED
#define COMMAND_UPDATE_VISUAL_VERTEX_BUFFER_H_INCLUDED

#include <vostok/render/base/engine_wrapper.h>
#include <vostok/render/base/common_types.h>
#include <vostok/render/base/visual.h>
#include <vostok/vectora.h>

namespace vostok {
namespace render {

class base_world;

class command_update_visual_vertex_buffer :
	public engine::command,
	private boost::noncopyable
{
public:
	typedef vectora<buffer_fragment> Fragments;
							command_update_visual_vertex_buffer( base_world& world, vostok::render::visual_ptr const& object, vectora<buffer_fragment> const& fragments,  memory::base_allocator& allocator);
							~command_update_visual_vertex_buffer	(  );
	virtual	void			execute				( );

private:
	base_world&						m_world;
	vostok::render::visual_ptr const& m_object;  
	memory::base_allocator&				m_allocator;
	Fragments							m_fragments;

}; // class command_update_visual_vertex_buffer

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_UPDATE_VISUAL_VERTEX_BUFFER_H_INCLUDED