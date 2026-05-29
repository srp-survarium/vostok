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
	return			"pc_dx11";
}

} // namespace platform
} // namespace vostok

#endif // #ifndef VOSTOK_PLATFORM_EXTENSIONS_H_INCLUDED