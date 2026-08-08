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

// Target signatures take the raw function pointer by value. The target emits no
// dynamic initializer/finalizer for debug's callback, so it is a POD pointer rather
// than boost::function. Top-level const on the value is invisible and is omitted.
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
