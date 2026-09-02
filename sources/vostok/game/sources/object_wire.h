// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_WIRE_H_INCLUDED
#define OBJECT_WIRE_H_INCLUDED

#include "game_object_static.h"
// render_model_instance_ptr
#include <vostok/render/facade/model.h>

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class object_wire : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_wire		( base_game_scene& w );
	virtual				~object_wire	( );

	virtual	void		load			(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert			( ) override;
	virtual	void		remove			( ) override;

protected:
			void		resources_ready	( resources::queries_result& data, boost::function< void( game_object_& ) >& cb );

protected:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	float3*									m_points;
	/* 0x0154 */	u32										m_points_count;
	/* 0x0158 */	float									m_wire_width;
	/* 0x015c */	render::render_model_instance_ptr		m_visual;
}; // class object_wire

STATIC_SIZE_ASSERT(object_wire, 0x160);

} // namespace survarium

#endif // #ifndef OBJECT_WIRE_H_INCLUDED
