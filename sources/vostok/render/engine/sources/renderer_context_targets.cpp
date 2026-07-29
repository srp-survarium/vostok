#include "pch.h"
#include "renderer_context_targets.h"

namespace vostok {
namespace render {

// STATE[STUB]
pcstr rt_index_to_name( enum_render_target_index index )
{
	// STATICS
	// static < NoType > 				 = <0x634618>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623470 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN73") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623476 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN72") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62347c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN71") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623482 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN70") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623488 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN69") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62348e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN68") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623494 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN67") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62349a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN66") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234a0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN65") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234a6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN64") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234ac }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN63") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234b2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN62") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234b8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN61") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234be }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN60") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234c4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN59") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234ca }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN58") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234d0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN57") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234d6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN56") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234dc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN55") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234e2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN54") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234e8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN53") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234ee }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN52") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234f4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN51") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6234fa }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN50") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623500 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN49") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623506 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN48") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62350c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN47") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623512 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN46") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623518 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN45") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62351e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN44") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623524 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN43") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62352a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN42") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623530 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN41") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623536 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN40") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62353c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN39") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623542 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN38") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623548 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN37") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62354e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN36") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623554 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN35") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62355a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN34") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623560 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN33") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623566 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN32") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62356c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN31") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623572 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN30") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623578 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN29") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62357e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN28") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623584 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN27") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62358a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN26") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623590 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN25") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623596 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN24") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62359c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN23") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235a2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN22") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235a8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN21") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235ae }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN20") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235b4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN19") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235ba }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN18") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235c0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN17") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235c6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN16") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235cc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN15") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235d2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN14") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235d8 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN13") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235de }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN12") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235e4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235ea }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235f0 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235f6 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6235fc }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623602 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623608 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x62360e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// ******

	return NULL;

	// FUNCTION BODY[0x634460]: 78
	// <0x634460>|0x000|+0x010:'28'
	// <0>
	// <0x634470>|0x010|+0x006:'30'
	// <0x634476>|0x016|+0x006:'31'
	// <0x63447c>|0x01c|+0x006:'32'
	// <0x634482>|0x022|+0x006:'33'
	// <0x634488>|0x028|+0x006:'34'
	// <0x63448e>|0x02e|+0x006:'35'
	// <0x634494>|0x034|+0x006:'36'
	// <0x63449a>|0x03a|+0x006:'37'
	// <0x6344a0>|0x040|+0x006:'38'
	// <0x6344a6>|0x046|+0x006:'39'
	// <0x6344ac>|0x04c|+0x006:'40'
	// <0x6344b2>|0x052|+0x006:'41'
	// <0x6344b8>|0x058|+0x006:'42'
	// <0x6344be>|0x05e|+0x006:'43'
	// <0x6344c4>|0x064|+0x006:'44'
	// <0x6344ca>|0x06a|+0x006:'45'
	// <0x6344d0>|0x070|+0x006:'46'
	// <0x6344d6>|0x076|+0x006:'47'
	// <0x6344dc>|0x07c|+0x006:'48'
	// <0x6344e2>|0x082|+0x006:'49'
	// <0x6344e8>|0x088|+0x006:'50'
	// <0x6344ee>|0x08e|+0x006:'51'
	// <0x6344f4>|0x094|+0x006:'52'
	// <0x6344fa>|0x09a|+0x006:'53'
	// <0x634500>|0x0a0|+0x006:'54'
	// <0x634506>|0x0a6|+0x006:'55'
	// <0x63450c>|0x0ac|+0x006:'56'
	// <0x634512>|0x0b2|+0x006:'57'
	// <0x634518>|0x0b8|+0x006:'58'
	// <0x63451e>|0x0be|+0x006:'59'
	// <0x634524>|0x0c4|+0x006:'60'
	// <0x63452a>|0x0ca|+0x006:'61'
	// <0x634530>|0x0d0|+0x006:'62'
	// <0x634536>|0x0d6|+0x006:'63'
	// <0x63453c>|0x0dc|+0x006:'64'
	// <0x634542>|0x0e2|+0x006:'65'
	// <0x634548>|0x0e8|+0x006:'66'
	// <0x63454e>|0x0ee|+0x006:'67'
	// <0x634554>|0x0f4|+0x006:'68'
	// <0x63455a>|0x0fa|+0x006:'69'
	// <0x634560>|0x100|+0x006:'70'
	// <0x634566>|0x106|+0x006:'71'
	// <0x63456c>|0x10c|+0x006:'72'
	// <0x634572>|0x112|+0x006:'73'
	// <0x634578>|0x118|+0x006:'74'
	// <0x63457e>|0x11e|+0x006:'75'
	// <0x634584>|0x124|+0x006:'76'
	// <0x63458a>|0x12a|+0x006:'77'
	// <0x634590>|0x130|+0x006:'78'
	// <0x634596>|0x136|+0x006:'79'
	// <0x63459c>|0x13c|+0x006:'80'
	// <0x6345a2>|0x142|+0x006:'81'
	// <0x6345a8>|0x148|+0x006:'82'
	// <0x6345ae>|0x14e|+0x006:'83'
	// <0x6345b4>|0x154|+0x006:'84'
	// <0x6345ba>|0x15a|+0x006:'85'
	// <0x6345c0>|0x160|+0x006:'86'
	// <0x6345c6>|0x166|+0x006:'87'
	// <0x6345cc>|0x16c|+0x006:'88'
	// <0x6345d2>|0x172|+0x006:'89'
	// <0x6345d8>|0x178|+0x006:'90'
	// <0x6345de>|0x17e|+0x006:'91'
	// <0x6345e4>|0x184|+0x006:'92'
	// <0x6345ea>|0x18a|+0x006:'93'
	// <0x6345f0>|0x190|+0x006:'94'
	// <0x6345f6>|0x196|+0x006:'95'
	// <0x6345fc>|0x19c|+0x006:'96'
	// <0x634602>|0x1a2|+0x006:'97'
	// <0x634608>|0x1a8|+0x006:'98'
	// <0x63460e>|0x1ae|+0x006:'99'
	// <0>
	// <1>
	// <2>
	// <0x634614>|0x1b4|-0x19f:'103'
	// <0>
	// <1>
	// <0x634475>|0x015|+0x006:'106'
	// <0x63447b>|0x01b|+0x006:'106'
	// <0x634481>|0x021|+0x006:'106'
	// <0x634487>|0x027|+0x006:'106'
	// <0x63448d>|0x02d|+0x006:'106'
	// <0x634493>|0x033|+0x006:'106'
	// <0x634499>|0x039|+0x006:'106'
	// <0x63449f>|0x03f|+0x006:'106'
	// <0x6344a5>|0x045|+0x006:'106'
	// <0x6344ab>|0x04b|+0x006:'106'
	// <0x6344b1>|0x051|+0x006:'106'
	// <0x6344b7>|0x057|+0x006:'106'
	// <0x6344bd>|0x05d|+0x006:'106'
	// <0x6344c3>|0x063|+0x006:'106'
	// <0x6344c9>|0x069|+0x006:'106'
	// <0x6344cf>|0x06f|+0x006:'106'
	// <0x6344d5>|0x075|+0x006:'106'
	// <0x6344db>|0x07b|+0x006:'106'
	// <0x6344e1>|0x081|+0x006:'106'
	// <0x6344e7>|0x087|+0x006:'106'
	// <0x6344ed>|0x08d|+0x006:'106'
	// <0x6344f3>|0x093|+0x006:'106'
	// <0x6344f9>|0x099|+0x006:'106'
	// <0x6344ff>|0x09f|+0x006:'106'
	// <0x634505>|0x0a5|+0x006:'106'
	// <0x63450b>|0x0ab|+0x006:'106'
	// <0x634511>|0x0b1|+0x006:'106'
	// <0x634517>|0x0b7|+0x006:'106'
	// <0x63451d>|0x0bd|+0x006:'106'
	// <0x634523>|0x0c3|+0x006:'106'
	// <0x634529>|0x0c9|+0x006:'106'
	// <0x63452f>|0x0cf|+0x006:'106'
	// <0x634535>|0x0d5|+0x006:'106'
	// <0x63453b>|0x0db|+0x006:'106'
	// <0x634541>|0x0e1|+0x006:'106'
	// <0x634547>|0x0e7|+0x006:'106'
	// <0x63454d>|0x0ed|+0x006:'106'
	// <0x634553>|0x0f3|+0x006:'106'
	// <0x634559>|0x0f9|+0x006:'106'
	// <0x63455f>|0x0ff|+0x006:'106'
	// <0x634565>|0x105|+0x006:'106'
	// <0x63456b>|0x10b|+0x006:'106'
	// <0x634571>|0x111|+0x006:'106'
	// <0x634577>|0x117|+0x006:'106'
	// <0x63457d>|0x11d|+0x006:'106'
	// <0x634583>|0x123|+0x006:'106'
	// <0x634589>|0x129|+0x006:'106'
	// <0x63458f>|0x12f|+0x006:'106'
	// <0x634595>|0x135|+0x006:'106'
	// <0x63459b>|0x13b|+0x006:'106'
	// <0x6345a1>|0x141|+0x006:'106'
	// <0x6345a7>|0x147|+0x006:'106'
	// <0x6345ad>|0x14d|+0x006:'106'
	// <0x6345b3>|0x153|+0x006:'106'
	// <0x6345b9>|0x159|+0x006:'106'
	// <0x6345bf>|0x15f|+0x006:'106'
	// <0x6345c5>|0x165|+0x006:'106'
	// <0x6345cb>|0x16b|+0x006:'106'
	// <0x6345d1>|0x171|+0x006:'106'
	// <0x6345d7>|0x177|+0x006:'106'
	// <0x6345dd>|0x17d|+0x006:'106'
	// <0x6345e3>|0x183|+0x006:'106'
	// <0x6345e9>|0x189|+0x006:'106'
	// <0x6345ef>|0x18f|+0x006:'106'
	// <0x6345f5>|0x195|+0x006:'106'
	// <0x6345fb>|0x19b|+0x006:'106'
	// <0x634601>|0x1a1|+0x006:'106'
	// <0x634607>|0x1a7|+0x006:'106'
	// <0x63460d>|0x1ad|+0x006:'106'
	// <0x634613>|0x1b3|+0x003:'106'
	// ******
}

// STATE[STUB]
renderer_context_targets::renderer_context_targets( math::uint2 size )
{
	// FUNCTION BODY[0x635890]: 3
	// <0x6358a3>|0x013|+0x010:'111'
	// <0x6358b3>|0x023|+0x006:'112'
	// <0x6358b9>|0x029|+0x01b:'113'
	// ******
}

// STATE[STUB]
 renderer_context_targets::~renderer_context_targets( )
{
	// FUNCTION BODY[0x634730]: 1
	// <0>
	// ******
}

// STATE[STUB]
u32 get_format_block_size( DXGI_FORMAT format )
{
	// STATICS
	// static < NoType > 				 = <0x634424>;
	// static < NoType > 				 = <0x63440c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6233ee }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN11") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6233f4 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6233fa }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623400 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x623406 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	return 0;

	// FUNCTION BODY[0x6343e0]: 14
	// <0x6343e0>|0x000|+0x00e:'127'
	// <0>
	// <0x6343ee>|0x00e|+0x006:'129'
	// <0x6343f4>|0x014|+0x006:'130'
	// <0>
	// <1>
	// <2>
	// <0x6343fa>|0x01a|+0x006:'134'
	// <0>
	// <1>
	// <0x634400>|0x020|+0x006:'137'
	// <0x634406>|0x026|-0x013:'138'
	// <0>
	// <1>
	// <0x6343f3>|0x013|+0x006:'141'
	// <0x6343f9>|0x019|+0x006:'141'
	// <0x6343ff>|0x01f|+0x006:'141'
	// <0x634405>|0x025|+0x006:'141'
	// ******
}

// STATE[STUB]
void renderer_context_targets::new_rt(
	enum_render_target_index	index,
	DXGI_FORMAT					in_format,
	const math::uint2			arg_2 /* math::uint2 in_size */,
	enum_rt_usage				usage,
	bool						enabled
)
{
	// FUNCTION BODY[0x634910]: 14
	// <0x634911>|0x001|+0x013:'144'
	// <0>
	// <1>
	// <0x634924>|0x014|+0x024:'147'
	// <0x634948>|0x038|+0x018:'148'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x634960>|0x050|+0x060:'155'
	// <0x6349c0>|0x0b0|+0x04d:'156'
	// <0x634a0d>|0x0fd|+0x019:'157'
	// ******
}

// STATE[STUB]
void renderer_context_targets::new_lt(
	enum_render_target_index	index,
	DXGI_FORMAT					in_format,
	const math::uint2			arg_2 /* math::uint2 in_size */
)
{
	// FUNCTION BODY[0x634830]: 13
	// <0x634838>|0x008|+0x024:'162'
	// <0x63485c>|0x02c|+0x018:'163'
	// <0x634874>|0x044|+0x029:'164'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x63489d>|0x06d|+0x04a:'173'
	// <0x6348e7>|0x0b7|+0x01a:'174'
	// ******
}

// STATE[STUB]
void renderer_context_targets::create_targets( math::uint2 size, bool force_resize )
{
	// LOCALS
	// math::uint2 						size_blur_4x
	// math::uint2 						size_blur_16x
	// math::uint2 						size_blur_1x
	// math::uint2 						size_d4
	// math::uint2 						size_d16
	// math::uint2 						size_blur_8x
	// math::uint2 						size_blur_2x
	// math::uint2 						size_blur_32x
	// ******

	// FUNCTION BODY[0x634a30]: 140
	// <0x634a36>|0x006|+0x03a:'180'
	// <0>
	// <1>
	// <0x634a70>|0x040|+0x042:'183'
	// <0x634ab2>|0x082|-0x037:'183'
	// <0>
	// <0x634a7b>|0x04b|+0x020:'185'
	// <0x634a9b>|0x06b|+0x022:'186'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x634abd>|0x08d|+0x007:'191'
	// <0x634ac4>|0x094|+0x019:'192'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x634add>|0x0ad|+0x02c:'198'
	// <0x634b09>|0x0d9|+0x02c:'199'
	// <0x634b35>|0x105|+0x033:'200'
	// <0x634b68>|0x138|+0x028:'201'
	// <0x634b90>|0x160|+0x02f:'202'
	// <0x634bbf>|0x18f|+0x033:'203'
	// <0x634bf2>|0x1c2|+0x024:'204'
	// <0x634c16>|0x1e6|+0x02a:'205'
	// <0x634c40>|0x210|+0x02b:'206'
	// <0x634c6b>|0x23b|+0x028:'207'
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
	// <0x634c93>|0x263|+0x037:'218'
	// <0x634cca>|0x29a|+0x029:'219'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x634cf3>|0x2c3|+0x029:'227'
	// <0x634d1c>|0x2ec|+0x029:'228'
	// <0x634d45>|0x315|+0x029:'229'
	// <0x634d6e>|0x33e|+0x029:'230'
	// <0x634d97>|0x367|+0x029:'231'
	// <0x634dc0>|0x390|+0x029:'232'
	// <0x634de9>|0x3b9|+0x029:'233'
	// <0x634e12>|0x3e2|+0x029:'234'
	// <0x634e3b>|0x40b|+0x029:'235'
	// <0x634e64>|0x434|+0x029:'236'
	// <0x634e8d>|0x45d|+0x029:'237'
	// <0x634eb6>|0x486|+0x029:'238'
	// <0x634edf>|0x4af|+0x029:'239'
	// <0x634f08>|0x4d8|+0x029:'240'
	// <0x634f31>|0x501|+0x029:'241'
	// <0x634f5a>|0x52a|+0x029:'242'
	// <0x634f83>|0x553|+0x029:'243'
	// <0x634fac>|0x57c|+0x029:'244'
	// <0x634fd5>|0x5a5|+0x029:'245'
	// <0x634ffe>|0x5ce|+0x034:'246'
	// <0x635032>|0x602|+0x035:'247'
	// <0x635067>|0x637|+0x034:'248'
	// <0x63509b>|0x66b|+0x027:'249'
	// <0x6350c2>|0x692|+0x01b:'250'
	// <0x6350dd>|0x6ad|+0x01b:'251'
	// <0x6350f8>|0x6c8|+0x01b:'252'
	// <0x635113>|0x6e3|+0x018:'253'
	// <0x63512b>|0x6fb|+0x01b:'254'
	// <0x635146>|0x716|+0x01b:'255'
	// <0x635161>|0x731|+0x01b:'256'
	// <0x63517c>|0x74c|+0x023:'257'
	// <0x63519f>|0x76f|+0x01b:'258'
	// <0x6351ba>|0x78a|+0x01b:'259'
	// <0x6351d5>|0x7a5|+0x01b:'260'
	// <0x6351f0>|0x7c0|+0x01c:'261'
	// <0x63520c>|0x7dc|+0x01f:'262'
	// <0x63522b>|0x7fb|+0x01a:'263'
	// <0x635245>|0x815|+0x01b:'264'
	// <0x635260>|0x830|+0x017:'265'
	// <0x635277>|0x847|+0x017:'266'
	// <0x63528e>|0x85e|+0x017:'267'
	// <0x6352a5>|0x875|+0x017:'268'
	// <0x6352bc>|0x88c|+0x01f:'269'
	// <0x6352db>|0x8ab|+0x017:'270'
	// <0x6352f2>|0x8c2|+0x01f:'271'
	// <0x635311>|0x8e1|+0x017:'272'
	// <0x635328>|0x8f8|+0x01f:'273'
	// <0x635347>|0x917|+0x017:'274'
	// <0x63535e>|0x92e|+0x01f:'275'
	// <0x63537d>|0x94d|+0x017:'276'
	// <0x635394>|0x964|+0x01f:'277'
	// <0x6353b3>|0x983|+0x017:'278'
	// <0x6353ca>|0x99a|+0x017:'279'
	// <0x6353e1>|0x9b1|+0x01f:'280'
	// <0x635400>|0x9d0|+0x01f:'281'
	// <0x63541f>|0x9ef|+0x01f:'282'
	// <0x63543e>|0xa0e|+0x01f:'283'
	// <0x63545d>|0xa2d|+0x01f:'284'
	// <0x63547c>|0xa4c|+0x01f:'285'
	// <0x63549b>|0xa6b|+0x01c:'286'
	// <0x6354b7>|0xa87|+0x01f:'287'
	// <0x6354d6>|0xaa6|+0x017:'288'
	// <0x6354ed>|0xabd|+0x017:'289'
	// <0x635504>|0xad4|+0x01b:'290'
	// <0x63551f>|0xaef|+0x017:'291'
	// <0x635536>|0xb06|+0x017:'292'
	// <0x63554d>|0xb1d|+0x01b:'293'
	// <0x635568>|0xb38|+0x01b:'294'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x635583>|0xb53|+0x0d5:'304'
	// <0>
	// <0x635658>|0xc28|+0x06a:'306'
	// <0x6356c2>|0xc92|+0x021:'307'
	// <0>
	// <0x6356e3>|0xcb3|+0x06a:'309'
	// <0x63574d>|0xd1d|+0x021:'310'
	// <0>
	// <0x63576e>|0xd3e|+0x00e:'312'
	// <0>
	// <0x63577c>|0xd4c|+0x05c:'314'
	// <0x6357d8>|0xda8|+0x01d:'315'
	// <0>
	// <0x6357f5>|0xdc5|+0x06a:'317'
	// <0x63585f>|0xe2f|+0x018:'318'
	// <0>
	// ******
}

// STATE[STUB]
void renderer_context_targets::resize( math::uint2 size, bool force_resize )
{
	// FUNCTION BODY[0x6358e0]: 1
	// <0x6358e1>|0x001|+0x021:'324'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
