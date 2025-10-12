////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_PACKET_H_INCLUDED
#define NETWORK_CORE_PACKET_H_INCLUDED

#include <vostok/network_core/base_packet.h>

namespace vostok {
namespace network_core {

template <typename T>
class packet : public base_packet {
public:
	inline	explicit	packet			( );
	inline				~packet			( );

	inline	u32			allocated_size	( ) const { return m_allocated_size; }
	inline	void		clear			( );
	inline	void		reserve			( u32 size );
	inline	void		resize			( u32 size );
	inline	void		clone			( base_packet const& other );

	inline	void		append			( pcvoid buffer, u32 buffer_size );
	inline	void		append			( pcstr string, u8 string_length );
	inline	void		append			( pcstr string );

	inline	void		append			( float3 size );
	inline	void		append			( float2 size );
	inline	void		append			( float size );

	inline	void		append			( s64 size );
	inline	void		append			( u64 size );

	inline	void		append			( s32 size );
	inline	void		append			( u32 size );

	inline	void		append			( s16 size );
	inline	void		append			( u16 size );

	inline	void		append			( s8 size );
	inline	void		append			( u8 size );

	inline	void		append			( bool size );

public:
	inline	T&			implementation	( );
	inline	T const&	implementation	( ) const;

private:
	inline	void		reallocate		( u32 new_size );
}; // class packet

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_PACKET_H_INCLUDED