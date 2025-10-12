////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_INLINE_H_INCLUDED
#define PACKET_INLINE_H_INCLUDED

namespace vostok {
namespace network_core {

// STATE[STUB]
// vostok::network_core::packet<vostok::network_core::udp_match_packet>::packet<vostok::network_core::udp_match_packet>()
packet<udp_match_packet>::packet<udp_match_packet>( )
{
}

// STATE[STUB]
// void vostok::network_core::packet<vostok::network_core::udp_match_packet>::append(const unsigned char)
void packet<udp_match_packet>::append( u8 value )
{
	// FUNCTION BODY
	// <0x8d751>|0x000|0x000:'57'
	// ******
}

// STATE[STUB]
// void vostok::network_core::packet<vostok::network_core::udp_match_packet>::append(const unsigned short)
void packet<udp_match_packet>::append( u16 value )
{
	// FUNCTION BODY
	// <0x8d731>|0x000|0x000:'69'
	// ******
}

// STATE[STUB]
// void vostok::network_core::packet<vostok::network_core::udp_match_packet>::append(const float)
void packet<udp_match_packet>::append( float value )
{
	// FUNCTION BODY
	// <0x8d711>|0x000|0x000:'105'
	// ******
}

// STATE[STUB]
// void vostok::network_core::packet<vostok::network_core::udp_match_packet>::append(vostok::math::float2 const&)
void packet<udp_match_packet>::append( float2 const& value )
{
	// FUNCTION BODY
	// <0x8d701>|0x000|0x000:'111'
	// ******
}

// STATE[STUB]
// void vostok::network_core::packet<vostok::network_core::udp_match_packet>::append(vostok::math::float3 const&)
void packet<udp_match_packet>::append( float3 const& value )
{
	// FUNCTION BODY
	// <0x8d6f1>|0x000|0x000:'117'
	// ******
}

// STATE[STUB]
// void vostok::network_core::packet<vostok::network_core::udp_match_packet>::append(void const*, unsigned int)
void packet<udp_match_packet>::append( void const* buffer, u32 buffer_size )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7c6e5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN44") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7c6cb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN45") })
	// ******

	// FUNCTION BODY
	// <0x8d695>|0x000|0x000:'137'
	// <0x8d6af>|0x01a|0x01a:'138'
	// <0x8d6b5>|0x020|0x006:'139'
	// <0x8d6b9>|0x024|0x004:'140'
	// 1
	// <0x8d6c6>|0x031|0x00d:'142'
	// 1
	// 2
	// 3
	// <0x8d6cb>|0x036|0x005:'146'
	// <0x8d6db>|0x046|0x010:'147'
	// ******
}

	// TYPEDEFS
	typedef
		collision::bone_collision_data const*
		iterator_type;

	typedef
		collision::bone_collision_data*
		iterator_type;

	typedef
		survarium::base_project::resolve_link_object*
		iterator_type;

	typedef
		survarium::scheduler::record*
		iterator_type;

	// ******

} // namespace network_core
} // namespace vostok

#endif // #ifndef PACKET_INLINE_H_INCLUDED
