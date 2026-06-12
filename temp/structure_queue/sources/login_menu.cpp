////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\login_menu.h"

namespace survarium {

// STATE[STUB]
explicit login_menu::login_menu( game& g )
{
	// FUNCTION BODY[0x702e00]: 1
	// <0x702e37>|0x037|+0x00c:'28'
	// ******
}

// STATE[STUB]
 login_menu::~login_menu( )
{
	// FUNCTION BODY[0x702be0]: 0
	// <0x702be0>|0x000|+0x01a:'32'	{
	// <0x702bfa>|0x01a|      :'33'	}
	// ******
}

// STATE[STUB]
void login_menu::on_activate( )
{
	// CALL SITE INFO
	// <0x702892> -> input::world& < unknown >()
	// <0x70289c> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x702870]: 2
	// <0x702874>|0x004|+0x005:'37'
	// <0x702879>|0x009|+0x027:'38'
	// ******
}

// STATE[STUB]
void login_menu::on_deactivate( )
{
	// CALL SITE INFO
	// <0x70285b> -> input::world& < unknown >()
	// <0x702865> -> void < unknown >( input::handler& )
	// ******

	// FUNCTION BODY[0x702830]: 2
	// <0x702833>|0x003|+0x017:'43'
	// <0x70284a>|0x01a|+0x01f:'44'
	// ******
}

// STATE[STUB]
void login_menu::tick(
	const u32		frame_delta_in_ms,
	const u32		current_time_in_ms,
	const bool		is_game_paused
)
{
	// LOCALS
	// float 							deltaTime
	// ******

	// FUNCTION BODY[0x702920]: 10
	// <0x702923>|0x003|+0x03c:'49'
	// <0>
	// <0x70295f>|0x03f|+0x00e:'51'
	// <0>
	// <1>
	// <0x70296d>|0x04d|+0x013:'54'
	// <0>
	// <0x702980>|0x060|+0x012:'56'
	// <0x702992>|0x072|+0x028:'57'
	// <0x7029ba>|0x09a|+0x028:'58'
	// ******
}

// STATE[STUB]
void login_menu::query_resources( )
{
	// LOCALS
	// variant< 32 > const*[4] 			data
	// variant< 32 > 					temp_data
	// resources::request[4] 			requests
	// render::scene_configuration 		render_configuration
	// ******

	// FUNCTION BODY[0x702cf0]: 24
	// <0x702cfa>|0x00a|+0x004:'63'
	// <0>
	// <0x702cfe>|0x00e|+0x00c:'65'
	// <0x702d0a>|0x01a|+0x005:'66'
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
	// <0x702d0f>|0x01f|+0x0d0:'86'
	// ******
}

// STATE[STUB]
void login_menu::clear_resources( )
{
	// FUNCTION BODY[0x702c60]: 2
	// <0x702c63>|0x003|+0x042:'91'
	// <0x702ca5>|0x045|+0x042:'92'
	// ******
}

// STATE[STUB]
void login_menu::set_status( login_menu_status_enum status )
{
	// LOCALS
	// fixed_string< 512 > 				status_str
	// ******

	// STATICS
	// static < NoType > 				 = <0x702bb0>;
	// ******

	// CALL SITE INFO
	// <0x702a12> -> network::login_client& < unknown >()
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1a53 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1a5d }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1a88 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1ab2 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1ada }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1b02 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1b09 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1b10 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6f1b37 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	// FUNCTION BODY[0x7029f0]: 69
	// <0x7029fa>|0x00a|+0x006:'97'
	// <0>
	// <0x702a00>|0x010|+0x014:'99'
	// <0>
	// <0x702a14>|0x024|+0x00c:'101'
	// <0>
	// <0x702a20>|0x030|+0x023:'103'
	// <0>
	// <0x702a43>|0x053|+0x010:'105'
	// <0>
	// <1>
	// <2>
	// <0x702a53>|0x063|+0x005:'109'
	// <0>
	// <0x702a58>|0x068|+0x005:'111'
	// <0>
	// <1>
	// <0x702a5d>|0x06d|+0x00e:'114'
	// <0>
	// <0x702a6b>|0x07b|+0x018:'116'
	// <0x702a83>|0x093|+0x005:'117'
	// <0>
	// <1>
	// <0x702a88>|0x098|+0x00e:'120'
	// <0>
	// <0x702a96>|0x0a6|+0x017:'122'
	// <0x702aad>|0x0bd|+0x005:'123'
	// <0>
	// <1>
	// <0x702ab2>|0x0c2|+0x00e:'126'
	// <0>
	// <0x702ac0>|0x0d0|+0x018:'128'
	// <0x702ad8>|0x0e8|+0x002:'129'
	// <0>
	// <1>
	// <0x702ada>|0x0ea|+0x00e:'132'
	// <0>
	// <0x702ae8>|0x0f8|+0x018:'134'
	// <0x702b00>|0x110|+0x002:'135'
	// <0>
	// <1>
	// <0x702b02>|0x112|+0x005:'138'
	// <0>
	// <0x702b07>|0x117|+0x002:'140'
	// <0>
	// <1>
	// <0x702b09>|0x119|+0x005:'143'
	// <0>
	// <0x702b0e>|0x11e|+0x002:'145'
	// <0>
	// <1>
	// <0x702b10>|0x120|+0x00e:'148'
	// <0>
	// <0x702b1e>|0x12e|+0x017:'150'
	// <0x702b35>|0x145|+0x002:'151'
	// <0>
	// <1>
	// <0x702b37>|0x147|+0x00e:'154'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x702b45>|0x155|+0x009:'161'
	// <0>
	// <0x702b4e>|0x15e|+0x01b:'163'
	// <0x702b69>|0x179|+0x01e:'164'
	// <0x702b87>|0x197|+0x01e:'165'
	// ******
}

// STATE[STUB]
void login_menu::enable_button( bool value )
{
	// LOCALS
	// flash_value 						sign_in_button_enable
	// ******

	// FUNCTION BODY[0x7028b0]: 3
	// <0>
	// <0x7028b9>|0x009|+0x004:'171'
	// <0x7028bd>|0x00d|+0x030:'172'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	char[32]
	// 	account_name_type;

	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::inventory_item_instance*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// ******

} // namespace survarium
