////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PLATFORM_EXTENSIONS_H_INCLUDED
#define VOSTOK_PLATFORM_EXTENSIONS_H_INCLUDED

namespace vostok {
namespace platform {

inline bool	little_endian	( )
{
	u16	const word	= 0x0001;
	return			( !!*( u8 const* )&word );
}

inline bool	big_endian		( )
{
	return			( !little_endian( ) );
}

inline pcstr platform_name ( )
{
#if VOSTOK_PLATFORM_PS3
	return			"ps3";
#elif VOSTOK_PLATFORM_XBOX_360
	return			"xbox360";
#elif VOSTOK_PLATFORM_WINDOWS
	return			"pc_dx11";
#else
	#error			"Platform macro is not defined!"
#endif
}

} // namespace platform
} // namespace vostok

#endif // #ifndef VOSTOK_PLATFORM_EXTENSIONS_H_INCLUDED