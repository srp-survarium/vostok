////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SCALEFORM_RENDER_COMMAND_H_INCLUDED
#define SCALEFORM_RENDER_COMMAND_H_INCLUDED

/* INCLUDES */
class Scaleform::Render::ThreadCommand;

namespace survarium {

struct scaleform_render_command {
	inline	void	execute	( ) { /* no source */ }

public:
	/* 0x0000 */	Scaleform::Render::ThreadCommand*		thread_command;
}; // struct scaleform_render_command

STATIC_SIZE_ASSERT(scaleform_render_command, 0x4);

} // namespace survarium

#endif // #ifndef SCALEFORM_RENDER_COMMAND_H_INCLUDED
