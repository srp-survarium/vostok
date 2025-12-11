////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/game_material_manager_cook.h>

namespace survarium {

// STATE[STUB]
game_material_manager_cook::game_material_manager_cook( bool server_usage ) : resources::translate_query_cook( resources::game_material_manager_class, reuse_true, use_current_thread_id )
{
	resources::register_cook( this );

	// FUNCTION BODY
	// <0x76366e>|0x03e|+0x00c:'21'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::translate_query(vostok::resources::query_result_for_cook&)
void game_material_manager_cook::translate_query( resources::query_result_for_cook& parent )
{
	// LOCALS
	// resources::request[2] 		r
	// ******

	// FUNCTION BODY
	// <0>
	// <0x7644b9>|0x009|+0x00e:'27'
	// <0x7644c7>|0x017|+0x00e:'28'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7644d5>|0x025|+0x0ce:'37'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::delete_resource(vostok::resources::resource_base*)
void game_material_manager_cook::delete_resource( resources::resource_base* res )
{
	// LOCALS
	// game_material_manager* 		mngr
	// ******

	// CALL SITE INFO
	// <0x7636ab> -> void* <unknown>(u32)
	// ******

	// FUNCTION BODY
	// <0x763699>|0x009|+0x006:'42'
	// <0>
	// <0x76369f>|0x00f|+0x00e:'44'
	// <0x7636ad>|0x01d|+0x022:'45'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::on_configs_loaded(vostok::resources::queries_result&)
void game_material_manager_cook::on_configs_loaded( resources::queries_result& data )
{
	// LOCALS
	// game_material_manager* 		manager
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> mtrl_cfg
	// resources::resource_ptr<configs::binary_config,resources::unmanaged_intrusive_base> pairs_cfg
	// ******

	// FUNCTION BODY
	// <0x76433a>|0x00a|+0x00c:'50'
	// <0x764346>|0x016|+0x022:'51'
	// <0>
	// <0x764368>|0x038|+0x022:'53'
	// <0>
	// <0x76438a>|0x05a|+0x04d:'55'
	// <0x7643d7>|0x0a7|+0x006:'56'
	// <0>
	// <0x7643dd>|0x0ad|+0x017:'58'
	// <0x7643f4>|0x0c4|+0x013:'59'
	// <0x764407>|0x0d7|+0x015:'60'
	// <0>
	// <1>
	// <0x76441c>|0x0ec|+0x025:'63'
	// <0>
	// <0x764441>|0x111|+0x026:'65'
	// <0x764467>|0x137|+0x02f:'66'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::create_game_materials(survarium::game_material_manager* const, vostok::configs::binary_config_value const&)
void game_material_manager_cook::create_game_materials( game_material_manager* manager, configs::binary_config_value const& materials_root )
{
	// LOCALS
	// configs::binary_config_value const* end
	// configs::binary_config_value const* it
	// configs::binary_config_value const& cfg<1>
	// game_material* 				mtrl<1>
	// ******

	// TYPEDEFS
	// typedef
	// 	configs::binary_config_value const*
	// 	const_iterator;

	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x7636e9>|0x009|+0x00d:'75'
	// <0x7636f6>|0x016|+0x00b:'76'
	// <0x763701>|0x021|+0x00b:'77'
	// <0>
	// <0x76370c>|0x02c|+0x017:'79'
	// <0>
	// <0x763723>|0x043|+0x006|[1]:'81'
	// <0x763729>|0x049|+0x02f:'82'
	// <0x763758>|0x078|+0x002:'83'
	// <0>
	// <0x76375a>|0x07a|+0x047:'85'
	// <0x7637a1>|0x0c1|+0x00c:'86'
	// <0x7637ad>|0x0cd|+0x00c:'87'
	// <0x7637b9>|0x0d9|+0x00c:'88'
	// <0x7637c5>|0x0e5|+0x01f:'89'
	// <0x7637e4>|0x104|+0x019:'90'
	// <0x7637fd>|0x11d|+0x005:'91'
	// <0x763802>|0x122|+0x00c:'92'
	// <0>
	// <0x76380e>|0x12e|+0x00f:'94'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::create_game_material_pairs(vostok::resources::query_result_for_cook&, survarium::game_material_manager* const, vostok::configs::binary_config_value const&)
void game_material_manager_cook::create_game_material_pairs( resources::query_result_for_cook& parent_query, game_material_manager* manager, configs::binary_config_value const& pairs_root )
{
	// LOCALS
	// vector<variant<32> const *> 	user_data_ptrs
	// configs::binary_config_value const* end
	// u32 							ud_idx
	// vector<game_material_manager_cook::query_ext_data>* ext_pair_data
	// vector<variant<32> > 		user_datas
	// vector<resources::request> 	requests
	// configs::binary_config_value const* it
	// material_pair* 				pair
	// pcstr 						particle_name
	// pcstr 						decal2_name
	// pcstr 						decal1_name
	// pcstr 						sound_name
	// resources::request 			r
	// game_material_manager_cook::query_ext_data ext_data
	// resources::request 			r
	// game_material_manager_cook::query_ext_data ext_data
	// u32 							sound_type
	// resources::request 			r
	// game_material_manager_cook::query_ext_data ext_data
	// resources::class_id_enum 	resource_type
	// u32 							i
	// resources::request 			r
	// game_material_manager_cook::query_ext_data ext_data
	// ******

	// TYPEDEFS
	// typedef
	// 	configs::binary_config_value const*
	// 	const_iterator;

	// typedef
	// 	vector<variant<32> >
	// 	user_data_vec;

	// typedef
	// 	vector<resources::request>
	// 	requests_vec;

	// typedef
	// 	vector<variant<32> const *>
	// 	user_data_ptrs_vec;

	// ******

	// FUNCTION BODY
	// <0>
	// <0x763b14>|0x014|+0x00f:'102'
	// <0x763b23>|0x023|+0x00f:'103'
	// <0>
	// <1>
	// <0x763b32>|0x032|+0x00c:'106'
	// <0>
	// <1>
	// <0x763b3e>|0x03e|+0x00c:'109'
	// <0>
	// <1>
	// <0x763b4a>|0x04a|+0x014:'112'
	// <0x763b5e>|0x05e|+0x04c:'113'
	// <0x763baa>|0x0aa|+0x00b:'114'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x763bb5>|0x0b5|+0x081:'121'
	// <0>
	// <0x763c36>|0x136|+0x027:'123'
	// <0>
	// <0x763c5d>|0x15d|+0x073|[1]:'125'
	// <0x763cd0>|0x1d0|+0x014:'126'
	// <0x763ce4>|0x1e4|+0x018:'127'
	// <0>
	// <0x763cfc>|0x1fc|+0x013:'129'
	// <0>
	// <0x763d0f>|0x20f|+0x01d|[2]:'131'
	// <0x763d2c>|0x22c|+0x014:'132'
	// <0>
	// <1>
	// <0x763d40>|0x240|+0x015|[3]:'135'
	// <0x763d55>|0x255|+0x013:'136'
	// <0>
	// <0x763d68>|0x268|+0x00e:'138'
	// <0x763d76>|0x276|+0x00b:'139'
	// <0>
	// <0x763d81>|0x281|+0x07c:'141'
	// <0x763dfd>|0x2fd|+0x040:'142'
	// <0>
	// <0x763e3d>|0x33d|+0x014:'144'
	// <0x763e51>|0x351|+0x06c:'145'
	// <0>
	// <1>
	// <0x763ebd>|0x3bd|+0x01d:'148'
	// <0x763eda>|0x3da|+0x014:'149'
	// <0>
	// <1>
	// <0x763eee>|0x3ee|+0x015|[3]:'152'
	// <0x763f03>|0x403|+0x013:'153'
	// <0>
	// <0x763f16>|0x416|+0x00e:'155'
	// <0x763f24>|0x424|+0x00b:'156'
	// <0x763f2f>|0x42f|+0x07c:'157'
	// <0>
	// <0x763fab>|0x4ab|+0x040:'159'
	// <0x763feb>|0x4eb|+0x014:'160'
	// <0x763fff>|0x4ff|+0x06c:'161'
	// <0>
	// <1>
	// <0x76406b>|0x56b|+0x01d:'164'
	// <0x764088>|0x588|+0x014:'165'
	// <0>
	// <0x76409c>|0x59c|+0x01d|[3]:'167'
	// <0x7640b9>|0x5b9|+0x00e:'168'
	// <0x7640c7>|0x5c7|+0x014:'169'
	// <0>
	// <0x7640db>|0x5db|+0x01b:'171'
	// <0x7640f6>|0x5f6|+0x013:'172'
	// <0>
	// <0x764109>|0x609|+0x00b:'174'
	// <0x764114>|0x614|+0x008:'175'
	// <0x76411c>|0x61c|+0x011:'176'
	// <0x76412d>|0x62d|+0x018:'177'
	// <0>
	// <1>
	// <0x764145>|0x645|+0x01a:'180'
	// <0x76415f>|0x65f|+0x00d:'181'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x76416c>|0x66c|+0x01c|[3]:'186'
	// <0>
	// <1>
	// <0x764188>|0x688|+0x00f|[4]:'189'
	// <0x764197>|0x697|+0x010:'190'
	// <0>
	// <0x7641a7>|0x6a7|+0x00b:'192'
	// <0x7641b2>|0x6b2|+0x008:'193'
	// <0x7641ba>|0x6ba|+0x011:'194'
	// <0>
	// <1>
	// <2>
	// <0x7641cb>|0x6cb|+0x018:'198'
	// <0x7641e3>|0x6e3|+0x002:'199'
	// <0>
	// <1>
	// <0x7641e5>|0x6e5|+0x010:'202'
	// <0x7641f5>|0x6f5|+0x005:'203'
	// <0>
	// <0x7641fa>|0x6fa|+0x013:'205'
	// <0>
	// <0x76420d>|0x70d|+0x00c:'207'
	// <0>
	// <1>
	// <0x764219>|0x719|+0x005:'210'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x76421e>|0x71e|+0x0d8:'219'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::on_decals_loaded(vostok::resources::queries_result&, survarium::vector<survarium::game_material_manager_cook::query_ext_data>*)
void game_material_manager_cook::on_decals_loaded( resources::queries_result& data, vector<game_material_manager_cook::query_ext_data>* ext_data )
{
	// LOCALS
	// game_material_manager_cook::query_ext_data* end
	// game_material_manager_cook::query_ext_data* it
	// u32 							i<1>
	// u32 							idx<1>
	// ******

	// FUNCTION BODY
	// <0>
	// <0x76384b>|0x01b|+0x013:'226'
	// <0>
	// <0x76385e>|0x02e|+0x023|[1]:'228'
	// <0>
	// <0x763881>|0x051|+0x01c:'230'
	// <0>
	// <0x76389d>|0x06d|+0x0ad:'232'
	// <0>
	// <0x76394a>|0x11a|+0x005:'234'
	// <0>
	// <1>
	// <2>
	// <0x76394f>|0x11f|+0x014:'238'
	// <0x763963>|0x133|+0x015:'239'
	// <0>
	// <0x763978>|0x148|+0x027|[1]:'241'
	// <0>
	// <0x76399f>|0x16f|+0x009:'243'
	// <0>
	// <0x7639a8>|0x178|+0x042:'245'
	// <0x7639ea>|0x1ba|+0x01b:'246'
	// <0x763a05>|0x1d5|+0x005:'247'
	// <0x763a0a>|0x1da|+0x009:'248'
	// <0>
	// <0x763a13>|0x1e3|+0x042:'250'
	// <0x763a55>|0x225|+0x01d:'251'
	// <0x763a72>|0x242|+0x009:'252'
	// <0x763a7b>|0x24b|+0x042:'253'
	// <0x763abd>|0x28d|+0x002:'254'
	// <0x763abf>|0x28f|+0x01e:'255'
	// <0>
	// <1>
	// <0x763add>|0x2ad|+0x005:'258'
	// <0>
	// <0x763ae2>|0x2b2|+0x013:'260'
	// ******
}

} // namespace survarium
