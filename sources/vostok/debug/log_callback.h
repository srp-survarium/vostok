////////////////////////////////////////////////////////////////////////////
//	Created		: 23.08.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_DEBUG_LOG_CALLBACK_H_INCLUDED
#define VOSTOK_DEBUG_LOG_CALLBACK_H_INCLUDED

#include <vostok/debug/api.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace vostok {
namespace debug {

// claude@MATCH: target PDB types both accessors with a RAW function pointer
// (`void (*)(pcstr,bool,bool,pcstr) set_log_callback(void (*)(pcstr,bool,bool,pcstr))`),
// and target debug.cpp emits NO dynamic initializer for s_log_callback - so the original
// typedef was a plain pointer, not boost::function.
typedef		void ( *log_callback )	  (	pcstr	initiator,
									bool	is_error_verbosity,
									bool	log_only_user_string,
									pcstr	message );

VOSTOK_DEBUG_API	log_callback	get_log_callback		( );
VOSTOK_DEBUG_API	log_callback	set_log_callback		( log_callback callback );
VOSTOK_DEBUG_API	void			disable_log_callback	( );
VOSTOK_DEBUG_API	void			enable_log_callback		( );


} // namespace debug
} // namespace vostok

#endif // #ifndef VOSTOK_DEBUG_LOG_CALLBACK_H_INCLUDED