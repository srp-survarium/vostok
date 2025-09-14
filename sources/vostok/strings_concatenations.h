////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.10.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_STRINGS_CONCATENATIONS_H_INLUDED
#define VOSTOK_STRINGS_CONCATENATIONS_H_INLUDED

#include <vostok/strings_detail_tuples.h>

namespace vostok {
namespace strings {

inline pstr append	( pstr const result, u32 const size, pcstr source )
{
#ifdef _MSC_VER
	errno_t const error	= strcat_s( result, size, source );
	R_ASSERT_U		( !error );
#else // #ifdef _MSC_VER
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	strcat			( result, source );
#endif // #ifdef _MSC_VER
	return			( result );
}

template < int Size >
inline pstr append	( char (&result)[Size], pcstr source )
{
	return			append( &result[0], Size, source );
}

template < typename T0 >
inline pstr join	( pstr const result, u32 const size, T0 const& str0 )
{
	detail::tuples	tuples( str0 );
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	R_ASSERT		( tuples.size ( ) <= size );
	tuples.concat	( result );
	return			 result;
}

template < int Size, typename T0 >
inline pstr join	( char (&result)[Size], T0 const& str0 )
{
	return			join( &result[0], Size, str0 );
}

template < typename T0, typename T1 >
inline pstr join	( pstr const result, u32 const size, T0 const& str0, T1 const& str1 )
{
	detail::tuples	tuples( str0, str1 );
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	R_ASSERT		( tuples.size ( ) <= size );
	tuples.concat	( result );
	return			 result;
}

template < int Size, typename T0, typename T1 >
inline pstr join	( char (&result)[Size], T0 const& str0, T1 const& str1 )
{
	return			join( &result[0], Size, str0, str1 );
}

template < typename T0, typename T1, typename T2 >
inline pstr join	( pstr const result, u32 const size, T0 const& str0, T1 const& str1, T2 const& str2 )
{
	detail::tuples	tuples( str0, str1, str2 );
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	R_ASSERT		( tuples.size ( ) <= size );
	tuples.concat	( result );
	return			 result;
}

template < int Size, typename T0, typename T1, typename T2 >
inline pstr join	( char (&result)[Size], T0 const& str0, T1 const& str1, T2 const& str2 )
{
	return			join( &result[0], Size, str0, str1, str2 );
}

template < typename T0, typename T1, typename T2, typename T3 >
inline pstr join	( pstr const result, u32 const size, T0 const& str0, T1 const& str1, T2 const& str2, T3 const& str3 )
{
	detail::tuples	tuples( str0, str1, str2, str3 );
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	R_ASSERT		( tuples.size ( ) <= size );
	tuples.concat	( result );
	return			 result;
}

template < int Size, typename T0, typename T1, typename T2, typename T3 >
inline pstr join	( char (&result)[Size], T0 const& str0, T1 const& str1, T2 const& str2, T3 const& str3 )
{
	return			join( &result[0], Size, str0, str1, str2, str3 );
}

template < typename T0, typename T1, typename T2, typename T3, typename T4 >
inline pstr join	( pstr const result, u32 const size, T0 const& str0, T1 const& str1, T2 const& str2, T3 const& str3, T4 const& str4 )
{
	detail::tuples	tuples( str0, str1, str2, str3, str4 );
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	R_ASSERT		( tuples.size ( ) <= size );
	tuples.concat	( result );
	return			 result;
}

template < int Size, typename T0, typename T1, typename T2, typename T3, typename T4 >
inline pstr join	( char (&result)[Size], T0 const& str0, T1 const& str1, T2 const& str2, T3 const& str3, T4 const& str4 )
{
	return			join( &result[0], Size, str0, str1, str2, str3, str4 );
}

template < typename T0, typename T1, typename T2, typename T3, typename T4, typename T5 >
inline pstr join	( pstr const result, u32 const size, T0 const& str0, T1 const& str1, T2 const& str2, T3 const& str3, T4 const& str4, T5 const& str5 )
{
	detail::tuples	tuples( str0, str1, str2, str3, str4, str5 );
	VOSTOK_UNREFERENCED_PARAMETER	( size );
	R_ASSERT		( tuples.size ( ) <= size );
	tuples.concat	( result );
	return			 result;
}

template < int Size, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5 >
inline pstr join	( char (&result)[Size], T0 const& str0, T1 const& str1, T2 const& str2, T3 const& str3, T4 const& str4, T5 const& str5 )
{
	return			join( &result[0], Size, str0, str1, str2, str3, str4, str5 );
}

} // namespace strings
} // namespace vostok

#define STR_JOINA(dest, ...) \
	if ( vostok::identity(true) ) {								\
		vostok::strings::detail::tuples				STR_JOINA_tuples_unique_identifier ( __VA_ARGS__ ); \
		( dest )									= ( pstr )ALLOCA( STR_JOINA_tuples_unique_identifier.size ( ) ); \
		STR_JOINA_tuples_unique_identifier.concat	( dest );	\
		VOSTOK_ANALYSIS_ASSUME						( dest );	\
	}															\
	else														\
		VOSTOK_ANALYSIS_ASSUME( dest )

#define STR_DUPLICATEA(dest, source) \
	if ( vostok::identity(true) ) { \
		u32 const buffer_size						= (vostok::strings::length( source ) + 1)*sizeof(char); \
		( dest )									= ( pstr )ALLOCA( buffer_size ); \
		VOSTOK_ANALYSIS_ASSUME						( dest );	\
		vostok::memory::copy							( dest, buffer_size, source, buffer_size ); \
	}															\
	else														\
		VOSTOK_ANALYSIS_ASSUME( dest )

#endif // #ifndef VOSTOK_STRINGS_CONCATENATIONS_H_INLUDED