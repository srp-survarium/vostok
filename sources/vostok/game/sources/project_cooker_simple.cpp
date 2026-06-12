////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "project_cooker_simple.h"

namespace survarium {

// STATE[STUB]
 project_cooker_simple::project_cooker_simple( bool editor_present ) :
	// base args are the legacy prior (translate_query_cook has no default ctor);
	// a matcher confirms when this TU is enabled
	translate_query_cook( resources::game_project_simple_class, reuse_true, use_any_thread_id ),
	m_editor_present( editor_present )
{
	// FUNCTION BODY[0x5d74d0]: 1
	// <0x5d74ee>|0x01e|+0x04b:'48'
	// ******
}

// STATE[STUB]
void project_cooker_simple::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// fs_new::virtual_path_string 		game_proj_path
	// fs_new::virtual_path_string 		project_name
	// ******

	// FUNCTION BODY[0x5d98a0]: 49
	// <0x5d98ad>|0x00d|+0x02b:'53'
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
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <0x5d98d8>|0x038|+0x00a:'90'
	// <0x5d98e2>|0x042|+0x038:'91'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5d991a>|0x07a|+0x094:'100'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_game_project_loaded( resources::queries_result& data, resources::query_result_for_cook* parent )
{
	// LOCALS
	// configs::binary_config_ptr 		game_proj_ptr
	// ******

	// FUNCTION BODY[0x5d9800]: 6
	// <0>
	// <1>
	// <2>
	// <0x5d9801>|0x001|+0x050:'150'
	// <0>
	// <0x5d9851>|0x051|+0x01e:'152'
	// ******
}

// STATE[STUB]
void project_cooker_simple::on_object_loaded(
	game_object_&							__formal,
	simple_game_project*					project,
	resources::query_result_for_cook*		parent_query
)
{
	// CALL SITE INFO
	// <0x5d7e8a> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x5d7e30]: 8
	// <0x5d7e34>|0x004|+0x00a:'383'
	// <0x5d7e3e>|0x00e|+0x03d:'384'
	// <0>
	// <0x5d7e7b>|0x04b|+0x011:'386'
	// <0>
	// <0x5d7e8c>|0x05c|+0x024:'388'
	// <0x5d7eb0>|0x080|+0x00b:'389'
	// <0>
	// ******
}

// STATE[STUB]
void project_cooker_simple::create_game_objects(
	configs::binary_config_ptr				project_cfg,
	resources::query_result_for_cook*		parent_query
)
{
	// LOCALS
	// vector< variant< 32 > const* > 	ladders_user_data
	// configs::binary_config_value 	render_visuals
	// boost::function< void( game_object_& ) > callback
	// vector< resources::request > 	damage_zones_requests
	// simple_game_project* 			project
	// vector< resources::creation_request > ext_objects_requests
	// fs_new::virtual_path_string 		project_source_path
	// base_game_scene* 				base_game_scene
	// vector< variant< 32 > const* > 	damage_zones_user_data
	// configs::binary_config_value const* it_e
	// u32 								request_idx
	// vector< resources::request > 	ladders_requests
	// vector< resources::request > 	collision_and_render
	// configs::binary_config_value 	collision_objects
	// configs::binary_config_value const* it
	// collision_geometry* 				geometry
	// victory_items_container* 		container
	// resources::request 				r
	// variant< 32 >* 					ud
	// variant< 32 >* 					ud
	// resources::request 				request
	// ******

} // namespace survarium
