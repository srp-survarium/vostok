////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\macros_extensions.h"

// STATE[STUB]
pcstr make_d3d11_error_string( HRESULT error_code )
{
	// STATICS
	// static < NoType > 				 = <0x559730>;
	// static < NoType > 				 = <0x55970c>;
	// static < NoType > 				 = <0x5596d0>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x54861c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN21") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548622 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN20") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5486a3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN19") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5486a9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN17") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5486af }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN16") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5486b5 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN15") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5486bb }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN14") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5486c1 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548628 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x54862e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548634 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x54863a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548640 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548646 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x54864c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548652 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548658 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x54865e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548664 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x54866a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	return NULL;

	// FUNCTION BODY[0x559590]: 38
	// <0>
	// <1>
	// <0x559590>|0x000|+0x043:'17'
	// <0x5595d3>|0x043|+0x024:'17'
	// <0x5595f7>|0x067|+0x07f:'17'
	// <0x559676>|0x0e6|+0x01c:'17'
	// <0x559692>|0x102|-0x00c:'17'
	// <0>
	// <0x559686>|0x0f6|-0x016:'19'
	// <0x559670>|0x0e0|-0x08b:'20'
	// <0x5595e5>|0x055|+0x006:'21'
	// <0x5595eb>|0x05b|-0x02a:'22'
	// <0x5595c1>|0x031|+0x00c:'23'
	// <0x5595cd>|0x03d|-0x012:'24'
	// <0x5595bb>|0x02b|+0x00c:'25'
	// <0x5595c7>|0x037|+0x02a:'26'
	// <0x5595f1>|0x061|+0x02b:'27'
	// <0x55961c>|0x08c|+0x006:'28'
	// <0x559622>|0x092|+0x081:'29'
	// <0x5596a3>|0x113|-0x017:'30'
	// <0x55968c>|0x0fc|+0x01d:'31'
	// <0x5596a9>|0x119|+0x006:'32'
	// <0x5596af>|0x11f|+0x006:'33'
	// <0x5596b5>|0x125|+0x006:'34'
	// <0x5596bb>|0x12b|+0x006:'35'
	// <0x5596c1>|0x131|-0x099:'36'
	// <0x559628>|0x098|+0x006:'37'
	// <0x55962e>|0x09e|+0x006:'38'
	// <0x559634>|0x0a4|+0x006:'39'
	// <0x55963a>|0x0aa|+0x006:'40'
	// <0x559640>|0x0b0|+0x006:'41'
	// <0x559646>|0x0b6|+0x006:'42'
	// <0x55964c>|0x0bc|+0x006:'43'
	// <0x559652>|0x0c2|+0x006:'44'
	// <0x559658>|0x0c8|+0x006:'45'
	// <0x55965e>|0x0ce|+0x006:'46'
	// <0x559664>|0x0d4|+0x006:'47'
	// <0x55966a>|0x0da|+0x05d:'48'
	// <0>
	// <1>
	// <2>
	// <0x5596c7>|0x137|-0x107:'52'
	// <0x5595c0>|0x030|+0x006:'53'
	// <0x5595c6>|0x036|+0x006:'53'
	// <0x5595cc>|0x03c|+0x006:'53'
	// <0x5595d2>|0x042|+0x018:'53'
	// <0x5595ea>|0x05a|+0x006:'53'
	// <0x5595f0>|0x060|+0x006:'53'
	// <0x5595f6>|0x066|+0x02b:'53'
	// <0x559621>|0x091|+0x006:'53'
	// <0x559627>|0x097|+0x006:'53'
	// <0x55962d>|0x09d|+0x006:'53'
	// <0x559633>|0x0a3|+0x006:'53'
	// <0x559639>|0x0a9|+0x006:'53'
	// <0x55963f>|0x0af|+0x006:'53'
	// <0x559645>|0x0b5|+0x006:'53'
	// <0x55964b>|0x0bb|+0x006:'53'
	// <0x559651>|0x0c1|+0x006:'53'
	// <0x559657>|0x0c7|+0x006:'53'
	// <0x55965d>|0x0cd|+0x006:'53'
	// <0x559663>|0x0d3|+0x006:'53'
	// <0x559669>|0x0d9|+0x006:'53'
	// <0x55966f>|0x0df|+0x006:'53'
	// <0x559675>|0x0e5|+0x016:'53'
	// <0x55968b>|0x0fb|+0x006:'53'
	// <0x559691>|0x101|+0x017:'53'
	// <0x5596a8>|0x118|+0x006:'53'
	// <0x5596ae>|0x11e|+0x006:'53'
	// <0x5596b4>|0x124|+0x006:'53'
	// <0x5596ba>|0x12a|+0x006:'53'
	// <0x5596c0>|0x130|+0x006:'53'
	// <0x5596c6>|0x136|+0x006:'53'
	// ******
}

