////////////////////////////////////////////////////////////////////////////
//	Created 	: 08.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
#define NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED

namespace vostok {
namespace network_core {

// claude@NOTE: intrusive-ref-counted allocator (used via intrusive_ptr). The PDB
// records only the ctor; full member layout is unknown and no enabled TU holds it
// by value, so the body is a stub for the matcher.
class udp_match_packets_allocator {
public:
	inline	udp_match_packets_allocator	( memory::base_allocator& allocator, void* arena, u32 arena_size );
}; // class udp_match_packets_allocator

// STATE[STUB]
inline udp_match_packets_allocator::udp_match_packets_allocator( memory::base_allocator& allocator, void* arena, u32 arena_size )
{
	VOSTOK_UNREFERENCED_PARAMETERS( allocator, arena, arena_size );

	// FUNCTION BODY[0xeac50]: 0
	// <0xeac50>|0x000|+0x02c:'21'	{
	// <0xeac7c>|0x02c|      :'22'	}
	// ******
}

} // namespace network_core
} // namespace vostok

#endif // #ifndef NETWORK_CORE_UDP_MATCH_PACKETS_ALLOCATOR_H_INCLUDED
