// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_OBJECT_STATIC_H_INCLUDED
#define GAME_OBJECT_STATIC_H_INCLUDED

#include "game_object_.h"

namespace survarium {

class game_object_static : public game_object_ {
	typedef game_object_ super;

public:
			explicit	game_object_static	( base_game_scene& s );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

protected:
	/* 0x0000 */	/* game_object_ */
	/* 0x0110 */	float4x4	m_transform;
}; // class game_object_static

STATIC_SIZE_ASSERT(game_object_static, 0x150);

} // namespace survarium

#endif // #ifndef GAME_OBJECT_STATIC_H_INCLUDED
