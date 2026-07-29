////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ONE_WAY_RENDER_CHANNEL_NULL_RENDER_COMMAND_H_INCLUDED
#define RENDER_ONE_WAY_RENDER_CHANNEL_NULL_RENDER_COMMAND_H_INCLUDED

/* INCLUDES */
class vostok::render::base_command;

namespace vostok {
namespace render {

struct one_way_render_channel::null_render_command : public base_command {
	virtual	void	execute				( ) override { /* no source */ }

	inline			null_render_command	( ) { /* no source */ }
	virtual			~null_render_command( ) { /* no source */ }
}; // struct one_way_render_channel::null_render_command

STATIC_SIZE_ASSERT(one_way_render_channel::null_render_command, 0x54);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ONE_WAY_RENDER_CHANNEL_NULL_RENDER_COMMAND_H_INCLUDED
