////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_client.h"

namespace survarium {

// STATE[STUB]
void network_client::on_match_packet_received( const u8 message_type, network_core::packet_reader& packet )
{
	// STATICS
	// static < NoType > 				 = <0x7c0620>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af41c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN30") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af42e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN29") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af440 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN28") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af45b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN27") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af48f }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN23") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af4a2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN22") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af4b4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN21") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af4c7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN20") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af4d9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN19") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af4eb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN18") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af4fd }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN17") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af510 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN16") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af522 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN15") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af534 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN14") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af546 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af55a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af56b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af57e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af590 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af5a3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af5b6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af5c9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af5dc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af5ef }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af600 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7af60f }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x7c0400]: 137
	// <0x7c0400>|0x000|+0x007:'14'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7c0407>|0x007|+0x015:'20'
	// <0>
	// <0x7c041c>|0x01c|+0x012:'22'
	// <0>
	// <1>
	// <2>
	// <0x7c042e>|0x02e|+0x012:'26'
	// <0>
	// <1>
	// <2>
	// <0x7c0440>|0x040|+0x01b:'30'
	// <0>
	// <1>
	// <2>
	// <0x7c045b>|0x05b|+0x015:'34'
	// <0>
	// <1>
	// <0x7c0470>|0x070|+0x00a:'37'
	// <0x7c047a>|0x07a|+0x015:'38'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7c048f>|0x08f|+0x013:'44'
	// <0>
	// <1>
	// <2>
	// <0x7c04a2>|0x0a2|+0x012:'48'
	// <0>
	// <1>
	// <2>
	// <0x7c04b4>|0x0b4|+0x013:'52'
	// <0>
	// <1>
	// <2>
	// <0x7c04c7>|0x0c7|+0x012:'56'
	// <0>
	// <1>
	// <2>
	// <0x7c04d9>|0x0d9|+0x012:'60'
	// <0>
	// <1>
	// <2>
	// <0x7c04eb>|0x0eb|+0x012:'64'
	// <0>
	// <1>
	// <2>
	// <0x7c04fd>|0x0fd|+0x013:'68'
	// <0>
	// <1>
	// <2>
	// <0x7c0510>|0x110|+0x012:'72'
	// <0>
	// <1>
	// <2>
	// <0x7c0522>|0x122|+0x012:'76'
	// <0>
	// <1>
	// <2>
	// <0x7c0534>|0x134|+0x012:'80'
	// <0>
	// <1>
	// <2>
	// <0x7c0546>|0x146|+0x014:'84'
	// <0>
	// <1>
	// <2>
	// <0x7c055a>|0x15a|+0x011:'88'
	// <0>
	// <1>
	// <2>
	// <0x7c056b>|0x16b|+0x013:'92'
	// <0>
	// <1>
	// <2>
	// <0x7c057e>|0x17e|+0x012:'96'
	// <0>
	// <1>
	// <2>
	// <0x7c0590>|0x190|+0x013:'100'
	// <0>
	// <1>
	// <2>
	// <0x7c05a3>|0x1a3|+0x013:'104'
	// <0>
	// <1>
	// <2>
	// <0x7c05b6>|0x1b6|+0x013:'108'
	// <0>
	// <1>
	// <2>
	// <0x7c05c9>|0x1c9|+0x013:'112'
	// <0>
	// <1>
	// <2>
	// <0x7c05dc>|0x1dc|+0x013:'116'
	// <0>
	// <1>
	// <2>
	// <0x7c05ef>|0x1ef|+0x011:'120'
	// <0>
	// <1>
	// <2>
	// <0x7c0600>|0x200|+0x00f:'124'
	// <0>
	// <1>
	// <2>
	// <0x7c060f>|0x20f|-0x1ea:'128'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <0x7c0425>|0x025|+0x012:'152'
	// <0x7c0437>|0x037|+0x01b:'152'
	// <0x7c0452>|0x052|+0x034:'152'
	// <0x7c0486>|0x086|+0x013:'152'
	// <0x7c0499>|0x099|+0x012:'152'
	// <0x7c04ab>|0x0ab|+0x013:'152'
	// <0x7c04be>|0x0be|+0x012:'152'
	// <0x7c04d0>|0x0d0|+0x012:'152'
	// <0x7c04e2>|0x0e2|+0x012:'152'
	// <0x7c04f4>|0x0f4|+0x013:'152'
	// <0x7c0507>|0x107|+0x012:'152'
	// <0x7c0519>|0x119|+0x012:'152'
	// <0x7c052b>|0x12b|+0x012:'152'
	// <0x7c053d>|0x13d|+0x014:'152'
	// <0x7c0551>|0x151|+0x011:'152'
	// <0x7c0562>|0x162|+0x013:'152'
	// <0x7c0575>|0x175|+0x012:'152'
	// <0x7c0587>|0x187|+0x013:'152'
	// <0x7c059a>|0x19a|+0x013:'152'
	// <0x7c05ad>|0x1ad|+0x013:'152'
	// <0x7c05c0>|0x1c0|+0x013:'152'
	// <0x7c05d3>|0x1d3|+0x013:'152'
	// <0x7c05e6>|0x1e6|+0x011:'152'
	// <0x7c05f7>|0x1f7|+0x00f:'152'
	// <0x7c0606>|0x206|+0x011:'152'
	// <0x7c0617>|0x217|      :'152'	}
	// ******
}

} // namespace survarium
