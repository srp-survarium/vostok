////////////////////////////////////////////////////////////////////////////
//	Created		: 22.02.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef COMMAND_ADD_LIGHT_H_INCLUDED
#define COMMAND_ADD_LIGHT_H_INCLUDED

#include <vostok/render/base/engine_wrapper.h>
#include <vostok/render/base/light_props.h>
#include <vostok/render/base/visual.h>

namespace vostok {
namespace render {

class light_props;

class base_world;

class command_add_light :
	public engine::command,
	private boost::noncopyable
{
public:
					command_add_light	( base_world& world, u32 id, light_props const& props, bool beditor );
	virtual			~command_add_light	(  );
	virtual	void	execute				( );

private:
	vostok::render::visual_ptr	m_object;
	u32							m_id;
	base_world&					m_world;
	light_props					m_props;
	bool						m_beditor;
}; // class command_add_light

} // namespace render
} // namespace vostok

#endif // #ifndef COMMAND_ADD_LIGHT_H_INCLUDED