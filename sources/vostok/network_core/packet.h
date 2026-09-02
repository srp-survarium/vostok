// SPDX-License-Identifier: GPL-3.0-or-later
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

	inline	u32			allocated_size	( ) const
	{
		return implementation( ).allocated_size( );
	}

	inline	void		clear			( );

	inline	void		reserve			( u32 size );
	inline	void		resize			( u32 size );
	inline	void		clone			( base_packet const& other );

	inline	void		append			( const bool value );
	inline	void		append			( const u8 value );
	inline	void		append			( const s8 value );
	inline	void		append			( const u16 value );
	inline	void		append			( const s16 value );
	inline	void		append			( const u32 value );
	inline	void		append			( const s32 value );
	inline	void		append			( const u64 value );
	inline	void		append			( const s64 value );
	inline	void		append			( const float value );
	inline	void		append			( float2 const& value );
	inline	void		append			( float3 const& value );
	inline	void		append			( pcstr string );
	inline	void		append			( pcstr string, u8 string_length );
	inline	void		append			( pcvoid buffer, u32 buffer_size );

private:
	inline	T const&	implementation	( ) const;
	inline	T&			implementation	( );

	// target mangles AAE (private) - ?reallocate@?$packet@Vtcp_packet..@@AAEXI@Z; only
	// reserve/resize call it, so the original kept it private.
	inline	void		reallocate		( u32 new_size )
	{
		implementation( ).reallocate( new_size );
	}
}; // class packet

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/packet_inline.h>

#endif // #ifndef NETWORK_CORE_PACKET_H_INCLUDED
