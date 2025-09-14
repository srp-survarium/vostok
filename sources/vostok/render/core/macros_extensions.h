////////////////////////////////////////////////////////////////////////////
//	Created		: 28.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_MACROS_EXTENSIONS_H_INCLUDED
#define VOSTOK_RENDER_CORE_MACROS_EXTENSIONS_H_INCLUDED

#define CHECK_RESULT( expression, ... )									\
	if ( vostok::identity(true) ) {										\
		static bool ignore_always = false;								\
		if ( !ignore_always && FAILED( vostok::identity(expression) ) ) {	\
			bool do_debug_break = false;								\
			::vostok::debug::on_error	(									\
				& do_debug_break,										\
				vostok::process_error_true,								\
				&ignore_always,											\
				vostok::assert_untyped,									\
				"assertion_failed",										\
				VOSTOK_MAKE_STRING( expression ),							\
				__FILE__,												\
				__FUNCTION__,											\
				__LINE__,												\
				__VA_ARGS__												\
			);															\
			if ( do_debug_break )										\
				DEBUG_BREAK	( );										\
		}																\
	}																	\
	else (void)0

#endif // #ifndef VOSTOK_RENDER_CORE_MACROS_EXTENSIONS_H_INCLUDED