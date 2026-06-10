////////////////////////////////////////////////////////////////////////////
//	Created		: 09.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef UDP_MATCH_FIXED_PACKETS_ALLOCATOR_H_INCLUDED
#define UDP_MATCH_FIXED_PACKETS_ALLOCATOR_H_INCLUDED

#include <vostok/network_core/udp_match_packets_allocator.h>

namespace vostok {
namespace network {

// PDB instantiations: < 2048 > (0x96014) and < 8192 > (0x258014)
template < u32 packets_count >
class udp_match_fixed_packets_allocator : public network_core::udp_match_packets_allocator {
public:
	// STATE[STUB]: arena wiring follows the base ctor (allocator, arena, size)
	inline	explicit	udp_match_fixed_packets_allocator	( memory::base_allocator& allocator ) :
		network_core::udp_match_packets_allocator( allocator, m_buffer, sizeof( m_buffer ) )
	{
	}

	inline				~udp_match_fixed_packets_allocator	( ) { /* no source */ }

private:
	char	m_buffer[ 300 * packets_count ];
}; // class udp_match_fixed_packets_allocator

STATIC_SIZE_ASSERT(udp_match_fixed_packets_allocator< 2048 >, 0x96014);
STATIC_SIZE_ASSERT(udp_match_fixed_packets_allocator< 8192 >, 0x258014);

} // namespace network
} // namespace vostok

#endif // #ifndef UDP_MATCH_FIXED_PACKETS_ALLOCATOR_H_INCLUDED
