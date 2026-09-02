// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_LIGHT_H_INCLUDED
#define OBJECT_LIGHT_H_INCLUDED

#include "game_object_static.h"
#include <vostok/render/facade/light_props.h>

namespace survarium {

class object_light : public game_object_static {
	typedef game_object_static super;

public:
			explicit				object_light	( base_game_scene& w );
	virtual							~object_light	( );

	virtual	void					load			(
										configs::binary_config_value const&		t,
										pcstr									__formal,
										boost::function< void( game_object_& ) >&	cb
									) override;

	inline	void					update_props	( ) { /* no source */ }

	inline	render::light_props&	props			( ) { /* no source */ return m_props; }

	virtual	void					insert			( ) override;
	virtual	void					remove			( ) override;

protected:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	render::light_props		m_props;
	/* 0x0240 */	u32						m_light_id;
}; // class object_light

STATIC_SIZE_ASSERT(object_light, 0x248);

} // namespace survarium

#endif // #ifndef OBJECT_LIGHT_H_INCLUDED
