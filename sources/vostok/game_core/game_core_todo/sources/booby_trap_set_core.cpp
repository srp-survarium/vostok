////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "booby_trap_set_core.h"

namespace survarium {

// STATE[STUB]
// survarium::booby_trap_set_core::booby_trap_set_core()
booby_trap_set_core::booby_trap_set_core( )
{
}

// STATE[STUB]
// void survarium::booby_trap_set_core::~booby_trap_set_core()
void booby_trap_set_core::~booby_trap_set_core( )
{
	// LOCALS
	// void const* 					damage_parms_buffer
	// ******

	// FUNCTION BODY
	// <0x6fe263>|0x000|0x000:'33'
	// <0x6fe281>|0x01e|0x01e:'34'
	// <0x6fe2b8>|0x055|0x037:'35'
	// 1
	// <0x6fe2cc>|0x069|0x014:'37'
	// <0x6fe2da>|0x077|0x00e:'38'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::load(vostok::configs::binary_config_value const&)
void booby_trap_set_core::load( vostok::configs::binary_config_value const& config )
{
	// LOCALS
	// float 						defuse_time
	// vostok::configs::binary_config_value const& apply_dmg
	// float 						disarmed_life_time
	// float 						fired_life_time
	// float 						armed_life_time
	// u32 							count
	// u32 							i<1>
	// vostok::configs::binary_config_value const& conf_entry<2>
	// booby_trap_set_core::apply_damage& ad<2>
	// ******

	// SKIPPED BLOCKS
	// <0x6fe0ae><2>
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::configs::binary_config_value const&
	// 	conf_val_const_ref;

	// ******

	// FUNCTION BODY
	// 1
	// 2
	// <0x6fdc60>|0x000|0x000:'45'
	// <0x6fdc89>|0x029|0x029:'46'
	// <0x6fdc95>|0x035|0x00c:'47'
	// <0x6fdcf1>|0x091|0x05c:'48'
	// 1
	// 2
	// <0x6fdd6b>|0x10b|0x07a:'51'
	// <0x6fdd77>|0x117|0x00c:'52'
	// 1
	// <0x6fdd83>|0x123|0x00c:'54'
	// <0x6fdd8f>|0x12f|0x00c:'55'
	// <0x6fdd9b>|0x13b|0x00c:'56'
	// <0x6fdda7>|0x147|0x00c:'57'
	// <0x6fddb3>|0x153|0x00c:'58'
	// 1
	// <0x6fddbf>|0x15f|0x00c:'60'
	// <0x6fddcb>|0x16b|0x00c:'61'
	// 1
	// <0x6fddd7>|0x177|0x00c:'63'
	// 1
	// 2
	// 3
	// <0x6fdde3>|0x183|0x00c:'67'
	// <0x6fde03>|0x1a3|0x020:'68'
	// 1
	// <0x6fde34>|0x1d4|0x031:'70'
	// <0x6fde4b>|0x1eb|0x017:'71'
	// <0x6fde72>|0x212|0x027:'72'
	// <0x6fde89>|0x229|0x017:'73'
	// <0x6fdeb0>|0x250|0x027:'74'
	// <0x6fdec7>|0x267|0x017:'75'
	// <0x6fdeee>|0x28e|0x027:'76'
	// <0x6fdf05>|0x2a5|0x017:'77'
	// <0x6fdf2c>|0x2cc|0x027:'78'
	// 1
	// <0x6fdf4c>|0x2ec|0x020:'80'
	// <0x6fdf6c>|0x30c|0x020:'81'
	// 1
	// <0x6fdf8c>|0x32c|0x020:'83'
	// 1
	// <0x6fdf9c>|0x33c|0x010:'85'
	// <0x6fdfa7>|0x347|0x00b:'86'
	// <0x6fe012>|0x3b2|0x06b:'87'
	// 1
	// <0x6fe096>|0x436|0x084|[1]:'89'
	// 1
	// <0x6fe0b4>|0x454|0x01e:'91'
	// <0x6fe0c0>|0x460|0x00c:'92'
	// <0x6fe0cc>|0x46c|0x00c:'93'
	// <0x6fe0d8>|0x478|0x00c:'94'
	// 1
	// <0x6fe0e4>|0x484|0x00c:'96'
	// <0x6fe0f2>|0x492|0x00e:'97'
	// <0x6fe127>|0x4c7|0x035:'98'
	// 1
	// <0x6fe14b>|0x4eb|0x024:'100'
	// <0x6fe176>|0x516|0x02b:'101'
	// <0x6fe1a4>|0x544|0x02e:'102'
	// <0x6fe1be>|0x55e|0x01a:'103'
	// 1
	// 2
	// 3
	// <0x6fe1d8>|0x578|0x01a:'107'
	// <0x6fe1e4>|0x584|0x00c:'108'
	// <0x6fe1f0>|0x590|0x00c:'109'
	// 1
	// <0x6fe1f5>|0x595|0x005:'111'
	// <0x6fe201>|0x5a1|0x00c:'112'
	// <0x6fe20d>|0x5ad|0x00c:'113'
	// <0x6fe219>|0x5b9|0x00c:'114'
	// <0x6fe225>|0x5c5|0x00c:'115'
	// <0x6fe231>|0x5d1|0x00c:'116'
	// ******
}

// STATE[STUB]
// bool survarium::find_free_trap_predicate(vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base>)
bool find_free_trap_predicate( vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base> trap )
{
	return false;
	// FUNCTION BODY
	// <0x6fdc06>|0x000|0x000:'121'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove_trap(survarium::booby_trap_core&)
void booby_trap_set_core::remove_trap( booby_trap_core& trap )
{
	// FUNCTION BODY
	// <0x6fd807>|0x000|0x000:'126'
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base>* survarium::booby_trap_set_core::try_place_trap()
vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base>* booby_trap_set_core::try_place_trap( )
{
	// LOCALS
	// vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base>* trap_iter
	// vostok::math::float4x4 		place_transform
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed9c9 }, type_index: TypeIndex(0x15a97) })
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x6fe919>|0x000|0x000:'131'
	// <0x6fe928>|0x00f|0x00f:'132'
	// 1
	// <0x6fe93c>|0x023|0x014:'134'
	// <0x6fe944>|0x02b|0x008:'135'
	// <0x6fe957>|0x03e|0x013:'136'
	// 1
	// <0x6fe968>|0x04f|0x011:'138'
	// 1
	// <0x6fe998>|0x07f|0x030:'140'
	// <0x6fe9ac>|0x093|0x014:'141'
	// 1
	// <0x6fe9b1>|0x098|0x005:'143'
	// 1
	// <0x6fe9cb>|0x0b2|0x01a:'145'
	// 1
	// <0x6fe9e1>|0x0c8|0x016:'147'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove_trap_if_active(vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base>&)
void booby_trap_set_core::remove_trap_if_active( vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base>& trap )
{
	// FUNCTION BODY
	// <0x6fd859>|0x000|0x000:'152'
	// 1
	// <0x6fd865>|0x00c|0x00c:'154'
	// <0x6fd88f>|0x036|0x02a:'155'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove()
void booby_trap_set_core::remove( )
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// <0x6fd8b9>|0x000|0x000:'164'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::create_place_matrix_for_looking_point(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float4x4 const&)
vostok::math::float4x4 create_place_matrix_for_looking_point( vostok::math::float3 const& hit_point, vostok::math::float3 const& normal, vostok::math::float4x4 const& head_transform )
{
	// LOCALS
	// vostok::math::float4x4 		result
	// vostok::math::float3 const& 	right_candidate
	// vostok::math::float3 const& 	head_forward
	// vostok::math::float3 const& 	right<1>
	// vostok::math::float3 const& 	forward<1>
	// vostok::math::float3 const& 	up<1>
	// vostok::math::float3 const& 	right<1>
	// vostok::math::float3 const& 	forward<1>
	// vostok::math::float3 const& 	forward_candidate<1>
	// vostok::math::float3 const& 	up<1>
	// vostok::math::float3 const& 	head_right<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fda65><1>
	// ******

	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x6fda1b>|0x000|0x000:'169'
	// 1
	// <0x6fda2b>|0x010|0x010:'171'
	// <0x6fda39>|0x01e|0x00e:'172'
	// 1
	// <0x6fda51>|0x036|0x018:'174'
	// <0x6fda6b>|0x050|0x01a:'175'
	// <0x6fda7c>|0x061|0x011:'176'
	// <0x6fda82>|0x067|0x006:'177'
	// <0x6fdaa7>|0x08c|0x025:'178'
	// <0x6fdac2>|0x0a7|0x01b:'179'
	// <0x6fdadd>|0x0c2|0x01b:'180'
	// 1
	// <0x6fdaf8>|0x0dd|0x01b:'182'
	// <0x6fdafd>|0x0e2|0x005|[1]:'183'
	// <0x6fdb08>|0x0ed|0x00b:'184'
	// <0x6fdb29>|0x10e|0x021:'185'
	// <0x6fdb3b>|0x120|0x012:'186'
	// <0x6fdb58>|0x13d|0x01d:'187'
	// <0x6fdb5e>|0x143|0x006:'188'
	// <0x6fdb8f>|0x174|0x031:'189'
	// <0x6fdbad>|0x192|0x01e:'190'
	// <0x6fdbc8>|0x1ad|0x01b:'191'
	// 1
	// 2
	// <0x6fdbe6>|0x1cb|0x01e:'194'
	// ******
}

// STATE[STUB]
// bool survarium::booby_trap_set_core::get_visible_place_transform(vostok::math::float4x4&)
bool booby_trap_set_core::get_visible_place_transform( vostok::math::float4x4& result )
{
	// LOCALS
	// vostok::math::float3 		dir_to_head
	// vostok::math::float3 		ray_dir
	// game_material const* 		material
	// vostok::math::float4x4 		matrix_b
	// vostok::physics::bt_ghost_object* ghost
	// float 						slope_cos
	// vostok::physics::bt_rigid_body_base* body
	// vostok::physics::closest_ray_result ray_result
	// vostok::math::float3 		ray_from
	// vostok::math::float4x4 		looking_point_matrix
	// vostok::math::float4x4 		matrix_a
	// vostok::physics::world* 		world
	// float 						ray_length
	// u16 							group
	// u16 							mask
	// vostok::math::float4x4 const& head_transform
	// vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base> arbitrary_trap
	// base_player* 				player
	// u16 							game_material_id
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed458 }, type_index: TypeIndex(0xa82a) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed496 }, type_index: TypeIndex(0xa829) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed558 }, type_index: TypeIndex(0x946c) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed635 }, type_index: TypeIndex(0x94b4) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed689 }, type_index: TypeIndex(0x94c6) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed6a8 }, type_index: TypeIndex(0x15ab4) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ed8b1 }, type_index: TypeIndex(0x9474) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x6fe431>|0x000|0x000:'219'
	// <0x6fe45d>|0x02c|0x02c:'220'
	// 1
	// <0x6fe46f>|0x03e|0x012:'222'
	// <0x6fe49b>|0x06a|0x02c:'223'
	// 1
	// 2
	// 3
	// <0x6fe4ad>|0x07c|0x012:'227'
	// 1
	// <0x6fe4b6>|0x085|0x009:'229'
	// <0x6fe4db>|0x0aa|0x025:'230'
	// <0x6fe500>|0x0cf|0x025:'231'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x6fe515>|0x0e4|0x015:'237'
	// <0x6fe51e>|0x0ed|0x009:'238'
	// <0x6fe527>|0x0f6|0x009:'239'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x6fe55a>|0x129|0x033:'248'
	// 1
	// 2
	// <0x6fe563>|0x132|0x009:'251'
	// <0x6fe5be>|0x18d|0x05b:'252'
	// 1
	// 2
	// <0x6fe5c5>|0x194|0x007:'255'
	// 1
	// <0x6fe5e6>|0x1b5|0x021:'257'
	// 1
	// <0x6fe5f6>|0x1c5|0x010:'259'
	// 1
	// 2
	// <0x6fe60e>|0x1dd|0x018:'262'
	// <0x6fe61e>|0x1ed|0x010:'263'
	// 1
	// 2
	// <0x6fe625>|0x1f4|0x007:'266'
	// 1
	// 2
	// <0x6fe641>|0x210|0x01c:'269'
	// <0x6fe651>|0x220|0x010:'270'
	// 1
	// 2
	// <0x6fe658>|0x227|0x007:'273'
	// <0x6fe669>|0x238|0x011:'274'
	// 1
	// <0x6fe68f>|0x25e|0x026:'276'
	// 1
	// 2
	// <0x6fe6b7>|0x286|0x028:'279'
	// 1
	// 2
	// <0x6fe6e4>|0x2b3|0x02d:'282'
	// <0x6fe6f4>|0x2c3|0x010:'283'
	// 1
	// 2
	// <0x6fe6fb>|0x2ca|0x007:'286'
	// 1
	// 2
	// <0x6fe728>|0x2f7|0x02d:'289'
	// <0x6fe738>|0x307|0x010:'290'
	// 1
	// 2
	// <0x6fe73f>|0x30e|0x007:'293'
	// <0x6fe79b>|0x36a|0x05c:'294'
	// <0x6fe7ac>|0x37b|0x011:'295'
	// <0x6fe7c1>|0x390|0x015:'296'
	// 1
	// <0x6fe7ee>|0x3bd|0x02d:'298'
	// <0x6fe800>|0x3cf|0x012:'299'
	// 1
	// <0x6fe83f>|0x40e|0x03f:'301'
	// 1
	// <0x6fe885>|0x454|0x046:'303'
	// 1
	// <0x6fe8ba>|0x489|0x035:'305'
	// <0x6fe8ca>|0x499|0x010:'306'
	// <0x6fe8e1>|0x4b0|0x017:'307'
	// 1
	// <0x6fe8e3>|0x4b2|0x002:'309'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// 18
	// 19
	// 20
	// 21
	// 22
	// 23
	// 24
	// 25
	// 26
	// 27
	// 28
	// 29
	// 30
	// 31
	// 32
	// 33
	// 34
	// 35
	// 36
	// 37
	// 38
	// 39
	// 40
	// 41
	// 42
	// 43
	// 44
	// 45
	// 46
	// 47
	// 48
	// 49
	// 50
	// 51
	// 52
	// 53
	// 54
	// 55
	// 56
	// 57
	// 58
	// 59
	// 60
	// 61
	// 62
	// 63
	// 64
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void booby_trap_set_core::update_bones_matrices(
	vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&	user_skeleton,
	vostok::math::float4x4*				user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	vostok::math::float4x4&				character_head_transform,
	vostok::math::float4x4&				character_transform,
	vostok::animation::animation_player const&	animation_player)
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// <0x6fd79b>|0x000|0x000:'394'
	// ******
}

// STATE[STUB]
// bool survarium::trap_is_active(vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base> const&)
bool trap_is_active( vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base> const& trap )
{
	return false;
	// FUNCTION BODY
	// <0x6fd826>|0x000|0x000:'399'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::serialize_game_world_object_header(survarium::booby_trap_core const&, vostok::network_core::udp_match_packet&) const
void booby_trap_set_core::serialize_game_world_object_header( booby_trap_core const& trap, vostok::network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x6fd9d9>|0x000|0x000:'409'
	// 1
	// <0x6fd9e5>|0x00c|0x00c:'411'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::deserialize_game_world_object(vostok::network_core::packet_reader&)
void booby_trap_set_core::deserialize_game_world_object( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// game_world_object& 			object
	// booby_trap_core& 			trap
	// u8 							trap_index
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ec9bd }, type_index: TypeIndex(0x6b55) })
	// ******

	// FUNCTION BODY
	// <0x6fd96a>|0x000|0x000:'416'
	// 1
	// <0x6fd975>|0x00b|0x00b:'418'
	// <0x6fd981>|0x017|0x00c:'419'
	// 1
	// <0x6fd99c>|0x032|0x01b:'421'
	// 1
	// <0x6fd9a8>|0x03e|0x00c:'423'
	// <0x6fd9ae>|0x044|0x006:'424'
	// ******
}

// STATE[STUB]
// unsigned char survarium::booby_trap_set_core::trap_index(survarium::booby_trap_core const&) const
u8 booby_trap_set_core::trap_index( booby_trap_core const& trap ) const
{
	// LOCALS
	// vostok::resources::resource_ptr<booby_trap_core,vostok::resources::unmanaged_intrusive_base> const* trap_iter
	// ******

	return 0;
	// FUNCTION BODY
	// <0x6fd719>|0x000|0x000:'429'
	// <0x6fd74e>|0x035|0x035:'430'
	// 1
	// <0x6fd75a>|0x041|0x00c:'432'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove_trap_impl(survarium::booby_trap_core&)
void booby_trap_set_core::remove_trap_impl( booby_trap_core& trap )
{
	// LOCALS
	// inventory_holder& 			holder
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ec6e4 }, type_index: TypeIndex(0xa82f) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ec6fa }, type_index: TypeIndex(0x21b5d) })
	// ******

	// FUNCTION BODY
	// <0x6fd6b9>|0x000|0x000:'437'
	// 1
	// <0x6fd6c5>|0x00c|0x00c:'439'
	// <0x6fd6d5>|0x01c|0x010:'440'
	// <0x6fd6e6>|0x02d|0x011:'441'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::insert_trap(survarium::booby_trap_core&, vostok::math::float4x4 const&)
void booby_trap_set_core::insert_trap( booby_trap_core& trap, vostok::math::float4x4 const& transform )
{
	// LOCALS
	// vostok::physics::world* 		world
	// inventory_holder& 			holder
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ec660 }, type_index: TypeIndex(0xa82a) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ec68d }, type_index: TypeIndex(0x21b64) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ec69e }, type_index: TypeIndex(0xa82f) })
	// ******

	// FUNCTION BODY
	// <0x6fd639>|0x000|0x000:'446'
	// 1
	// <0x6fd645>|0x00c|0x00c:'448'
	// 1
	// <0x6fd655>|0x01c|0x010:'450'
	// <0x6fd665>|0x02c|0x010:'451'
	// 1
	// <0x6fd671>|0x038|0x00c:'453'
	// <0x6fd68f>|0x056|0x01e:'454'
	// ******
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium
