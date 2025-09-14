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

typedef		boost::function< void (	pcstr	initiator, 
									bool	is_error_verbosity, 
									bool	log_only_user_string, 
									pcstr	message )	>		log_callback;

VOSTOK_DEBUG_API	log_callback	get_log_callback		( );
VOSTOK_DEBUG_API	log_callback	set_log_callback		( log_callback const & callback );
VOSTOK_DEBUG_API	void			disable_log_callback	( );
VOSTOK_DEBUG_API	void			enable_log_callback		( );


} // namespace debug
} // namespace vostok

#endif // #ifndef VOSTOK_DEBUG_LOG_CALLBACK_H_INCLUDED