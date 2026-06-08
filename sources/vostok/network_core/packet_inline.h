////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
#define NETWORK_CORE_PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

template < typename T >
inline packet< T >::packet( )
{
	// FUNCTION BODY[0x8d680]: 0
	// <0x8d680>|0x000|+0x007:'15'	{
	// <0x8d687>|0x007|      :'16'	}
	// ******
}

template < typename T >
inline void packet< T >::clear( ) { /* no source */ }

template < typename T >
inline void packet< T >::reserve( u32 size ) { /* no source */ }

template < typename T >
inline void packet< T >::resize( u32 size )
{
	// FUNCTION BODY[0x134ee0]: 3
	// <0x134eea>|0x00a|+0x00c:'36'
	// <0x134ef6>|0x016|+0x01a:'37'
	// <0x134f10>|0x030|+0x009:'38'
	// ******
}

template < typename T >
inline void packet< T >::clone( base_packet const& other ) { /* no source */ }

template < typename T >
inline void packet< T >::append( bool value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( u8 value )
{
	// FUNCTION BODY[0x8d750]: 1
	// <0x8d751>|0x001|+0x011:'57'
	// ******
}

template < typename T >
inline void packet< T >::append( s8 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( u16 value )
{
	// FUNCTION BODY[0x8d730]: 1
	// <0x8d731>|0x001|+0x011:'69'
	// ******
}

template < typename T >
inline void packet< T >::append( s16 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( u32 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( s32 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( u64 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( s64 value ) { /* no source */ }

template < typename T >
inline void packet< T >::append( float value )
{
	// FUNCTION BODY[0x8d710]: 1
	// <0x8d711>|0x001|+0x011:'105'
	// ******
}

template < typename T >
inline void packet< T >::append( float2 const& value )
{
	// FUNCTION BODY[0x8d700]: 1
	// <0x8d701>|0x001|+0x00d:'111'
	// ******
}

template < typename T >
inline void packet< T >::append( float3 const& value )
{
	// FUNCTION BODY[0x8d6f0]: 1
	// <0x8d6f1>|0x001|+0x00d:'117'
	// ******
}

template < typename T >
inline void packet< T >::append( pcstr string ) { /* no source */ }

template < typename T >
inline void packet< T >::append( pcstr string, u8 string_length ) { /* no source */ }

template < typename T >
inline void packet< T >::append( pcvoid buffer, u32 buffer_size )
{
	// FUNCTION BODY[0xa72f0]: 11 : packet< tcp_packet >
	// <0xa72f0>|0x000|+0x019:'137'
	// <0xa7309>|0x019|+0x006:'138'
	// <0xa730f>|0x01f|+0x004:'139'
	// <0xa7313>|0x023|+0x00a:'140'
	// <0>
	// <0xa731d>|0x02d|+0x031:'142'
	// <0>
	// <1>
	// <2>
	// <0xa734e>|0x05e|+0x010:'146'
	// <0xa735e>|0x06e|+0x007:'147'
	// ******
	// FUNCTION BODY[0x8d690]: 11 : packet< udp_match_packet >
	// <0x8d695>|0x005|+0x01a:'137'
	// <0x8d6af>|0x01f|+0x006:'138'
	// <0x8d6b5>|0x025|+0x004:'139'
	// <0x8d6b9>|0x029|+0x00d:'140'
	// <0>
	// <0x8d6c6>|0x036|+0x005:'142'
	// <0>
	// <1>
	// <2>
	// <0x8d6cb>|0x03b|+0x010:'146'
	// <0x8d6db>|0x04b|+0x007:'147'
	// ******
}

template < typename T >
inline T const& packet< T >::implementation( ) const { /* no source */ }

template < typename T >
inline T& packet< T >::implementation( ) { /* no source */ }

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_INLINE_H_INCLUDED
