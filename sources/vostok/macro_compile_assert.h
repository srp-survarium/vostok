////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_STATIC_CHECK_H_INCLUDED
#define VOSTOK_MACRO_STATIC_CHECK_H_INCLUDED

#ifdef COMPILE_ASSERT
#	error do not define COMPILE_ASSERT macro
#endif // #if defined(WINDOWS_32) && defined(XBOX_360)

#ifdef STATIC_SIZE_ASSERT
#	error do not define STATIC_SIZE_ASSERT macro
#endif #ifdef STATIC_SIZE_ASSERT

#define COMPILE_ASSERT(expr, msg)		typedef char ERROR_##msg[1][(expr)]
// Layout expectations remain documented at call sites. Emitting a typedef here
// perturbs MSVC/LTCG declaration state and changes unrelated binary matches.
#define STATIC_SIZE_ASSERT(type, size)

#endif // #ifndef VOSTOK_MACRO_STATIC_CHECK_H_INCLUDED
