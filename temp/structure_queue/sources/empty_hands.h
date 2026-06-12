////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\GAME\SOURCES\EMPTY_HANDS_H_INCLUDED
#define VOSTOK\GAME\SOURCES\EMPTY_HANDS_H_INCLUDED

namespace survarium {

// STATE[STUB]
inline void* empty_hands::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0x607c0]
	// <0x4f210>|0x000|      :'31'	{
	// ******
}

// STATE[STUB]
inline float4x4 empty_hands::transform( ) const
{
	return vostok::math::float4x4();

	// FUNCTION BODY[0xacc70]
	// <0xacc70>|0x000|      :'32'	{
	// ******
}

// STATE[STUB]
inline bool empty_hands::is_ready_to_be_deactivated( ) const
{
	return false;

	// FUNCTION BODY[0x22700]
	// <0x22700>|0x000|      :'34'	{
	// ******
}

// STATE[STUB]
inline bool empty_hands::is_sprinting( ) const
{
	return false;

	// FUNCTION BODY[0xb2740]
	// <0xb2740>|0x000|      :'47'	{
	// ******
}

// STATE[STUB]
inline void empty_hands::serialize( network_core::udp_match_packet& packet, u32 client_offset ) const
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9da12 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x9da12 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// ******

	// FUNCTION BODY[0xaea00]
	// <0xaea00>|0x000|      :'49'	{
	// ******
}

// STATE[STUB]
inline void empty_hands::deserialize( network_core::packet_reader& reader )
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x111e20 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	// FUNCTION BODY[0x122e20]
	// <0x122e20>|0x000|      :'50'	{
	// ******
}

} // namespace survarium

#endif // #ifndef VOSTOK\GAME\SOURCES\EMPTY_HANDS_H_INCLUDED
