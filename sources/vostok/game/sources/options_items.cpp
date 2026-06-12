////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "options_gamma_selector.h"
#include "options_graphics_quality_selector.h"
#include "options_item_base.h"
#include "options_item_bool.h"
#include "options_item_float.h"
#include "options_item_int.h"
#include "options_monitor_index_selector.h"
#include "options_resolution_selector.h"
#include "options_tab.h"

namespace survarium {

// STATE[STUB]
 options_item_base::options_item_base(
	options_tab&				parent_tab,
	pcstr						console_command,
	u8							option_item_id,
	option_item_type_enum		type
)
{
	// FUNCTION BODY[0x5cad50]: 3
	// <0x5cadb0>|0x060|+0x008:'109'
	// <0x5cadb8>|0x068|+0x008:'110'
	// <0x5cadc0>|0x070|+0x0c1:'111'
	// ******
}

// STATE[STUB]
void options_item_base::revert( )
{
	// LOCALS
	// flash_value[4] 					source_data
	// ******

	// CALL SITE INFO
	// <0x5ca45b> -> void < unknown >( flash_value& )
	// ******

	// FUNCTION BODY[0x5ca3c0]: 7
	// <0x5ca3c8>|0x008|+0x01c:'116'
	// <0x5ca3e4>|0x024|+0x02b:'117'
	// <0x5ca40f>|0x04f|+0x034:'118'
	// <0x5ca443>|0x083|+0x01a:'119'
	// <0x5ca45d>|0x09d|+0x024:'120'
	// <0>
	// <0x5ca481>|0x0c1|+0x033:'122'
	// ******
}

// STATE[STUB]
 options_item_int::options_item_int(
	options_tab&	parent_tab,
	pcstr			console_command,
	u8				option_item_id,
	pcstr*			values,
	u8				values_count
)
{
	// FUNCTION BODY[0x5cb0a0]: 2
	// <0>
	// <0x5cb0b2>|0x012|+0x014:'130'
	// ******
}

// STATE[STUB]
void options_item_int::initialize( )
{
	// FUNCTION BODY[0x5c9e10]: 8
	// <0x5c9e10>|0x000|+0x000:'134'	{
	// <0x5c9e10>|0x000|+0x007:'135'
	// <0>
	// <0x5c9e17>|0x007|+0x00e:'137'
	// <0>
	// <1>
	// <0x5c9e25>|0x015|-0x009:'140'
	// <0>
	// <0x5c9e1c>|0x00c|+0x00d:'142'
	// <0x5c9e29>|0x019|-0x005:'142'
	// <0x5c9e24>|0x014|+0x00b:'143'
	// <0x5c9e2f>|0x01f|      :'143'	}
	// ******
}

// STATE[STUB]
void options_item_int::fill_data( flash_value& val )
{
	// LOCALS
	// u8 								i
	// flash_value 						str_val
	// wchar_t[512] 					val_txt
	// ******

	// FUNCTION BODY[0x5caf90]: 13
	// <0x5caf9f>|0x00f|+0x011:'147'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5cafb0>|0x020|+0x02a:'155'
	// <0x5cafda>|0x04a|+0x06c:'156'
	// <0>
	// <0x5cb046>|0x0b6|+0x016:'158'
	// <0x5cb05c>|0x0cc|+0x031:'159'
	// ******
}

// STATE[STUB]
void options_item_int::fill_value( flash_value& val )
{
	// FUNCTION BODY[0x5c9dd0]: 1
	// <0x5c9dd1>|0x001|+0x033:'164'
	// ******
}

// STATE[STUB]
void options_item_int::apply( )
{
	// LOCALS
	// fixed_string< 8 > 				args
	// ******

	// CALL SITE INFO
	// <0x5ca860> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5ca810]: 7
	// <0>
	// <1>
	// <0x5ca816>|0x006|+0x00c:'170'
	// <0x5ca822>|0x012|+0x00a:'171'
	// <0x5ca82c>|0x01c|+0x024:'172'
	// <0x5ca850>|0x040|+0x013:'173'
	// <0>
	// ******
}

// STATE[STUB]
void options_item_int::revert( )
{
	// FUNCTION BODY[0x5ca800]: 2
	// <0x5ca800>|0x000|+0x006:'178'
	// <0>
	// ******
}

// STATE[STUB]
void options_item_int::call( flash_function_handler_params& params )
{
	// LOCALS
	// flash_value[4] 					new_resolution_data
	// ******

	// FUNCTION BODY[0x5ca1f0]: 34
	// <0x5ca1f0>|0x000|+0x011:'185'
	// <0>
	// <0x5ca201>|0x011|+0x00d:'187'
	// <0>
	// <1>
	// <0x5ca20e>|0x01e|+0x008:'190'
	// <0>
	// <0x5ca216>|0x026|+0x004:'192'
	// <0>
	// <0x5ca21a>|0x02a|+0x037:'194'
	// <0x5ca251>|0x061|-0x021:'194'
	// <0>
	// <0x5ca230>|0x040|+0x005:'196'
	// <0x5ca235>|0x045|+0x06e:'197'
	// <0>
	// <0x5ca2a3>|0x0b3|+0x017:'199'
	// <0x5ca2ba>|0x0ca|+0x02a:'200'
	// <0x5ca2e4>|0x0f4|+0x030:'201'
	// <0x5ca314>|0x124|+0x030:'202'
	// <0x5ca344>|0x154|+0x030:'203'
	// <0>
	// <0x5ca374>|0x184|-0x11c:'205'
	// <0x5ca258>|0x068|+0x146:'206'
	// <0x5ca39e>|0x1ae|-0x144:'206'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5ca25a>|0x06a|+0x00c:'213'
	// <0x5ca266>|0x076|+0x005:'214'
	// <0>
	// <1>
	// <2>
	// <0x5ca26b>|0x07b|+0x032:'218'
	// ******
}

// STATE[STUB]
 options_item_float::options_item_float(
	options_tab&	parent_tab,
	pcstr			console_command,
	u8				option_item_id,
	float			step
)
{
	// FUNCTION BODY[0x5caf60]: 1
	// <0x5caf60>|0x000|+0x024:'225'	{
	// <0>
	// <0x5caf84>|0x024|      :'227'	}
	// ******
}

// STATE[STUB]
void options_item_float::initialize( )
{
	// FUNCTION BODY[0x5c9da0]: 6
	// <0x5c9da0>|0x000|+0x000:'230'	{
	// <0x5c9da0>|0x000|+0x007:'231'
	// <0x5c9da7>|0x007|+0x00f:'232'
	// <0>
	// <0x5c9db6>|0x016|-0x007:'234'
	// <0>
	// <0x5c9daf>|0x00f|+0x00f:'236'
	// <0x5c9dbe>|0x01e|-0x009:'236'
	// <0x5c9db5>|0x015|+0x00f:'237'
	// <0x5c9dc4>|0x024|      :'237'	}
	// ******
}

// STATE[STUB]
void options_item_float::fill_data( flash_value& val )
{
	// LOCALS
	// flash_value 						slider_data_member
	// ******

	// FUNCTION BODY[0x5ca070]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5ca079>|0x009|+0x006:'245'
	// <0x5ca07f>|0x00f|+0x04a:'246'
	// <0>
	// <0x5ca0c9>|0x059|+0x03d:'248'
	// <0x5ca106>|0x096|+0x03e:'249'
	// <0>
	// <0x5ca144>|0x0d4|+0x042:'251'
	// <0x5ca186>|0x116|+0x03e:'252'
	// ******
}

// STATE[STUB]
void options_item_float::fill_value( flash_value& val )
{
	// FUNCTION BODY[0x5c9d50]: 1
	// <0x5c9d57>|0x007|+0x03f:'257'
	// ******
}

// STATE[STUB]
void options_item_float::apply( )
{
	// LOCALS
	// fixed_string< 8 > 				args
	// ******

	// CALL SITE INFO
	// <0x5ca7eb> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5ca790]: 7
	// <0>
	// <1>
	// <0x5ca796>|0x006|+0x010:'263'
	// <0x5ca7a6>|0x016|+0x004:'264'
	// <0x5ca7aa>|0x01a|+0x031:'265'
	// <0x5ca7db>|0x04b|+0x013:'266'
	// <0>
	// ******
}

// STATE[STUB]
void options_item_float::revert( )
{
	// FUNCTION BODY[0x5ca780]: 2
	// <0x5ca780>|0x000|+0x006:'271'
	// <0>
	// ******
}

// STATE[STUB]
void options_item_float::call( flash_function_handler_params& params )
{
	// FUNCTION BODY[0x5c9cf0]: 5
	// <0x5c9cf7>|0x007|+0x00c:'278'
	// <0>
	// <1>
	// <2>
	// <0x5c9d03>|0x013|+0x041:'282'
	// ******
}

// STATE[STUB]
 options_item_bool::options_item_bool( options_tab& parent_tab, pcstr console_command, u8 option_item_id )
{
	// FUNCTION BODY[0x5caf40]: 0
	// <0x5caf40>|0x000|+0x018:'289'	{
	// <0x5caf58>|0x018|      :'290'	}
	// ******
}

// STATE[STUB]
void options_item_bool::initialize( )
{
	// FUNCTION BODY[0x5c9cd0]: 6
	// <0x5c9cd0>|0x000|+0x000:'293'	{
	// <0x5c9cd0>|0x000|+0x007:'294'
	// <0x5c9cd7>|0x007|+0x00e:'295'
	// <0>
	// <0x5c9ce5>|0x015|-0x009:'297'
	// <0>
	// <0x5c9cdc>|0x00c|+0x00d:'299'
	// <0x5c9ce9>|0x019|-0x005:'299'
	// <0x5c9ce4>|0x014|+0x00b:'300'
	// <0x5c9cef>|0x01f|      :'300'	}
	// ******
}

// STATE[STUB]
void options_item_bool::fill_data( flash_value& __formal )
{
	// FUNCTION BODY[0x5c9c40]: 2
	// <0x5c9c40>|0x000|+0x000:'303'	{
	// <0>
	// <1>
	// <0x5c9c40>|0x000|      :'306'	}
	// ******
}

// STATE[STUB]
void options_item_bool::fill_value( flash_value& val )
{
	// FUNCTION BODY[0x5c9c90]: 1
	// <0x5c9c91>|0x001|+0x032:'310'
	// ******
}

// STATE[STUB]
void options_item_bool::apply( )
{
	// LOCALS
	// fixed_string< 8 > 				args
	// ******

	// CALL SITE INFO
	// <0x5ca770> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5ca720]: 7
	// <0>
	// <1>
	// <0x5ca726>|0x006|+0x00c:'316'
	// <0x5ca732>|0x012|+0x00a:'317'
	// <0x5ca73c>|0x01c|+0x024:'318'
	// <0x5ca760>|0x040|+0x013:'319'
	// <0>
	// ******
}

// STATE[STUB]
void options_item_bool::revert( )
{
	// FUNCTION BODY[0x5ca710]: 2
	// <0x5ca710>|0x000|+0x006:'324'
	// <0>
	// ******
}

// STATE[STUB]
void options_item_bool::call( flash_function_handler_params& params )
{
	// FUNCTION BODY[0x5c9c50]: 5
	// <0x5c9c50>|0x000|+0x00f:'331'
	// <0>
	// <1>
	// <2>
	// <0x5c9c5f>|0x00f|+0x02c:'335'
	// ******
}

// STATE[STUB]
void options_gamma_selector::call( flash_function_handler_params& params )
{
	// FUNCTION BODY[0x5ca950]: 3
	// <0x5ca959>|0x009|+0x051:'341'
	// <0>
	// <0x5ca9aa>|0x05a|+0x028:'343'
	// ******
}

// STATE[STUB]
void options_gamma_selector::revert( )
{
	// FUNCTION BODY[0x5ca910]: 2
	// <0x5ca913>|0x003|+0x00b:'348'
	// <0x5ca91e>|0x00e|+0x029:'349'
	// ******
}

// STATE[STUB]
 options_resolution_selector::options_resolution_selector( options_tab& parent_tab )
{
	// FUNCTION BODY[0x5cb1e0]: 5
	// <0x5cb217>|0x037|+0x00a:'355'
	// <0>
	// <0x5cb221>|0x041|+0x003:'357'
	// <0>
	// <0x5cb224>|0x044|+0x00a:'359'
	// ******
}

// STATE[STUB]
void options_resolution_selector::fill_resolutions( u8 monitor_number )
{
	// LOCALS
	// fixed_string< 32 > 				old_resolution
	// u32 								old_resolution_index
	// flash_value[4] 					new_resolution_data
	// ******

	// FUNCTION BODY[0x5ca9e0]: 41
	// <0x5ca9e0>|0x000|+0x00e:'368'	{
	// <0x5ca9ee>|0x00e|+0x005:'369'
	// <0>
	// <0x5ca9f3>|0x013|+0x029:'371'
	// <0x5caa1c>|0x03c|+0x03f:'372'
	// <0x5caa5b>|0x07b|+0x022:'373'
	// <0>
	// <1>
	// <0x5caa7d>|0x09d|+0x017:'376'
	// <0>
	// <1>
	// <0x5caa94>|0x0b4|+0x003:'379'
	// <0x5caa97>|0x0b7|+0x012:'380'
	// <0>
	// <1>
	// <0x5caaa9>|0x0c9|+0x026:'383'
	// <0>
	// <1>
	// <0x5caacf>|0x0ef|+0x064:'386'
	// <0x5cab33>|0x153|-0x017:'386'
	// <0>
	// <1>
	// <0x5cab1c>|0x13c|+0x061:'389'
	// <0x5cab7d>|0x19d|-0x047:'389'
	// <0>
	// <0x5cab36>|0x156|+0x009:'391'
	// <0>
	// <0x5cab3f>|0x15f|+0x03a:'393'
	// <0x5cab79>|0x199|+0x00e:'394'
	// <0>
	// <0x5cab87>|0x1a7|+0x03a:'396'
	// <0x5cabc1>|0x1e1|+0x00d:'397'
	// <0x5cabce>|0x1ee|+0x00b:'398'
	// <0x5cabd9>|0x1f9|+0x01a:'399'
	// <0x5cabf3>|0x213|+0x025:'400'
	// <0x5cac18>|0x238|+0x036:'401'
	// <0x5cac4e>|0x26e|+0x038:'402'
	// <0x5cac86>|0x2a6|+0x03f:'403'
	// <0>
	// <0x5cacc5>|0x2e5|+0x035:'405'
	// <0x5cacfa>|0x31a|+0x043:'406'
	// <0>
	// <0x5cad3d>|0x35d|-0x009:'408'
	// <0>
	// <0x5cad34>|0x354|+0x00f:'410'
	// <0x5cad43>|0x363|      :'410'	}
	// ******
}

// STATE[STUB]
void options_resolution_selector::initialize( )
{
	// FUNCTION BODY[0x5ca6a0]: 10
	// <0x5ca6a0>|0x000|+0x003:'413'	{
	// <0x5ca6a3>|0x003|+0x003:'414'
	// <0>
	// <0x5ca6a6>|0x006|+0x010:'416'
	// <0>
	// <0x5ca6b6>|0x016|+0x048:'418'
	// <0x5ca6fe>|0x05e|-0x00e:'419'
	// <0>
	// <1>
	// <2>
	// <0x5ca6f0>|0x050|+0x011:'423'
	// <0x5ca701>|0x061|+0x005:'423'
	// <0x5ca706>|0x066|-0x00c:'423'
	// <0x5ca6fa>|0x05a|+0x00b:'424'
	// <0x5ca705>|0x065|+0x009:'424'
	// <0x5ca70e>|0x06e|      :'424'	}
	// ******
}

// STATE[STUB]
void options_resolution_selector::apply( )
{
	// CALL SITE INFO
	// <0x5c9c3b> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5c9c20]: 3
	// <0x5c9c22>|0x002|+0x003:'428'
	// <0>
	// <0x5c9c25>|0x005|+0x019:'430'
	// ******
}

// STATE[STUB]
 options_monitor_index_selector::options_monitor_index_selector( options_tab& parent_tab )
{
	// FUNCTION BODY[0x5cb100]: 12
	// <0>
	// <1>
	// <0x5cb140>|0x040|+0x018:'438'
	// <0>
	// <1>
	// <0x5cb158>|0x058|+0x04c:'441'
	// <0x5cb1a4>|0x0a4|+0x009:'442'
	// <0>
	// <0x5cb1ad>|0x0ad|+0x00b:'444'
	// <0>
	// <0x5cb1b8>|0x0b8|+0x018:'446'
	// <0>
	// ******
}

// STATE[STUB]
void options_monitor_index_selector::call( flash_function_handler_params& params )
{
	// FUNCTION BODY[0x5caf00]: 2
	// <0x5caf00>|0x000|+0x00d:'452'
	// <0x5caf0d>|0x00d|+0x024:'453'
	// ******
}

// STATE[STUB]
void options_monitor_index_selector::revert( )
{
	// FUNCTION BODY[0x5caec0]: 1
	// <0x5caec3>|0x003|+0x00b:'458'
	// ******
}

// STATE[STUB]
void options_monitor_index_selector::refill_resolutions_data( )
{
	// FUNCTION BODY[0x5cae90]: 4
	// <0>
	// <1>
	// <0x5cae90>|0x000|+0x013:'466'
	// <0>
	// ******
}

// STATE[STUB]
 options_graphics_quality_selector::options_graphics_quality_selector( options_tab& parent_tab )
{
	// FUNCTION BODY[0x5cb0d0]: 1
	// <0x5cb0d0>|0x000|+0x023:'480'	{
	// <0>
	// <0x5cb0f3>|0x023|      :'482'	}
	// ******
}

// STATE[STUB]
void options_graphics_quality_selector::call( flash_function_handler_params& params )
{
	// LOCALS
	// flash_value[4] 					new_resolution_data
	// ******

	// FUNCTION BODY[0x5ca4f0]: 17
	// <0x5ca4f0>|0x000|+0x010:'486'
	// <0>
	// <0x5ca500>|0x010|+0x013:'488'
	// <0>
	// <1>
	// <0x5ca513>|0x023|+0x00f:'491'
	// <0>
	// <0x5ca522>|0x032|+0x015:'493'
	// <0>
	// <0x5ca537>|0x047|+0x01a:'495'
	// <0x5ca551>|0x061|+0x025:'496'
	// <0x5ca576>|0x086|+0x037:'497'
	// <0x5ca5ad>|0x0bd|+0x034:'498'
	// <0x5ca5e1>|0x0f1|+0x030:'499'
	// <0>
	// <0x5ca611>|0x121|+0x02f:'501'
	// <0x5ca640>|0x150|+0x04d:'502'
	// ******
}

// STATE[STUB]
 options_tab::options_tab( game& g, flash_movie_resource_ptr& movie, options_enum type )
{
	// STATICS
	// static < NoType > 				 = <0x5cba78>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba264 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba44b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba80b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba8ac }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5cb240]: 66
	// <0x5cb240>|0x000|+0x014:'509'	{
	// <0x5cb254>|0x014|+0x010:'510'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cb264>|0x024|+0x01e:'515'
	// <0>
	// <0x5cb282>|0x042|+0x032:'517'
	// <0x5cb2b4>|0x074|+0x02e:'518'
	// <0x5cb2e2>|0x0a2|+0x031:'519'
	// <0x5cb313>|0x0d3|+0x034:'520'
	// <0x5cb347>|0x107|+0x02e:'521'
	// <0x5cb375>|0x135|+0x031:'522'
	// <0x5cb3a6>|0x166|+0x030:'523'
	// <0x5cb3d6>|0x196|+0x02e:'524'
	// <0x5cb404>|0x1c4|+0x038:'525'
	// <0x5cb43c>|0x1fc|+0x00f:'525'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cb44b>|0x20b|+0x01f:'530'
	// <0>
	// <0x5cb46a>|0x22a|+0x025:'532'
	// <0x5cb48f>|0x24f|+0x028:'533'
	// <0x5cb4b7>|0x277|+0x02e:'534'
	// <0x5cb4e5>|0x2a5|+0x031:'535'
	// <0x5cb516>|0x2d6|+0x033:'536'
	// <0x5cb549>|0x309|+0x02d:'537'
	// <0x5cb576>|0x336|+0x03e:'538'
	// <0x5cb5b4>|0x374|+0x041:'539'
	// <0x5cb5f5>|0x3b5|+0x020:'540'
	// <0x5cb615>|0x3d5|+0x033:'541'
	// <0x5cb648>|0x408|+0x02d:'542'
	// <0x5cb675>|0x435|+0x033:'543'
	// <0x5cb6a8>|0x468|+0x02d:'544'
	// <0x5cb6d5>|0x495|+0x033:'545'
	// <0x5cb708>|0x4c8|+0x02d:'546'
	// <0x5cb735>|0x4f5|+0x033:'547'
	// <0x5cb768>|0x528|+0x02d:'548'
	// <0x5cb795>|0x555|+0x033:'549'
	// <0x5cb7c8>|0x588|+0x034:'550'
	// <0x5cb7fc>|0x5bc|+0x00f:'550'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cb80b>|0x5cb|+0x01e:'555'
	// <0>
	// <0x5cb829>|0x5e9|+0x032:'557'
	// <0x5cb85b>|0x61b|+0x042:'558'
	// <0x5cb89d>|0x65d|+0x00f:'558'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cb8ac>|0x66c|+0x01e:'565'
	// <0>
	// <0x5cb8ca>|0x68a|+0x03f:'567'
	// <0x5cb909>|0x6c9|+0x03b:'568'
	// <0x5cb944>|0x704|+0x03e:'569'
	// <0x5cb982>|0x742|+0x041:'570'
	// <0x5cb9c3>|0x783|+0x02e:'571'
	// <0x5cb9f1>|0x7b1|+0x03e:'572'
	// <0x5cba2f>|0x7ef|+0x038:'573'
	// <0x5cba67>|0x827|-0x633:'573'
	// <0>
	// <1>
	// <0x5cb434>|0x1f4|+0x00f:'576'
	// <0x5cb443>|0x203|+0x3b1:'576'
	// <0x5cb7f4>|0x5b4|+0x00f:'576'
	// <0x5cb803>|0x5c3|+0x092:'576'
	// <0x5cb895>|0x655|+0x00f:'576'
	// <0x5cb8a4>|0x664|+0x1bb:'576'
	// <0x5cba5f>|0x81f|+0x00f:'576'
	// <0x5cba6e>|0x82e|      :'576'	}
	// ******
}

// STATE[STUB]
 options_tab::~options_tab( )
{
	// FUNCTION BODY[0x5ca870]: 5
	// <0x5ca870>|0x000|+0x064:'580'
	// <0x5ca8d4>|0x064|-0x054:'580'
	// <0>
	// <0x5ca880>|0x010|+0x061:'582'
	// <0>
	// <0x5ca8e1>|0x071|+0x028:'584'
	// ******
}

// STATE[STUB]
void options_tab::apply( flash_movie_resource_ptr& movie )
{
	// CALL SITE INFO
	// <0x5cbaad> -> void < unknown >()
	// <0x5cbb1f> -> void < unknown >( pcstr )
	// ******

	// FUNCTION BODY[0x5cba90]: 16
	// <0x5cba97>|0x007|+0x009:'589'
	// <0>
	// <0x5cbaa0>|0x010|+0x016:'591'
	// <0>
	// <1>
	// <0x5cbab6>|0x026|+0x006:'594'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cbabc>|0x02c|+0x04d:'599'
	// <0>
	// <1>
	// <0x5cbb09>|0x079|+0x018:'602'
	// <0>
	// <0x5cbb21>|0x091|+0x00b:'604'
	// ******
}

// STATE[STUB]
void options_tab::revert( flash_movie_resource_ptr& movie )
{
	// CALL SITE INFO
	// <0x5c9c15> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5c9c00]: 4
	// <0x5c9c01>|0x001|+0x007:'609'
	// <0>
	// <0x5c9c08>|0x008|+0x017:'611'
	// <0>
	// ******
}

// STATE[STUB]
void options_tab::initialize_data( flash_movie_resource_ptr& movie )
{
	// LOCALS
	// flash_value[3] 					options_args
	// u8 								i
	// flash_value 						option_item_member
	// flash_value 						option_item
	// ******

	// CALL SITE INFO
	// <0x5c9efd> -> void < unknown >()
	// <0x5c9f6b> -> void < unknown >( flash_value& )
	// ******

	// FUNCTION BODY[0x5c9e30]: 25
	// <0x5c9e43>|0x013|+0x01e:'616'
	// <0x5c9e61>|0x031|+0x027:'617'
	// <0x5c9e88>|0x058|+0x033:'618'
	// <0>
	// <1>
	// <0x5c9ebb>|0x08b|+0x026:'621'
	// <0>
	// <0x5c9ee1>|0x0b1|+0x00d:'623'
	// <0>
	// <0x5c9eee>|0x0be|+0x011:'625'
	// <0>
	// <1>
	// <0x5c9eff>|0x0cf|+0x021:'628'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c9f20>|0x0f0|+0x03c:'633'
	// <0>
	// <0x5c9f5c>|0x12c|+0x011:'635'
	// <0x5c9f6d>|0x13d|+0x02e:'636'
	// <0>
	// <0x5c9f9b>|0x16b|+0x016:'638'
	// <0x5c9fb1>|0x181|+0x05b:'639'
	// <0x5ca00c>|0x1dc|+0x01e:'640'
	// ******
}

} // namespace survarium
