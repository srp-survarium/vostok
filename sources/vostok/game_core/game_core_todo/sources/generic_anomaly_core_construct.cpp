////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "generic_anomaly_core_construct.h"

namespace survarium {

// STATE[STUB]
// void survarium::generic_anomaly_core::load(vostok::configs::binary_config_value const&)
void generic_anomaly_core::load( vostok::configs::binary_config_value const& config )
{
	// LOCALS
	// u32 							states_count
	// u32 							artefact_containers_count<1>
	// u32 							s<1>
	// anomaly_state* 				state<2>
	// vostok::configs::binary_config_value current_state<2>
	// u32 							groups_count<2>
	// u32 							g<3>
	// zone_group* 					group<4>
	// u32 							zones_count<4>
	// vostok::configs::binary_config_value current_group<4>
	// ******

	// SKIPPED BLOCKS
	// <0x59ca2f><1>
	// <0x59cc21><2>
	// <0x59ce22><4>
	// ******

	// FUNCTION BODY
	// 1
	// <0x59c9d0>|0x000|0x000:'18'
	// <0x59c9ed>|0x01d|0x01d:'19'
	// <0x59ca08>|0x038|0x01b:'20'
	// <0x59ca23>|0x053|0x01b:'21'
	// 1
	// <0x59ca31>|0x061|0x00e:'23'
	// <0x59ca48>|0x078|0x017:'24'
	// <0x59ca6a>|0x09a|0x022:'25'
	// 1
	// 2
	// 3
	// <0x59ca84>|0x0b4|0x01a:'29'
	// <0x59caa1>|0x0d1|0x01d:'30'
	// <0x59cabe>|0x0ee|0x01d:'31'
	// <0x59cad9>|0x109|0x01b:'32'
	// <0x59caf4>|0x124|0x01b:'33'
	// <0x59cb0f>|0x13f|0x01b:'34'
	// <0x59cb2a>|0x15a|0x01b:'35'
	// <0x59cb45>|0x175|0x01b:'36'
	// <0x59cb60>|0x190|0x01b:'37'
	// <0x59cb7b>|0x1ab|0x01b:'38'
	// <0x59cb96>|0x1c6|0x01b:'39'
	// <0x59cbb1>|0x1e1|0x01b:'40'
	// 1
	// 2
	// <0x59cbcc>|0x1fc|0x01b:'43'
	// <0x59cbe3>|0x213|0x017:'44'
	// <0x59cc09>|0x239|0x026|[1]:'45'
	// 1
	// <0x59cc27>|0x257|0x01e:'47'
	// <0x59cc61>|0x291|0x03a:'48'
	// <0x59ccda>|0x30a|0x079:'49'
	// <0x59cd1a>|0x34a|0x040:'50'
	// <0x59cd23>|0x353|0x009:'51'
	// <0x59cd3c>|0x36c|0x019:'52'
	// <0x59cd54>|0x384|0x018:'53'
	// <0x59cd6e>|0x39e|0x01a:'54'
	// <0x59cd88>|0x3b8|0x01a:'55'
	// <0x59cda0>|0x3d0|0x018:'56'
	// <0x59cdb8>|0x3e8|0x018:'57'
	// 1
	// <0x59cdd0>|0x400|0x018:'59'
	// <0x59cde7>|0x417|0x017:'60'
	// <0x59ce0a>|0x43a|0x023|[3]:'61'
	// 1
	// <0x59ce28>|0x458|0x01e:'63'
	// <0x59ce62>|0x492|0x03a:'64'
	// <0x59cee4>|0x514|0x082:'65'
	// <0x59cf21>|0x551|0x03d:'66'
	// <0x59cf3a>|0x56a|0x019:'67'
	// <0x59cf52>|0x582|0x018:'68'
	// 1
	// <0x59cf6a>|0x59a|0x018:'70'
	// <0x59cf81>|0x5b1|0x017:'71'
	// <0x59cf9c>|0x5cc|0x01b:'72'
	// <0x59cfac>|0x5dc|0x010:'73'
	// <0x59cfb1>|0x5e1|0x005:'74'
	// 1
	// ******
}

// STATE[STUB]
// bool survarium::state_prio(survarium::anomaly_state*, survarium::anomaly_state*)
bool state_prio( anomaly_state* s1, anomaly_state* s2 )
{
	return false;
	// FUNCTION BODY
	// <0x59c634>|0x000|0x000:'80'
	// 1
	// 2
	// 3
	// <0x59c644>|0x010|0x010:'84'
	// <0x59c67f>|0x04b|0x03b:'85'
	// <0x59c681>|0x04d|0x002:'86'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::resolve_links(survarium::base_project*, vostok::configs::binary_config_value)
void generic_anomaly_core::resolve_links( base_project* p, vostok::configs::binary_config_value config )
{
	// LOCALS
	// u32 							artefact_containers_count
	// u32 							states_count
	// u32 							a<1>
	// pcstr 						full_path_name<2>
	// u32 							s<2>
	// anomaly_state* 				state<3>
	// u32 							g<4>
	// zone_group* 					group<5>
	// u32 							z<6>
	// pcstr 						full_path_name<7>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58b71e }, type_index: TypeIndex(0x94fb) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58b8c7 }, type_index: TypeIndex(0x94fb) })
	// ******

	// FUNCTION BODY
	// <0x59c6bf>|0x000|0x000:'92'
	// <0x59c6d0>|0x011|0x011|[1]:'93'
	// 1
	// <0x59c6ee>|0x02f|0x01e|[2]:'95'
	// <0x59c70f>|0x050|0x021:'96'
	// <0x59c778>|0x0b9|0x069:'97'
	// 1
	// <0x59c77d>|0x0be|0x005:'99'
	// <0x59c78e>|0x0cf|0x011|[2]:'100'
	// 1
	// <0x59c7ac>|0x0ed|0x01e|[3]:'102'
	// <0x59c7e0>|0x121|0x034|[4]:'103'
	// 1
	// <0x59c806>|0x147|0x026|[5]:'105'
	// <0x59c837>|0x178|0x031|[6]:'106'
	// 1
	// <0x59c869>|0x1aa|0x032|[7]:'108'
	// <0x59c8b8>|0x1f9|0x04f:'109'
	// <0x59c90e>|0x24f|0x056:'110'
	// <0x59c945>|0x286|0x037:'111'
	// <0x59c94a>|0x28b|0x005:'112'
	// <0x59c94f>|0x290|0x005:'113'
	// 1
	// <0x59c954>|0x295|0x005:'115'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		void**
		iterator_type;

	typedef
		zone_group::zone_wrapper*
		iterator_type;

} // namespace survarium
