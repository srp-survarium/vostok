// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SCALEFORM_RENDER_COMMAND_H_INCLUDED
#define SCALEFORM_RENDER_COMMAND_H_INCLUDED

namespace Scaleform {
namespace Render {
	class ThreadCommand;
} // namespace Render
} // namespace Scaleform

namespace survarium {

// passed BY VALUE through the render facade (execute_scaleform_command)
struct scaleform_render_command {
			void	execute	( );

public:
	/* 0x0000 */	Scaleform::Render::ThreadCommand*		thread_command;
}; // struct scaleform_render_command

STATIC_SIZE_ASSERT(scaleform_render_command, 0x4);

} // namespace survarium

#endif // #ifndef SCALEFORM_RENDER_COMMAND_H_INCLUDED
