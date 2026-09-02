// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

namespace survarium {

class bullet_manager;

struct engine {
	virtual	bullet_manager&		get_bullet_manager	( ) const = 0;

}; // struct engine

STATIC_SIZE_ASSERT(engine, 0x4);

} // namespace survarium

#endif // #ifndef ENGINE_H_INCLUDED
