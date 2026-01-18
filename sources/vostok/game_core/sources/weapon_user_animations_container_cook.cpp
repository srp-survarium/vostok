////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_user_animations_container_cook.h>

namespace survarium {

// STATE[STUB]
weapon_user_animations_container_cook::weapon_user_animations_container_cook( )
	: resources::translate_query_cook( resources::animation_container_class, reuse_true, use_current_thread_id )
{
	// FUNCTION BODY
	// <0x762910>|0x000|+0x035:'15'	{
	// <0x762945>|0x035|      :'16'	}
	// ******
}

// STATE[STUB]
void weapon_user_animations_container_cook::delete_resource( resources::resource_base* resource )
{
	// FUNCTION BODY[0x7629b0]: 1
	// <0x7629b9>|0x009|+0x013:'20'
	// ******
}

// STATE[STUB]
void weapon_user_animations_container_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		config_name
	// ******

	// FUNCTION BODY[0x7632e0]: 10
	// <0x7632ef>|0x00f|+0x00b:'25'
	// <0x7632fa>|0x01a|+0x01d:'26'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x763317>|0x037|+0x11e:'34'
	// ******
}

// STATE[STUB]
void create_requests_for_animations(
	configs::binary_config_value const&		cfg,
	const u32								requests_count,
	buffer_vector< resources::request >&	requests
)
{
	// LOCALS
	// u32 								i<1>
	// ******

	// FUNCTION BODY[0x762950]: 5
	// <0x762956>|0x006|+0x00c:'41'
	// <0x762962>|0x012|+0x01a|[1]:'42'
	// <0>
	// <0x76297c>|0x02c|+0x02d:'44'
	// <0>
	// ******
}

// STATE[STUB]
void weapon_user_animations_container_cook::on_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// configs::binary_config_value const& root
	// buffer_vector< resources::request > requests
	// configs::binary_config_ptr 		config
	// ******

	// FUNCTION BODY[0x762f50]: 52
	// <0x762f60>|0x010|+0x00f:'50'
	// <0>
	// <0x762f6f>|0x01f|+0x00c:'52'
	// <0x762f7b>|0x02b|+0x013:'53'
	// <0x762f8e>|0x03e|+0x005:'54'
	// <0>
	// <1>
	// <2>
	// <0x762f93>|0x043|+0x027:'58'
	// <0>
	// <0x762fba>|0x06a|+0x022:'60'
	// <0x762fdc>|0x08c|+0x010:'61'
	// <0>
	// <0x762fec>|0x09c|+0x01c:'63'
	// <0x763008>|0x0b8|+0x01c:'64'
	// <0>
	// <0x763024>|0x0d4|+0x01c:'66'
	// <0x763040>|0x0f0|+0x01c:'67'
	// <0>
	// <0x76305c>|0x10c|+0x01c:'69'
	// <0x763078>|0x128|+0x01c:'70'
	// <0>
	// <0x763094>|0x144|+0x01c:'72'
	// <0x7630b0>|0x160|+0x01c:'73'
	// <0>
	// <0x7630cc>|0x17c|+0x01c:'75'
	// <0x7630e8>|0x198|+0x01c:'76'
	// <0>
	// <0x763104>|0x1b4|+0x01c:'78'
	// <0x763120>|0x1d0|+0x01c:'79'
	// <0>
	// <0x76313c>|0x1ec|+0x01c:'81'
	// <0x763158>|0x208|+0x01c:'82'
	// <0>
	// <0x763174>|0x224|+0x01c:'84'
	// <0x763190>|0x240|+0x01c:'85'
	// <0>
	// <0x7631ac>|0x25c|+0x01c:'87'
	// <0x7631c8>|0x278|+0x01c:'88'
	// <0>
	// <0x7631e4>|0x294|+0x01c:'90'
	// <0x763200>|0x2b0|+0x01c:'91'
	// <0>
	// <0x76321c>|0x2cc|+0x00c:'93'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x763228>|0x2d8|+0x08d:'101'
	// ******
}
/*
// STATE[STUB]
void get_animations_from_request_results<27>(
	resources::queries_result const&	data,
	const u32							animations_count,
	u32&								resource_index,
	resources::managed_resource_ptr[27]&	result
)
{
	// LOCALS
	// u32 								i<1>
	// ******

	// FUNCTION BODY[0x762b90]: 3
	// <0x762b97>|0x007|+0x00c:'107'
	// <0x762ba3>|0x013|+0x01a|[1]:'108'
	// <0x762bbd>|0x02d|+0x054:'109'
	// ******
}
*/
// STATE[STUB]
void weapon_user_animations_container_cook::on_animations_loaded( resources::queries_result& data )
{
	// LOCALS
	// u32 								resource_index
	// weapon_user_animations_container* container
	// ******

	// FUNCTION BODY[0x762c20]: 43
	// <0x762c2c>|0x00c|+0x00f:'114'
	// <0>
	// <0x762c3b>|0x01b|+0x00c:'116'
	// <0x762c47>|0x027|+0x013:'117'
	// <0x762c5a>|0x03a|+0x005:'118'
	// <0>
	// <1>
	// <0x762c5f>|0x03f|+0x051:'121'
	// <0x762cb0>|0x090|+0x007:'122'
	// <0x762cb7>|0x097|+0x01c:'123'
	// <0x762cd3>|0x0b3|+0x01c:'124'
	// <0>
	// <0x762cef>|0x0cf|+0x01c:'126'
	// <0x762d0b>|0x0eb|+0x01c:'127'
	// <0>
	// <0x762d27>|0x107|+0x01c:'129'
	// <0x762d43>|0x123|+0x01c:'130'
	// <0>
	// <0x762d5f>|0x13f|+0x01c:'132'
	// <0x762d7b>|0x15b|+0x01c:'133'
	// <0>
	// <0x762d97>|0x177|+0x01c:'135'
	// <0x762db3>|0x193|+0x01c:'136'
	// <0>
	// <0x762dcf>|0x1af|+0x01c:'138'
	// <0x762deb>|0x1cb|+0x01c:'139'
	// <0>
	// <0x762e07>|0x1e7|+0x01c:'141'
	// <0x762e23>|0x203|+0x01c:'142'
	// <0>
	// <0x762e3f>|0x21f|+0x01c:'144'
	// <0x762e5b>|0x23b|+0x01c:'145'
	// <0>
	// <0x762e77>|0x257|+0x01c:'147'
	// <0x762e93>|0x273|+0x01c:'148'
	// <0>
	// <0x762eaf>|0x28f|+0x01c:'150'
	// <0x762ecb>|0x2ab|+0x01c:'151'
	// <0>
	// <0x762ee7>|0x2c7|+0x00c:'153'
	// <0>
	// <0x762ef3>|0x2d3|+0x038:'155'
	// <0x762f2b>|0x30b|+0x013:'156'
	// ******
}

} // namespace survarium