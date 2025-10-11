////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_material_manager_cook.h"

namespace survarium {

// STATE[STUB]
// survarium::game_material_manager_cook::game_material_manager_cook(bool)
game_material_manager_cook::game_material_manager_cook( bool server_usage )
{
	// FUNCTION BODY
	// <0x76366e>|0x000|0x000:'21'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::translate_query(vostok::resources::query_result_for_cook&)
void game_material_manager_cook::translate_query( vostok::resources::query_result_for_cook& parent )
{
	// LOCALS
	// vostok::resources::request[2] r
	// ******

	// FUNCTION BODY
	// 1
	// <0x7644b9>|0x000|0x000:'27'
	// <0x7644c7>|0x00e|0x00e:'28'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x7644d5>|0x01c|0x00e:'37'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::delete_resource(vostok::resources::resource_base*)
void game_material_manager_cook::delete_resource( vostok::resources::resource_base* res )
{
	// LOCALS
	// game_material_manager* 		mngr
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x7526ab }, type_index: TypeIndex(0xaf48) })
	// ******

	// FUNCTION BODY
	// <0x763699>|0x000|0x000:'42'
	// 1
	// <0x76369f>|0x006|0x006:'44'
	// <0x7636ad>|0x014|0x00e:'45'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::on_configs_loaded(vostok::resources::queries_result&)
void game_material_manager_cook::on_configs_loaded( vostok::resources::queries_result& data )
{
	// LOCALS
	// game_material_manager* 		manager
	// vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> mtrl_cfg
	// vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> pairs_cfg
	// ******

	// FUNCTION BODY
	// <0x76433a>|0x000|0x000:'50'
	// <0x764346>|0x00c|0x00c:'51'
	// 1
	// <0x764368>|0x02e|0x022:'53'
	// 1
	// <0x76438a>|0x050|0x022:'55'
	// <0x7643d7>|0x09d|0x04d:'56'
	// 1
	// <0x7643dd>|0x0a3|0x006:'58'
	// <0x7643f4>|0x0ba|0x017:'59'
	// <0x764407>|0x0cd|0x013:'60'
	// 1
	// 2
	// <0x76441c>|0x0e2|0x015:'63'
	// 1
	// <0x764441>|0x107|0x025:'65'
	// <0x764467>|0x12d|0x026:'66'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::create_game_materials(survarium::game_material_manager* const, vostok::configs::binary_config_value const&)
void game_material_manager_cook::create_game_materials( game_material_manager* manager, vostok::configs::binary_config_value const& materials_root )
{
	// LOCALS
	// vostok::configs::binary_config_value const* end
	// vostok::configs::binary_config_value const* it
	// vostok::configs::binary_config_value const& cfg<1>
	// game_material* 				mtrl<1>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::configs::binary_config_value const*
	// 	const_iterator;

	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x7636e9>|0x000|0x000:'75'
	// <0x7636f6>|0x00d|0x00d:'76'
	// <0x763701>|0x018|0x00b:'77'
	// 1
	// <0x76370c>|0x023|0x00b:'79'
	// 1
	// <0x763723>|0x03a|0x017|[1]:'81'
	// <0x763729>|0x040|0x006:'82'
	// <0x763758>|0x06f|0x02f:'83'
	// 1
	// <0x76375a>|0x071|0x002:'85'
	// <0x7637a1>|0x0b8|0x047:'86'
	// <0x7637ad>|0x0c4|0x00c:'87'
	// <0x7637b9>|0x0d0|0x00c:'88'
	// <0x7637c5>|0x0dc|0x00c:'89'
	// <0x7637e4>|0x0fb|0x01f:'90'
	// <0x7637fd>|0x114|0x019:'91'
	// <0x763802>|0x119|0x005:'92'
	// 1
	// <0x76380e>|0x125|0x00c:'94'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::create_game_material_pairs(vostok::resources::query_result_for_cook&, survarium::game_material_manager* const, vostok::configs::binary_config_value const&)
void game_material_manager_cook::create_game_material_pairs( vostok::resources::query_result_for_cook& parent_query, game_material_manager* manager, vostok::configs::binary_config_value const& pairs_root )
{
	// LOCALS
	// vector<vostok::variant<32> const *> user_data_ptrs
	// vostok::configs::binary_config_value const* end
	// u32 							ud_idx
	// vector<game_material_manager_cook::query_ext_data>* ext_pair_data
	// vector<vostok::variant<32> > user_datas
	// vector<vostok::resources::request> requests
	// vostok::configs::binary_config_value const* it
	// material_pair* 				pair
	// pcstr 						particle_name
	// pcstr 						decal2_name
	// pcstr 						decal1_name
	// pcstr 						sound_name
	// vostok::resources::request 	r
	// game_material_manager_cook::query_ext_data ext_data
	// vostok::resources::request 	r
	// game_material_manager_cook::query_ext_data ext_data
	// u32 							sound_type
	// vostok::resources::request 	r
	// game_material_manager_cook::query_ext_data ext_data
	// vostok::resources::class_id_enum resource_type
	// u32 							i
	// vostok::resources::request 	r
	// game_material_manager_cook::query_ext_data ext_data
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::configs::binary_config_value const*
	// 	const_iterator;

	// typedef
	// 	vector<vostok::variant<32> >
	// 	user_data_vec;

	// typedef
	// 	vector<vostok::resources::request>
	// 	requests_vec;

	// typedef
	// 	vector<vostok::variant<32> const *>
	// 	user_data_ptrs_vec;

	// ******

	// FUNCTION BODY
	// 1
	// <0x763b14>|0x000|0x000:'102'
	// <0x763b23>|0x00f|0x00f:'103'
	// 1
	// 2
	// <0x763b32>|0x01e|0x00f:'106'
	// 1
	// 2
	// <0x763b3e>|0x02a|0x00c:'109'
	// 1
	// 2
	// <0x763b4a>|0x036|0x00c:'112'
	// <0x763b5e>|0x04a|0x014:'113'
	// <0x763baa>|0x096|0x04c:'114'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// <0x763bb5>|0x0a1|0x00b:'121'
	// 1
	// <0x763c36>|0x122|0x081:'123'
	// 1
	// <0x763c5d>|0x149|0x027|[1]:'125'
	// <0x763cd0>|0x1bc|0x073:'126'
	// <0x763ce4>|0x1d0|0x014:'127'
	// 1
	// <0x763cfc>|0x1e8|0x018:'129'
	// 1
	// <0x763d0f>|0x1fb|0x013|[2]:'131'
	// <0x763d2c>|0x218|0x01d:'132'
	// 1
	// 2
	// <0x763d40>|0x22c|0x014|[3]:'135'
	// <0x763d55>|0x241|0x015:'136'
	// 1
	// <0x763d68>|0x254|0x013:'138'
	// <0x763d76>|0x262|0x00e:'139'
	// 1
	// <0x763d81>|0x26d|0x00b:'141'
	// <0x763dfd>|0x2e9|0x07c:'142'
	// 1
	// <0x763e3d>|0x329|0x040:'144'
	// <0x763e51>|0x33d|0x014:'145'
	// 1
	// 2
	// <0x763ebd>|0x3a9|0x06c:'148'
	// <0x763eda>|0x3c6|0x01d:'149'
	// 1
	// 2
	// <0x763eee>|0x3da|0x014|[3]:'152'
	// <0x763f03>|0x3ef|0x015:'153'
	// 1
	// <0x763f16>|0x402|0x013:'155'
	// <0x763f24>|0x410|0x00e:'156'
	// <0x763f2f>|0x41b|0x00b:'157'
	// 1
	// <0x763fab>|0x497|0x07c:'159'
	// <0x763feb>|0x4d7|0x040:'160'
	// <0x763fff>|0x4eb|0x014:'161'
	// 1
	// 2
	// <0x76406b>|0x557|0x06c:'164'
	// <0x764088>|0x574|0x01d:'165'
	// 1
	// <0x76409c>|0x588|0x014|[3]:'167'
	// <0x7640b9>|0x5a5|0x01d:'168'
	// <0x7640c7>|0x5b3|0x00e:'169'
	// 1
	// <0x7640db>|0x5c7|0x014:'171'
	// <0x7640f6>|0x5e2|0x01b:'172'
	// 1
	// <0x764109>|0x5f5|0x013:'174'
	// <0x764114>|0x600|0x00b:'175'
	// <0x76411c>|0x608|0x008:'176'
	// <0x76412d>|0x619|0x011:'177'
	// 1
	// 2
	// <0x764145>|0x631|0x018:'180'
	// <0x76415f>|0x64b|0x01a:'181'
	// 1
	// 2
	// 3
	// 4
	// <0x76416c>|0x658|0x00d|[3]:'186'
	// 1
	// 2
	// <0x764188>|0x674|0x01c|[4]:'189'
	// <0x764197>|0x683|0x00f:'190'
	// 1
	// <0x7641a7>|0x693|0x010:'192'
	// <0x7641b2>|0x69e|0x00b:'193'
	// <0x7641ba>|0x6a6|0x008:'194'
	// 1
	// 2
	// 3
	// <0x7641cb>|0x6b7|0x011:'198'
	// <0x7641e3>|0x6cf|0x018:'199'
	// 1
	// 2
	// <0x7641e5>|0x6d1|0x002:'202'
	// <0x7641f5>|0x6e1|0x010:'203'
	// 1
	// <0x7641fa>|0x6e6|0x005:'205'
	// 1
	// <0x76420d>|0x6f9|0x013:'207'
	// 1
	// 2
	// <0x764219>|0x705|0x00c:'210'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x76421e>|0x70a|0x005:'219'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager_cook::on_decals_loaded(vostok::resources::queries_result&, survarium::vector<survarium::game_material_manager_cook::query_ext_data>*)
void game_material_manager_cook::on_decals_loaded( vostok::resources::queries_result& data, vector<game_material_manager_cook::query_ext_data>* ext_data )
{
	// LOCALS
	// game_material_manager_cook::query_ext_data* end
	// game_material_manager_cook::query_ext_data* it
	// u32 							i<1>
	// u32 							idx<1>
	// ******

	// FUNCTION BODY
	// 1
	// <0x76384b>|0x000|0x000:'226'
	// 1
	// <0x76385e>|0x013|0x013|[1]:'228'
	// 1
	// <0x763881>|0x036|0x023:'230'
	// 1
	// <0x76389d>|0x052|0x01c:'232'
	// 1
	// <0x76394a>|0x0ff|0x0ad:'234'
	// 1
	// 2
	// 3
	// <0x76394f>|0x104|0x005:'238'
	// <0x763963>|0x118|0x014:'239'
	// 1
	// <0x763978>|0x12d|0x015|[1]:'241'
	// 1
	// <0x76399f>|0x154|0x027:'243'
	// 1
	// <0x7639a8>|0x15d|0x009:'245'
	// <0x7639ea>|0x19f|0x042:'246'
	// <0x763a05>|0x1ba|0x01b:'247'
	// <0x763a0a>|0x1bf|0x005:'248'
	// 1
	// <0x763a13>|0x1c8|0x009:'250'
	// <0x763a55>|0x20a|0x042:'251'
	// <0x763a72>|0x227|0x01d:'252'
	// <0x763a7b>|0x230|0x009:'253'
	// <0x763abd>|0x272|0x042:'254'
	// <0x763abf>|0x274|0x002:'255'
	// 1
	// 2
	// <0x763add>|0x292|0x01e:'258'
	// 1
	// <0x763ae2>|0x297|0x005:'260'
	// ******
}

	/* TYPEDEFS

	typedef
		game_material_manager_cook::query_ext_data*
		iterator_type;

	typedef
		void const**
		iterator_type;

	typedef
		vostok::resources::request*
		iterator_type;

	typedef
		vostok::variant<32>*
		iterator_type;

} // namespace survarium
