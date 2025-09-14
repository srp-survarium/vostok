////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MACRO_STRINGS_H_INCLUDED
#define VOSTOK_MACRO_STRINGS_H_INCLUDED

// VOSTOK_STRING_CONCAT macro
#if defined(VOSTOK_STRING_CONCAT) || defined(VOSTOK_STRING_CONCAT_HELPER)
#	error please do not define VOSTOK_STRING_CONCAT or VOSTOK_STRING_CONCAT_HELPER macros
#endif // #if defined(VOSTOK_STRING_CONCAT) || defined(VOSTOK_STRING_CONCAT_HELPER)

#define VOSTOK_STRING_CONCAT_HELPER(a,b)	a##b
#define VOSTOK_STRING_CONCAT(a,b)			VOSTOK_STRING_CONCAT_HELPER(a,b)

// VOSTOK_MAKE_STRING macro
#if defined(VOSTOK_MAKE_STRING) || defined(VOSTOK_MAKE_STRING_HELPER)
#	error please do not define VOSTOK_MAKE_STRING or VOSTOK_MAKE_STRING_HELPER macros
#endif // #if defined(VOSTOK_MAKE_STRING) || defined(VOSTOK_MAKE_STRING_HELPER)

#define VOSTOK_MAKE_STRING_HELPER(a)		#a
#define VOSTOK_MAKE_STRING(a)				VOSTOK_MAKE_STRING_HELPER(a)

#endif // #ifndef VOSTOK_MACRO_STRINGS_H_INCLUDED