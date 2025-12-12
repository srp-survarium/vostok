////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/items_cook.h>

namespace survarium {

// STATE[STUB]
items_cook::items_cook( ) : resources::translate_query_cook( resources::item_class, reuse_false, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x7616a5>|0x035|+0x00c:'19'
	// ******
}

// STATE[STUB]
void items_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 	config_name
	// ******

	// FUNCTION BODY
	// <0x761b2f>|0x00f|+0x00b:'24'
	// <0x761b3a>|0x01a|+0x01d:'25'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x761b57>|0x037|+0x0e3:'34'
	// ******
}

// STATE[STUB]
void items_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY
	// <0x7616c9>|0x009|+0x017:'39'
	// ******
}

// STATE[STUB]
void items_cook::on_config_ready( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	// LOCALS
	// item_types_enum 				item_type
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> config
	// configs::binary_config_value current
	// ******

	// FUNCTION BODY
	// <0x761a6a>|0x00a|+0x00c:'44'
	// <0x761a76>|0x016|+0x022:'45'
	// <0x761a98>|0x038|+0x03c:'46'
	// <0x761ad4>|0x074|+0x015:'47'
	// <0>
	// <0x761ae9>|0x089|+0x01c:'49'
	// ******
}

// STATE[STUB]
void items_cook::create_item_and_finish_query( item_types_enum item_type, configs::binary_config_ptr config, resources::query_result_for_cook* parent )
{
	// LOCALS
	// resources::cook_base::result_enum result
	// u16 							item_dict_id
	// medkit* 						resource<1>
	// oxygen_tank* 				resource<1>
	// artefact_lifebone_core* 		resource<1>
	// ******

	// STATICS
	// static <NoType> 				 = <0x761a4c>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x750742 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x750832 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x750913 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x75092b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x750a27 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY
	// <0x7616fc>|0x00c|+0x007:'54'
	// <0>
	// <0x761703>|0x013|+0x006:'56'
	// <0x761709>|0x019|+0x00c:'57'
	// <0x761715>|0x025|+0x013:'58'
	// <0>
	// <0x761728>|0x038|+0x01a:'60'
	// <0>
	// <1>
	// <2>
	// <0x761742>|0x052|+0x051|[1]:'64'
	// <0x761793>|0x0a3|+0x048:'65'
	// <0x7617db>|0x0eb|+0x008:'66'
	// <0x7617e3>|0x0f3|+0x016:'67'
	// <0>
	// <0x7617f9>|0x109|+0x02d:'69'
	// <0x761826>|0x136|+0x007:'70'
	// <0x76182d>|0x13d|+0x005:'71'
	// <0>
	// <1>
	// <2>
	// <0x761832>|0x142|+0x05a|[1]:'75'
	// <0x76188c>|0x19c|+0x048:'76'
	// <0x7618d4>|0x1e4|+0x033:'77'
	// <0x761907>|0x217|+0x007:'78'
	// <0x76190e>|0x21e|+0x005:'79'
	// <0>
	// <1>
	// <2>
	// <0x761913>|0x223|+0x00c:'83'
	// <0x76191f>|0x22f|+0x007:'84'
	// <0x761926>|0x236|+0x005:'85'
	// <0>
	// <1>
	// <0x76192b>|0x23b|+0x05a|[1]:'88'
	// <0x761985>|0x295|+0x048:'89'
	// <0x7619cd>|0x2dd|+0x008:'90'
	// <0x7619d5>|0x2e5|+0x016:'91'
	// <0>
	// <0x7619eb>|0x2fb|+0x033:'93'
	// <0x761a1e>|0x32e|+0x007:'94'
	// <0x761a25>|0x335|+0x002:'95'
	// <0>
	// <1>
	// <0x761a27>|0x337|+0x007:'98'
	// <0>
	// <1>
	// <0x761a2e>|0x33e|+0x00e:'101'
	// <0>
	// ******
}

} // namespace survarium
