////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_GRASS_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_GRASS_INSTANCE_H_INCLUDED

namespace vostok {
namespace render {

struct grass_template;

struct grass_instance
{
						grass_instance(u32 const				in_id, 
									   grass_template*			in_tempalate, 
									   math::color const&		in_color, 
									   math::float4x4 const&	in_transform);
	grass_template*		m_template;
	math::color			m_color;
	math::float4x4		m_transform;
	u32					m_index;
}; // struct grass_instance

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_GRASS_INSTANCE_H_INCLUDED