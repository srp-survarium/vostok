////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_H_INCLUDED
#define NETWORK_CORE_PACKET_H_INCLUDED

#include <vostok/network_core/base_packet.h>

namespace vostok {
namespace network_core {

template < typename T >
class packet : public base_packet {
public:
	inline				packet			( );

	// STATE[PARTIAL]: CRTP forward to implementation().allocated_size() ([+0xC]);
	// shape exact, base instance is the address-anchored debug COMDAT.
	inline	u32			allocated_size	( ) const
	{
		return implementation( ).allocated_size( );
	}

	inline	void		clear			( );

	inline	void		reserve			( u32 size );
	inline	void		resize			( u32 size );
	inline	void		clone			( base_packet const& other );

	inline	void		append			( bool value );
	inline	void		append			( u8 value );
	inline	void		append			( s8 value );
	inline	void		append			( u16 value );
	inline	void		append			( s16 value );
	inline	void		append			( u32 value );
	inline	void		append			( s32 value );
	inline	void		append			( u64 value );
	inline	void		append			( s64 value );
	inline	void		append			( float value );
	inline	void		append			( float2 const& value );
	inline	void		append			( float3 const& value );
	inline	void		append			( pcstr string );
	inline	void		append			( pcstr string, u8 string_length );
	inline	void		append			( pcvoid buffer, u32 buffer_size );

	inline	T const&	implementation	( ) const;
	inline	T&			implementation	( );

	// STATE[PARTIAL]: forwards to implementation().reallocate(); the surviving
	// COMDAT for packet< tcp_packet > is the inlined tcp_packet::reallocate (the
	// allocator + 3-byte length-prefix dance). Shape exact vs target.
	inline	void		reallocate		( u32 new_size )
	{
		implementation( ).reallocate( new_size );
	}
}; // class packet

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/packet_inline.h>

#endif // #ifndef NETWORK_CORE_PACKET_H_INCLUDED
