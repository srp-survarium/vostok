#ifndef VOSTOK_RENDER_FACADE_BASE_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_BASE_COMMAND_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace render {

class VOSTOK_NOVTABLE base_command {
public:
	base_command( bool const is_deferred = false, bool const depth = true )
		: remove_frame_id( 0 ),
		  is_deferred_command( is_deferred ),
		  use_depth( depth )
	{
	}

	virtual void execute( ) = 0;
	virtual void defer_execution( ) { R_ASSERT( !is_deferred_command ); }
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( base_command )

public:
	base_command* next;
	base_command* deferred_next;
	bool is_deferred_command;
	bool use_depth;
	VOSTOK_MAX_CACHE_LINE_PAD;
	u32 remove_frame_id;
};

STATIC_SIZE_ASSERT( base_command, 0x54 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_BASE_COMMAND_H_INCLUDED
