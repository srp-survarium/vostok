// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SCALEFORM_RENDER_COMMAND_QUEUE_H_INCLUDED
#define SCALEFORM_RENDER_COMMAND_QUEUE_H_INCLUDED

namespace survarium {

class scaleform_game_engine;
class scaleform_render_command_queue_impl;
class flash_factory;
struct flash_renderer;

class scaleform_render_command_queue {
public:
			explicit	scaleform_render_command_queue	( scaleform_game_engine& arg_0 );
						~scaleform_render_command_queue	( );

public:
	friend class flash_factory;
	friend struct flash_renderer;

	/* 0x0000 */	scaleform_render_command_queue_impl*	impl;
}; // class scaleform_render_command_queue

STATIC_SIZE_ASSERT(scaleform_render_command_queue, 0x4);

} // namespace survarium

#endif // #ifndef SCALEFORM_RENDER_COMMAND_QUEUE_H_INCLUDED
