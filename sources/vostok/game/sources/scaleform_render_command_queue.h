////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SCALEFORM_RENDER_COMMAND_QUEUE_H_INCLUDED
#define SCALEFORM_RENDER_COMMAND_QUEUE_H_INCLUDED

namespace survarium {

class scaleform_game_engine;
class scaleform_render_command_queue_impl;

// ctor/dtor are addressed in the original vostok/scaleform/sources/
// command_queue.cpp (module absent from our tree); inline stubs until then
class scaleform_render_command_queue {
public:
	inline	explicit	scaleform_render_command_queue	( scaleform_game_engine& arg_0 ) { /* no source */ }
	inline				~scaleform_render_command_queue	( ) { /* no source */ }

private:
	/* 0x0000 */	scaleform_render_command_queue_impl*	impl;
}; // class scaleform_render_command_queue

STATIC_SIZE_ASSERT(scaleform_render_command_queue, 0x4);

} // namespace survarium

#endif // #ifndef SCALEFORM_RENDER_COMMAND_QUEUE_H_INCLUDED
