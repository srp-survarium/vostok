////////////////////////////////////////////////////////////////////////////
//	Created		: 16.03.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_BASE_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_BASE_COMMAND_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace render {

class VOSTOK_NOVTABLE base_command {
public:
	inline			base_command		( bool const is_deferred = false, bool const depth = true ) :
		remove_frame_id		( 0 ),
		is_deferred_command	( is_deferred ),
		use_depth			( depth )
	{
	}

	virtual	void	execute				( ) = 0;
	virtual	void	defer_execution		( ) { R_ASSERT( !is_deferred_command ); }
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( base_command )

public:
	/* 0x0004 */	base_command*	next;
	/* 0x0008 */	base_command*	deferred_next;
	/* 0x000c */	bool			is_deferred_command;
	/* 0x000d */	bool			use_depth;

public:
	/* 0x000e */	VOSTOK_MAX_CACHE_LINE_PAD;
	/* 0x0050 */	u32				remove_frame_id;
}; // class base_command

STATIC_SIZE_ASSERT( base_command, 0x54 );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_BASE_COMMAND_H_INCLUDED
