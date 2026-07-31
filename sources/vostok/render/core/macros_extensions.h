////////////////////////////////////////////////////////////////////////////
//	Created		: 28.12.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CORE_MACROS_EXTENSIONS_H_INCLUDED
#define VOSTOK_RENDER_CORE_MACROS_EXTENSIONS_H_INCLUDED

pcstr make_d3d11_error_string( HRESULT error_code );

// claude@MATCH: the expression is evaluated TWICE - once by the FAILED test and once
// to build the message - so a CHECK_RESULT'ed call emits its `call` twice; the target
// does that at every site (create_xs_hw_impl calls D3DCreateBlob twice,
// create_texture3d calls CreateTexture3D twice), and the on_error argument in the
// VOSTOK_MAKE_STRING slot is the formatted d3d error, not the stringized expression
// (9 pushes / `add esp, 24h`, i.e. the non-variadic on_error overload).
#define CHECK_RESULT( expression, ... )\
	if ( vostok::identity( true ) ) {\
		static bool ignore_always = false;\
		if ( !ignore_always && FAILED( vostok::identity( expression ) ) ) {\
			bool do_debug_break = true;\
			::vostok::debug::on_error(\
				&do_debug_break,\
				vostok::process_error_true,\
				&ignore_always,\
				vostok::assert_untyped,\
				"assertion_failed",\
				make_d3d11_error_string( expression ),\
				__FILE__,\
				__FUNCTION__,\
				__LINE__\
			);\
			if ( ::vostok::debug::is_debugger_present( ) || do_debug_break )\
				DEBUG_BREAK( );\
		}\
	}\
	else ( void )0

#endif // #ifndef VOSTOK_RENDER_CORE_MACROS_EXTENSIONS_H_INCLUDED
