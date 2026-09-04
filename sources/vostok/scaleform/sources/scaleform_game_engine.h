// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SCALEFORM_GAME_ENGINE_H_INCLUDED
#define SCALEFORM_GAME_ENGINE_H_INCLUDED

// execute_scaleform_command takes the command by value
#include "scaleform_render_command.h"

namespace survarium {

class VOSTOK_NOVTABLE scaleform_game_engine {
public:
	virtual	void	execute_scaleform_command	( scaleform_render_command arg_0 ) = 0;

}; // class scaleform_game_engine

STATIC_SIZE_ASSERT(scaleform_game_engine, 0x4);

} // namespace survarium

#endif // #ifndef SCALEFORM_GAME_ENGINE_H_INCLUDED
