////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\custom_config.h"

namespace vostok {
namespace render {

// STATE[STUB]
 custom_config::custom_config( )
{
	// FUNCTION BODY[0x55a710]
	// ******
}

// STATE[STUB]
void custom_config::destroy( custom_config* in_this )
{
	// FUNCTION BODY[0x55a910]: 5
	// <0x55a910>|0x000|+0x009:'22'
	// <0x55a919>|0x009|+0x008:'23'
	// <0>
	// <0x55a921>|0x011|+0x006:'25'
	// <0x55a927>|0x017|+0x01a:'26'
	// ******
}

// STATE[STUB]
u32 custom_config::get_need_buffer_size( effect_options_descriptor const& v )
{
	// LOCALS
	// u32 								last_align_value
	// u32 								need_bytes_to_align
	// ******

	return 0;

	// FUNCTION BODY[0x55a100]: 8
	// <0x55a104>|0x004|+0x009:'31'
	// <0x55a10d>|0x00d|+0x006:'32'
	// <0x55a113>|0x013|+0x004:'33'
	// <0>
	// <0x55a117>|0x017|+0x011:'35'
	// <0x55a128>|0x028|+0x00e:'36'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
u16 convert_type<vostok::render::custom_config_value>( u16 type )
{
	return 0;

	// FUNCTION BODY[0x559d00]: 1
	// <0>
	// ******
}

// STATE[STUB]
u16 convert_type<vostok::render::effect_options_descriptor>( u16 type )
{
	return 0;

	// FUNCTION BODY[0x559cf0]: 1
	// <0>
	// ******
}

// STATE[STUB]
u16 convert_type<vostok::configs::binary_config_value>( u16 lua_type )
{
	// STATICS
	// static < NoType > 				 = <0x559d94>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d6a }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN10") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d6f }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d74 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d79 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d7f }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN5") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d84 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d89 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x548d8e }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// ******

	return 0;

	// FUNCTION BODY[0x559d60]: 15
	// <0x559d60>|0x000|+0x00a:'55'
	// <0>
	// <0x559d6a>|0x00a|+0x005:'57'
	// <0x559d6f>|0x00f|+0x005:'58'
	// <0x559d74>|0x014|+0x005:'59'
	// <0x559d79>|0x019|+0x006:'60'
	// <0>
	// <0x559d7f>|0x01f|+0x005:'62'
	// <0x559d84>|0x024|+0x005:'63'
	// <0x559d89>|0x029|+0x005:'64'
	// <0x559d8e>|0x02e|-0x010:'65'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x559d7e>|0x01e|+0x016:'70'
	// ******
}

// STATE[STUB]
void copy_destroyer<vostok::configs::binary_config_value,vostok::render::custom_config_value>( configs::binary_config_value const& __formal, custom_config_value& __formal )
{
	// FUNCTION BODY[0x559c70]
	// ******
}

// STATE[STUB]
void copy_destroyer<vostok::render::custom_config_value,vostok::render::custom_config_value>( custom_config_value const& value, custom_config_value& item )
{
	// FUNCTION BODY[0x559ce0]
	// ******
}

// STATE[STUB]
void copy_destroyer<vostok::render::effect_options_descriptor,vostok::render::custom_config_value>( effect_options_descriptor const& value, custom_config_value& item )
{
	// FUNCTION BODY[0x559cd0]
	// ******
}

// STATE[STUB]
void construct<vostok::render::effect_options_descriptor,vostok::render::custom_config_value>(
	effect_options_descriptor const&	value,
	custom_config_value&				item,
	mutable_buffer&						data_buffer
)
{
	// LOCALS
	// custom_config_value* 			items
	// effect_options_descriptor::const_iterator it
	// custom_config_value* 			next_item
	// boost::crc_optimal< 32, 79764919, 4294967295, 4294967295, 1, 1 > processor
	// ******

	// FUNCTION BODY[0x55a560]: 39
	// <0x55a560>|0x000|+0x014:'79'
	// <0x55a574>|0x014|+0x004:'80'
	// <0>
	// <0x55a578>|0x018|+0x00b:'82'
	// <0x55a583>|0x023|+0x003:'83'
	// <0>
	// <0x55a586>|0x026|+0x006:'85'
	// <0>
	// <0x55a58c>|0x02c|+0x010:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55a59c>|0x03c|+0x009:'92'
	// <0x55a5a5>|0x045|+0x009:'93'
	// <0>
	// <1>
	// <0x55a5ae>|0x04e|+0x017:'96'
	// <0>
	// <0x55a5c5>|0x065|+0x024:'98'
	// <0>
	// <0x55a5e9>|0x089|+0x02c:'100'
	// <0x55a615>|0x0b5|+0x00f:'101'
	// <0>
	// <0x55a624>|0x0c4|+0x006:'103'
	// <0>
	// <0x55a62a>|0x0ca|+0x013:'105'
	// <0x55a63d>|0x0dd|+0x01d:'106'
	// <0x55a65a>|0x0fa|+0x025:'107'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x55a67f>|0x11f|+0x006:'114'
	// <0x55a685>|0x125|+0x009:'115'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void sort_by_crc<vostok::render::custom_config_value>( custom_config_value& item )
{
	// TYPEDEFS
	// typedef
	// 	sort_by_crc::__l5::predicate
	// 	sort_by_crc::__l5::predicate;

	// ******

	// FUNCTION BODY[0x559d10]: 20
	// <0x559d11>|0x001|+0x010:'122'
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
	// <0x559d21>|0x011|+0x016:'134'
	// <0>
	// <0x559d37>|0x027|+0x00a:'136'
	// <0x559d41>|0x031|+0x018:'137'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
bool `vostok::render::sort_by_crc<vostok::configs::binary_config_value>'::`5'::predicate::compare(
	configs::binary_config_value const&		arg_0 /* vostok::configs::binary_config_value const& left */,
	configs::binary_config_value const&		arg_1 /* vostok::configs::binary_config_value const& right */
)
{
	return false;

	// FUNCTION BODY[0x559ca0]: 1
	// <0x559ca0>|0x000|+0x012:'130'
	// ******
}

// STATE[STUB]
void* align4( void* ptr )
{
	return NULL;

	// FUNCTION BODY[0x559cc0]: 1
	// <0x559cc0>|0x000|+0x006:'146'
	// ******
}

// STATE[STUB]
void fill_data<vostok::render::custom_config_value>( custom_config_value const& value, mutable_buffer& data_buffer )
{
	// FUNCTION BODY[0x55a000]: 29
	// <0x55a003>|0x003|+0x0c8:'151'
	// <0>
	// <0x55a0cb>|0x0cb|-0x003:'153'
	// <0x55a0c8>|0x0c8|-0x0a9:'154'
	// <0>
	// <1>
	// <2>
	// <0x55a01f>|0x01f|+0x006:'158'
	// <0x55a025>|0x025|+0x008:'159'
	// <0x55a02d>|0x02d|+0x002:'160'
	// <0>
	// <0x55a02f>|0x02f|+0x009:'162'
	// <0x55a038>|0x038|+0x008:'163'
	// <0>
	// <1>
	// <0x55a040>|0x040|+0x012:'166'
	// <0>
	// <0x55a052>|0x052|+0x011:'168'
	// <0x55a063>|0x063|+0x005:'169'
	// <0x55a068>|0x068|+0x00c:'170'
	// <0>
	// <1>
	// <0x55a074>|0x074|+0x006:'173'
	// <0>
	// <0x55a07a>|0x07a|+0x012:'175'
	// <0x55a08c>|0x08c|+0x00a:'176'
	// <0x55a096>|0x096|+0x004:'177'
	// <0x55a09a>|0x09a|+0x00b:'178'
	// <0>
	// ******
}

// STATE[STUB]
void fill_data_crc_buffer<vostok::render::custom_config_value>( custom_config_value const& value, mutable_buffer& buffer )
{
	// FUNCTION BODY[0x559e10]: 26
	// <0x559e11>|0x001|+0x011:'184'
	// <0>
	// <0x559e22>|0x012|+0x011:'186'
	// <0x559e33>|0x023|+0x00a:'187'
	// <0x559e3d>|0x02d|+0x008:'188'
	// <0>
	// <1>
	// <0x559e45>|0x035|+0x007:'191'
	// <0>
	// <0x559e4c>|0x03c|+0x015:'193'
	// <0>
	// <0x559e61>|0x051|+0x00f:'195'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x559e70>|0x060|+0x00c:'202'
	// <0x559e7c>|0x06c|+0x009:'203'
	// <0>
	// <0x559e85>|0x075|+0x003:'205'
	// <0>
	// <0x559e88>|0x078|+0x00e:'207'
	// <0x559e96>|0x086|+0x010:'208'
	// <0>
	// ******
}

// STATE[STUB]
u32 get_data_crc_buffer_size<vostok::render::custom_config_value>( custom_config_value const& value )
{
	return 0;

	// FUNCTION BODY[0x559eb0]: 16
	// <0>
	// <1>
	// <0x559eb1>|0x001|+0x00d:'216'
	// <0x559ebe>|0x00e|+0x00f:'217'
	// <0>
	// <0x559ecd>|0x01d|+0x01c:'219'
	// <0>
	// <0x559ee9>|0x039|+0x018:'221'
	// <0x559f01>|0x051|-0x024:'222'
	// <0>
	// <1>
	// <2>
	// <0x559edd>|0x02d|+0x006:'226'
	// <0>
	// <1>
	// <0x559ee3>|0x033|+0x02f:'229'
	// <0x559f12>|0x062|-0x02b:'229'
	// <0x559ee7>|0x037|+0x02f:'230'
	// ******
}

// STATE[STUB]
u32 calc_data_crc<vostok::render::custom_config_value>( custom_config_value const& value )
{
	// LOCALS
	// mutable_buffer 					buffer
	// boost::crc_optimal< 32, 79764919, 4294967295, 4294967295, 1, 1 > processor
	// ******

	return 0;

	// FUNCTION BODY[0x55a4f0]: 11
	// <0x55a4fc>|0x00c|+0x00b:'234'
	// <0>
	// <0x55a507>|0x017|+0x007:'236'
	// <0x55a50e>|0x01e|+0x00a:'237'
	// <0>
	// <0x55a518>|0x028|+0x00a:'239'
	// <0>
	// <0x55a522>|0x032|+0x012:'241'
	// <0x55a534>|0x044|+0x00e:'242'
	// <0>
	// <0x55a542>|0x052|+0x005:'244'
	// ******
}

// STATE[STUB]
u32 get_num_config_fields_impl<vostok::configs::binary_config_value>( configs::binary_config_value const& value )
{
	return 0;

	// FUNCTION BODY[0x55a140]: 7
	// <0>
	// <1>
	// <0x55a142>|0x002|+0x01a:'251'
	// <0x55a15c>|0x01c|+0x014:'252'
	// <0x55a170>|0x030|+0x012:'253'
	// <0>
	// <0x55a182>|0x042|+0x003:'255'
	// ******
}

// STATE[STUB]
u32 get_num_config_fields<vostok::configs::binary_config_value>( configs::binary_config_value const& value )
{
	return 0;

	// FUNCTION BODY[0x55a240]: 7
	// <0>
	// <1>
	// <0x55a240>|0x000|+0x015:'262'
	// <0x55a255>|0x015|+0x010:'263'
	// <0x55a265>|0x025|+0x012:'264'
	// <0>
	// <0x55a277>|0x037|+0x003:'266'
	// ******
}

// STATE[STUB]
u32 get_config_data_memory_usage<vostok::configs::binary_config_value>( configs::binary_config_value const& value, u32& last_align_value )
{
	return 0;

	// FUNCTION BODY[0x55a190]: 27
	// <0>
	// <0x55a191>|0x001|+0x016:'272'
	// <0>
	// <0x55a1a7>|0x017|+0x016:'274'
	// <0>
	// <1>
	// <0x55a1bd>|0x02d|+0x044:'277'
	// <0>
	// <0x55a201>|0x071|+0x01f:'279'
	// <0>
	// <0x55a220>|0x090|-0x053:'281'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55a1cd>|0x03d|+0x012:'286'
	// <0x55a1df>|0x04f|+0x006:'287'
	// <0>
	// <0x55a1e5>|0x055|+0x005:'289'
	// <0>
	// <0x55a1ea>|0x05a|+0x002:'291'
	// <0>
	// <0x55a1ec>|0x05c|+0x00b:'293'
	// <0x55a1f7>|0x067|+0x004:'294'
	// <0>
	// <1>
	// <0x55a1fb>|0x06b|+0x037:'297'
	// <0x55a232>|0x0a2|-0x032:'297'
	// <0x55a200>|0x070|+0x037:'298'
	// ******
}

// STATE[STUB]
u32 calc_config_memory_usage_impl<vostok::configs::binary_config_value>( configs::binary_config_value const& value )
{
	// LOCALS
	// u32 								last_align_value
	// ******

	return 0;

	// FUNCTION BODY[0x55a6a0]: 7
	// <0>
	// <1>
	// <0x55a6a2>|0x002|+0x012:'305'
	// <0x55a6b4>|0x014|+0x018:'306'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
u32 calc_config_memory_usage_impl<vostok::render::effect_options_descriptor>( effect_options_descriptor const& value )
{
	return 0;

	// FUNCTION BODY[0x55a6d0]: 1
	// <0x55a6d4>|0x004|+0x032:'314'
	// ******
}

// STATE[STUB]
custom_config* create_custom_config_impl<vostok::render::effect_options_descriptor>(
	effect_options_descriptor const&	value,
	mutable_buffer&						data_buffer,
	u32&								out_data_crc,
	bool								is_calc_data_crc
)
{
	return NULL;

	// FUNCTION BODY[0x55a840]: 15
	// <0x55a841>|0x001|+0x039:'319'
	// <0>
	// <1>
	// <0x55a87a>|0x03a|+0x019:'322'
	// <0>
	// <0x55a893>|0x053|+0x009:'324'
	// <0>
	// <0x55a89c>|0x05c|+0x00a:'326'
	// <0>
	// <0x55a8a6>|0x066|+0x006:'328'
	// <0x55a8ac>|0x06c|+0x016:'329'
	// <0>
	// <0x55a8c2>|0x082|-0x007:'331'
	// <0>
	// <0x55a8bb>|0x07b|+0x002:'333'
	// <0x55a8bd>|0x07d|+0x009:'334'
	// ******
}

// STATE[STUB]
custom_config_ptr create_custom_config_impl<vostok::render::effect_options_descriptor>( effect_options_descriptor const& value, u32& out_data_crc, bool is_calc_data_crc )
{
	// LOCALS
	// mutable_buffer 					b
	// ******

	// FUNCTION BODY[0x55aa60]: 9
	// <0x55aa6c>|0x00c|+0x037:'338'
	// <0>
	// <0x55aaa3>|0x043|+0x019:'340'
	// <0>
	// <0x55aabc>|0x05c|+0x014:'342'
	// <0>
	// <0x55aad0>|0x070|+0x004:'344'
	// <0>
	// <0x55aad4>|0x074|+0x00b:'346'
	// ******
}

// STATE[STUB]
custom_config_ptr create_custom_config( effect_options_descriptor const& value, u32& out_data_crc, bool is_calc_data_crc )
{
	// FUNCTION BODY[0x55ab30]: 1
	// <0x55ab30>|0x000|+0x010:'351'
	// ******
}

// STATE[STUB]
custom_config_ptr create_custom_config( custom_config_value const& value, u32& out_data_crc, bool is_calc_data_crc )
{
	// FUNCTION BODY[0x55ab10]: 1
	// <0x55ab12>|0x002|+0x011:'356'
	// ******
}

// STATE[STUB]
custom_config_ptr create_custom_config(
	configs::binary_config_value const&		value,
	u32&									out_data_crc,
	bool									is_calc_data_crc
)
{
	// FUNCTION BODY[0x55aaf0]: 1
	// <0x55aaf2>|0x002|+0x011:'361'
	// ******
}

// STATE[STUB]
custom_config_ptr create_custom_config(
	effect_options_descriptor const&	value,
	mutable_buffer&						data_buffer,
	u32&								out_data_crc,
	bool								is_calc_data_crc
)
{
	// FUNCTION BODY[0x55a950]: 1
	// <0x55a950>|0x000|+0x023:'367'
	// ******
}

// STATE[STUB]
u32 calc_config_memory_usage( effect_options_descriptor const& value )
{
	return 0;

	// FUNCTION BODY[0x55a8d0]: 1
	// <0x55a8d4>|0x004|+0x032:'382'
	// ******
}

} // namespace render
} // namespace vostok
