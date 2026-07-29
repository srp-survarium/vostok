////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BASE_COMMAND_H_INCLUDED
#define RENDER_BASE_COMMAND_H_INCLUDED

/* INCLUDES */
class vostok::render::base_command;

namespace vostok {
namespace render {

class base_command {
public:
	inline			base_command	( const bool arg_0, const bool arg_1 ) { /* no source */ }

	virtual	void	execute			( ) = 0;

	virtual	void	defer_execution	( ) { /* no source */ }

	virtual			~base_command	( ) = 0;

	/* 0x0004 */	base_command*		next;
	/* 0x0008 */	base_command*		deferred_next;
	/* 0x000c */	bool				is_deferred_command;
	/* 0x000d */	bool				use_depth;
	/* 0x000e */	char				m_cache_line_pad_$29[64];
	/* 0x0050 */	u32					remove_frame_id;
}; // class base_command

STATIC_SIZE_ASSERT(base_command, 0x54);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BASE_COMMAND_H_INCLUDED
