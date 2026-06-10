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

// claude@MATCH: re-verified against the target PDB structure (sushi review, PR #286):
//   ?set_log_callback@debug@vostok@@YAP6AXPBD_N10@ZP6AX0110@Z@Z
//   ?get_log_callback@debug@vostok@@YAP6AXPBD_N10@ZXZ
// the parameter mangles P6AX0110@Z = raw function pointer BY VALUE (a boost::function
// const& would mangle ABV?$function@..., a `log_callback const&` would mangle ABQ6...),
// and the only s_log_callback dynamic initializer/atexit dtor pair in the whole target
// belongs to vostok::logging (its boost::function one) - debug.cpp emits none, so its
// s_log_callback is a POD pointer. Top-level const on the by-value parameter would not
// mangle (codegen- and PDB-invisible), so its presence is unknowable; spelled without it.
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