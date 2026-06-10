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

	// STATE[INLINED]: no surviving base COMDAT (no base packet.h unit at all) - every
	// consumer inlines the forward; proof: the base packet<tcp_packet>::append(pcvoid,u32)
	// COMDAT (rva 0x92AA0) folds the capacity check to the raw m_allocated_size load
	// ([this+0xC]). Target keeps only a 4-byte ICF rep (?allocated_size@tcp_packet@..QBEIXZ,
	// rva 0x82CE0) that also fronts packet<tcp_packet>::allocated_size - nothing pairable.
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

private:
	// target mangles AAE (private) - ?reallocate@?$packet@Vtcp_packet..@@AAEXI@Z; only
	// reserve/resize call it, so the original kept it private.
	// STATE[INLINED]: no surviving base COMDAT (no base packet.h unit) - the forward
	// dissolves into reserve/resize, which then call tcp_packet::reallocate out-of-line
	// (the base ?reallocate@tcp_packet@..QAEXI@Z, rva 0x914F0, tcp_packet_inline.h unit).
	// Target made the OPPOSITE inline choice: tcp_packet::reallocate folded INTO this
	// forward, surviving as the 56-byte AAE COMDAT (rva 0x97400) + a 1-byte udp stub -
	// an LTCG inline-direction flip, not steerable from source.
	inline	void		reallocate		( u32 new_size )
	{
		implementation( ).reallocate( new_size );
	}
}; // class packet

} // namespace network_core
} // namespace vostok

#include <vostok/network_core/packet_inline.h>

#endif // #ifndef NETWORK_CORE_PACKET_H_INCLUDED
