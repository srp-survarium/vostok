////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "animated_model_instance_cook.h"
#include "animated_model_instance.h"

namespace survarium {

// STATE[STUB]
 animated_model_instance_cook::animated_model_instance_cook( ) :
	// buildability: matcher supplies real class/reuse/thread args
	resources::translate_query_cook( resources::game_animated_model_instance_class, reuse_false, use_current_thread_id )
{
	// FUNCTION BODY[0x769390]: 0
	// <0x769390>|0x000|+0x051:'25'	{
	// <0x7693e1>|0x051|      :'26'	}
	// ******
}

// STATE[STUB]
void animated_model_instance_cook::translate_request_path( pcstr request, fs_new::virtual_path_string& new_request ) const
{
	// FUNCTION BODY[0x7693f0]: 1
	// <0x7693f0>|0x000|+0x017:'30'
	// ******
}

// STATE[STUB]
void animated_model_instance_cook::translate_query( resources::query_result_for_cook& parent )
{
	// FUNCTION BODY[0x769bb0]: 8
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x769bba>|0x00a|+0x084:'42'
	// ******
}

// STATE[STUB]
void animated_model_instance_cook::on_config_loaded( resources::queries_result& data )
{
	// LOCALS
	// pcstr 							render_model_path
	// fs_new::virtual_path_string 		damage_config_path
	// variant< 32 > const*[3] 			params
	// variant< 32 > 					physics_world
	// configs::binary_config_ptr 		config
	// physics::world* 					physics
	// resources::request[3] 			requests
	// pcstr 							physics_model_path
	// ******

	// FUNCTION BODY[0x769980]: 56
	// <0x769980>|0x000|+0x00c:'46'	{
	// <0x76998c>|0x00c|+0x008:'47'
	// <0x769994>|0x014|+0x00d:'48'
	// <0>
	// <1>
	// <0x7699a1>|0x021|+0x014:'51'
	// <0>
	// <1>
	// <2>
	// <0x7699b5>|0x035|+0x042:'55'
	// <0x7699f7>|0x077|+0x00a:'56'
	// <0x769a01>|0x081|+0x00c:'57'
	// <0>
	// <1>
	// <0x769a0d>|0x08d|+0x00e:'60'
	// <0>
	// <1>
	// <0x769a1b>|0x09b|+0x012:'63'
	// <0>
	// <1>
	// <0x769a2d>|0x0ad|+0x012:'66'
	// <0x769a3f>|0x0bf|+0x009:'67'
	// <0x769a48>|0x0c8|+0x03e:'68'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x769a86>|0x106|+0x017:'74'
	// <0>
	// <1>
	// <2>
	// <0x769a9d>|0x11d|+0x008:'78'
	// <0x769aa5>|0x125|+0x011:'79'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x769ab6>|0x136|+0x006:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x769abc>|0x13c|+0x008:'90'
	// <0>
	// <0x769ac4>|0x144|+0x00b:'92'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x769acf>|0x14f|-0x123:'102'
	// <0x7699ac>|0x02c|+0x1c2:'103'
	// <0x769b6e>|0x1ee|      :'103'	}
	// ******
}

// STATE[STUB]
void animated_model_instance_cook::on_subresources_loaded( resources::queries_result& data )
{
	// LOCALS
	// variant< 32 > 					ud
	// configs::binary_config_ptr 		collision_config
	// resources::query_result_for_cook* const parent
	// fs_new::virtual_path_string 		hit_params_config_path
	// ******

	// FUNCTION BODY[0x7695c0]: 37
	// <0x7695c0>|0x000|+0x00e:'106'	{
	// <0x7695ce>|0x00e|+0x003:'107'
	// <0x7695d1>|0x011|+0x014:'108'
	// <0>
	// <1>
	// <0x7695e5>|0x025|+0x014:'111'
	// <0>
	// <1>
	// <2>
	// <0x7695f9>|0x039|+0x042:'115'
	// <0x76963b>|0x07b|+0x0a7:'116'
	// <0x7696e2>|0x122|+0x0af:'117'
	// <0>
	// <0x769791>|0x1d1|+0x049:'119'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7697da>|0x21a|+0x026:'125'
	// <0x769800>|0x240|+0x029:'126'
	// <0x769829>|0x269|+0x006:'127'
	// <0>
	// <0x76982f>|0x26f|+0x035:'129'
	// <0x769864>|0x2a4|+0x022:'130'
	// <0>
	// <0x769886>|0x2c6|+0x008:'132'
	// <0>
	// <0x76988e>|0x2ce|+0x005:'134'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x769893>|0x2d3|-0x2a3:'143'
	// <0x7695f0>|0x030|+0x34b:'144'
	// <0x76993b>|0x37b|      :'144'	}
	// ******
}

// STATE[STUB]
void animated_model_instance_cook::on_hit_params_loaded( resources::queries_result& data, animated_model_instance* new_model )
{
	// LOCALS
	// resources::query_result_for_cook* const parent
	// ******

	// FUNCTION BODY[0x769490]: 14
	// <0x769490>|0x000|+0x009:'147'	{
	// <0x769499>|0x009|+0x003:'148'
	// <0x76949c>|0x00c|+0x005:'149'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7694a1>|0x011|+0x027:'155'
	// <0x7694c8>|0x038|+0x0a9:'155'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x769571>|0x0e1|-0x0b7:'160'
	// <0x7694ba>|0x02a|+0x0e6:'161'
	// <0x7695a0>|0x110|-0x0e1:'161'
	// <0x7694bf>|0x02f|+0x0ec:'162'
	// <0x7695ab>|0x11b|      :'162'	}
	// ******
}

// STATE[STUB]
void animated_model_instance_cook::delete_resource( resources::resource_base* resource )
{
	// CALL SITE INFO
	// <0x769475> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x769410]: 5
	// <0>
	// <0x769412>|0x002|+0x014:'167'
	// <0x769426>|0x016|+0x009:'168'
	// <0x76942f>|0x01f|+0x033:'169'
	// <0x769462>|0x052|+0x027:'170'
	// ******
}

} // namespace survarium
