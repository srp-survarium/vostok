////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include  <vostok/game_core/booby_trap_set_core.h>

#include  <vostok/game_core/booby_trap_core.h>

namespace survarium {

// STATE[STUB]
booby_trap_set_core::booby_trap_set_core( ) :
	m_traps				( NULL, 0 ),
	m_damage_parameters	( NULL, 0 ),
	inventory_item		( use_silent )
{
	// FUNCTION BODY
	// <0x6fe380>|0x000|+0x08e:'28'	{
	// <0x6fe40e>|0x08e|      :'29'	}
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::~booby_trap_set_core()
booby_trap_set_core::~booby_trap_set_core( )
{
	// LOCALS
	// pcvoid 						damage_parms_buffer
	// ******

	// FUNCTION BODY
	// <0x6fe263>|0x013|+0x01e:'33'
	// <0x6fe281>|0x031|+0x037:'34'
	// <0x6fe2b8>|0x068|+0x014:'35'
	// <0>
	// <0x6fe2cc>|0x07c|+0x00e:'37'
	// <0x6fe2da>|0x08a|+0x040:'38'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::load(vostok::configs::binary_config_value const&)
void booby_trap_set_core::load( configs::binary_config_value const& config )
{
	// LOCALS
	// float 						defuse_time
	// configs::binary_config_value const& apply_dmg
	// float 						disarmed_life_time
	// float 						fired_life_time
	// float 						armed_life_time
	// u32 							count
	// u32 							i<1>
	// configs::binary_config_value const& conf_entry<2>
	// booby_trap_set_core::apply_damage& ad<2>
	// ******

	// SKIPPED BLOCKS
	// <0x6fe0ae><2>
	// ******

	// TYPEDEFS
	// typedef
	// 	configs::binary_config_value const&
	// 	conf_val_const_ref;

	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x6fdc60>|0x010|+0x029:'45'
	// <0x6fdc89>|0x039|+0x00c:'46'
	// <0x6fdc95>|0x045|+0x05c:'47'
	// <0x6fdcf1>|0x0a1|+0x07a:'48'
	// <0>
	// <1>
	// <0x6fdd6b>|0x11b|+0x00c:'51'
	// <0x6fdd77>|0x127|+0x00c:'52'
	// <0>
	// <0x6fdd83>|0x133|+0x00c:'54'
	// <0x6fdd8f>|0x13f|+0x00c:'55'
	// <0x6fdd9b>|0x14b|+0x00c:'56'
	// <0x6fdda7>|0x157|+0x00c:'57'
	// <0x6fddb3>|0x163|+0x00c:'58'
	// <0>
	// <0x6fddbf>|0x16f|+0x00c:'60'
	// <0x6fddcb>|0x17b|+0x00c:'61'
	// <0>
	// <0x6fddd7>|0x187|+0x00c:'63'
	// <0>
	// <1>
	// <2>
	// <0x6fdde3>|0x193|+0x020:'67'
	// <0x6fde03>|0x1b3|+0x031:'68'
	// <0>
	// <0x6fde34>|0x1e4|+0x017:'70'
	// <0x6fde4b>|0x1fb|+0x027:'71'
	// <0x6fde72>|0x222|+0x017:'72'
	// <0x6fde89>|0x239|+0x027:'73'
	// <0x6fdeb0>|0x260|+0x017:'74'
	// <0x6fdec7>|0x277|+0x027:'75'
	// <0x6fdeee>|0x29e|+0x017:'76'
	// <0x6fdf05>|0x2b5|+0x027:'77'
	// <0x6fdf2c>|0x2dc|+0x020:'78'
	// <0>
	// <0x6fdf4c>|0x2fc|+0x020:'80'
	// <0x6fdf6c>|0x31c|+0x020:'81'
	// <0>
	// <0x6fdf8c>|0x33c|+0x010:'83'
	// <0>
	// <0x6fdf9c>|0x34c|+0x00b:'85'
	// <0x6fdfa7>|0x357|+0x06b:'86'
	// <0x6fe012>|0x3c2|+0x084:'87'
	// <0>
	// <0x6fe096>|0x446|+0x01e|[1]:'89'
	// <0>
	// <0x6fe0b4>|0x464|+0x00c:'91'
	// <0x6fe0c0>|0x470|+0x00c:'92'
	// <0x6fe0cc>|0x47c|+0x00c:'93'
	// <0x6fe0d8>|0x488|+0x00c:'94'
	// <0>
	// <0x6fe0e4>|0x494|+0x00e:'96'
	// <0x6fe0f2>|0x4a2|+0x035:'97'
	// <0x6fe127>|0x4d7|+0x024:'98'
	// <0>
	// <0x6fe14b>|0x4fb|+0x02b:'100'
	// <0x6fe176>|0x526|+0x02e:'101'
	// <0x6fe1a4>|0x554|+0x01a:'102'
	// <0x6fe1be>|0x56e|+0x01a:'103'
	// <0>
	// <1>
	// <2>
	// <0x6fe1d8>|0x588|+0x00c:'107'
	// <0x6fe1e4>|0x594|+0x00c:'108'
	// <0x6fe1f0>|0x5a0|+0x005:'109'
	// <0>
	// <0x6fe1f5>|0x5a5|+0x00c:'111'
	// <0x6fe201>|0x5b1|+0x00c:'112'
	// <0x6fe20d>|0x5bd|+0x00c:'113'
	// <0x6fe219>|0x5c9|+0x00c:'114'
	// <0x6fe225>|0x5d5|+0x00c:'115'
	// <0x6fe231>|0x5e1|+0x00c:'116'
	// ******
}

// STATE[STUB]
// bool survarium::find_free_trap_predicate(vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base>)
bool find_free_trap_predicate( resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base> trap )
{
	return false;

	// FUNCTION BODY
	// <0x6fdc06>|0x006|+0x039:'121'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove_trap(survarium::booby_trap_core&)
void booby_trap_set_core::remove_trap( booby_trap_core& trap )
{
	// FUNCTION BODY
	// <0x6fd807>|0x007|+0x00c:'126'
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base>* survarium::booby_trap_set_core::try_place_trap()
resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base>* booby_trap_set_core::try_place_trap( )
{
	// LOCALS
	// resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base>* trap_iter
	// float4x4 					place_transform
	// ******

	// CALL SITE INFO
	// <0x6fe9c9> -> void <unknown>(booby_trap_core&, float4x4 const&)
	// ******

	return NULL;

	// FUNCTION BODY
	// <0x6fe919>|0x009|+0x00f:'131'
	// <0x6fe928>|0x018|+0x014:'132'
	// <0>
	// <0x6fe93c>|0x02c|+0x008:'134'
	// <0x6fe944>|0x034|+0x013:'135'
	// <0x6fe957>|0x047|+0x011:'136'
	// <0>
	// <0x6fe968>|0x058|+0x030:'138'
	// <0>
	// <0x6fe998>|0x088|+0x014:'140'
	// <0x6fe9ac>|0x09c|+0x005:'141'
	// <0>
	// <0x6fe9b1>|0x0a1|+0x01a:'143'
	// <0>
	// <0x6fe9cb>|0x0bb|+0x016:'145'
	// <0>
	// <0x6fe9e1>|0x0d1|+0x003:'147'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove_trap_if_active(vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base>&)
void booby_trap_set_core::remove_trap_if_active( resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base>& trap )
{
	// FUNCTION BODY
	// <0x6fd859>|0x009|+0x00c:'152'
	// <0>
	// <0x6fd865>|0x015|+0x02a:'154'
	// <0x6fd88f>|0x03f|+0x011:'155'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove()
void booby_trap_set_core::remove( )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6fd8b9>|0x009|+0x09c:'164'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::create_place_matrix_for_looking_point(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float4x4 const&)
float4x4 create_place_matrix_for_looking_point( float3 const& hit_point, float3 const& normal, float4x4 const& head_transform )
{
	// LOCALS
	// float4x4 					result
	// float3 const& 				right_candidate
	// float3 const& 				head_forward
	// float3 const& 				right<1>
	// float3 const& 				forward<1>
	// float3 const& 				up<1>
	// float3 const& 				right<1>
	// float3 const& 				forward<1>
	// float3 const& 				forward_candidate<1>
	// float3 const& 				up<1>
	// float3 const& 				head_right<1>
	// ******

	// SKIPPED BLOCKS
	// <0x6fda65><1>
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY
	// <0x6fda1b>|0x00b|+0x010:'169'
	// <0>
	// <0x6fda2b>|0x01b|+0x00e:'171'
	// <0x6fda39>|0x029|+0x018:'172'
	// <0>
	// <0x6fda51>|0x041|+0x01a:'174'
	// <0x6fda6b>|0x05b|+0x011:'175'
	// <0x6fda7c>|0x06c|+0x006:'176'
	// <0x6fda82>|0x072|+0x025:'177'
	// <0x6fdaa7>|0x097|+0x01b:'178'
	// <0x6fdac2>|0x0b2|+0x01b:'179'
	// <0x6fdadd>|0x0cd|+0x01b:'180'
	// <0>
	// <0x6fdaf8>|0x0e8|+0x005:'182'
	// <0x6fdafd>|0x0ed|+0x00b|[1]:'183'
	// <0x6fdb08>|0x0f8|+0x021:'184'
	// <0x6fdb29>|0x119|+0x012:'185'
	// <0x6fdb3b>|0x12b|+0x01d:'186'
	// <0x6fdb58>|0x148|+0x006:'187'
	// <0x6fdb5e>|0x14e|+0x031:'188'
	// <0x6fdb8f>|0x17f|+0x01e:'189'
	// <0x6fdbad>|0x19d|+0x01b:'190'
	// <0x6fdbc8>|0x1b8|+0x01e:'191'
	// <0>
	// <1>
	// <0x6fdbe6>|0x1d6|+0x010:'194'
	// ******
}

// STATE[STUB]
// bool survarium::booby_trap_set_core::get_visible_place_transform(vostok::math::float4x4&)
bool booby_trap_set_core::get_visible_place_transform( float4x4& result )
{
	// LOCALS
	// float3 						dir_to_head
	// float3 						ray_dir
	// game_material const* 		material
	// float4x4 					matrix_b
	// physics::bt_ghost_object* 	ghost
	// float 						slope_cos
	// physics::bt_rigid_body_base* body
	// physics::closest_ray_result 	ray_result
	// float3 						ray_from
	// float4x4 					looking_point_matrix
	// float4x4 					matrix_a
	// physics::world* 				world
	// float 						ray_length
	// u16 							group
	// u16 							mask
	// float4x4 const& 				head_transform
	// resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base> arbitrary_trap
	// base_player* 				player
	// u16 							game_material_id
	// ******

	// CALL SITE INFO
	// <0x6fe458> -> physics::world* <unknown>()
	// <0x6fe496> -> base_player* <unknown>()
	// <0x6fe558> -> physics::closest_ray_result <unknown>(float3 const&, float3 const&, const float, u16, u16)
	// <0x6fe635> -> u16 <unknown>() const
	// <0x6fe689> -> u16 <unknown>(const int, const bool) const
	// <0x6fe6a8> -> game_material_manager const& <unknown>()
	// <0x6fe8b1> -> bool <unknown>(physics::bt_collision_shape* const, float4x4 const&, float4x4&, u16, u16)
	// ******

	return false;

	// FUNCTION BODY
	// <0x6fe431>|0x011|+0x02c:'219'
	// <0x6fe45d>|0x03d|+0x012:'220'
	// <0>
	// <0x6fe46f>|0x04f|+0x02c:'222'
	// <0x6fe49b>|0x07b|+0x012:'223'
	// <0>
	// <1>
	// <2>
	// <0x6fe4ad>|0x08d|+0x009:'227'
	// <0>
	// <0x6fe4b6>|0x096|+0x025:'229'
	// <0x6fe4db>|0x0bb|+0x025:'230'
	// <0x6fe500>|0x0e0|+0x015:'231'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6fe515>|0x0f5|+0x009:'237'
	// <0x6fe51e>|0x0fe|+0x009:'238'
	// <0x6fe527>|0x107|+0x033:'239'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6fe55a>|0x13a|+0x009:'248'
	// <0>
	// <1>
	// <0x6fe563>|0x143|+0x05b:'251'
	// <0x6fe5be>|0x19e|+0x007:'252'
	// <0>
	// <1>
	// <0x6fe5c5>|0x1a5|+0x021:'255'
	// <0>
	// <0x6fe5e6>|0x1c6|+0x010:'257'
	// <0>
	// <0x6fe5f6>|0x1d6|+0x018:'259'
	// <0>
	// <1>
	// <0x6fe60e>|0x1ee|+0x010:'262'
	// <0x6fe61e>|0x1fe|+0x007:'263'
	// <0>
	// <1>
	// <0x6fe625>|0x205|+0x01c:'266'
	// <0>
	// <1>
	// <0x6fe641>|0x221|+0x010:'269'
	// <0x6fe651>|0x231|+0x007:'270'
	// <0>
	// <1>
	// <0x6fe658>|0x238|+0x011:'273'
	// <0x6fe669>|0x249|+0x026:'274'
	// <0>
	// <0x6fe68f>|0x26f|+0x028:'276'
	// <0>
	// <1>
	// <0x6fe6b7>|0x297|+0x02d:'279'
	// <0>
	// <1>
	// <0x6fe6e4>|0x2c4|+0x010:'282'
	// <0x6fe6f4>|0x2d4|+0x007:'283'
	// <0>
	// <1>
	// <0x6fe6fb>|0x2db|+0x02d:'286'
	// <0>
	// <1>
	// <0x6fe728>|0x308|+0x010:'289'
	// <0x6fe738>|0x318|+0x007:'290'
	// <0>
	// <1>
	// <0x6fe73f>|0x31f|+0x05c:'293'
	// <0x6fe79b>|0x37b|+0x011:'294'
	// <0x6fe7ac>|0x38c|+0x015:'295'
	// <0x6fe7c1>|0x3a1|+0x02d:'296'
	// <0>
	// <0x6fe7ee>|0x3ce|+0x012:'298'
	// <0x6fe800>|0x3e0|+0x03f:'299'
	// <0>
	// <0x6fe83f>|0x41f|+0x046:'301'
	// <0>
	// <0x6fe885>|0x465|+0x035:'303'
	// <0>
	// <0x6fe8ba>|0x49a|+0x010:'305'
	// <0x6fe8ca>|0x4aa|+0x017:'306'
	// <0x6fe8e1>|0x4c1|+0x002:'307'
	// <0>
	// <0x6fe8e3>|0x4c3|+0x017:'309'
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
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <45>
	// <46>
	// <47>
	// <48>
	// <49>
	// <50>
	// <51>
	// <52>
	// <53>
	// <54>
	// <55>
	// <56>
	// <57>
	// <58>
	// <59>
	// <60>
	// <61>
	// <62>
	// <63>
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void booby_trap_set_core::update_bones_matrices(
	resources::resource_ptr<animation::skeleton,resources::unmanaged_intrusive_base> const&	user_skeleton,
	float4x4*							user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	float4x4&							character_head_transform,
	float4x4&							character_transform,
	animation::animation_player const&	animation_player
)
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6fd79b>|0x00b|+0x052:'394'
	// ******
}

// STATE[STUB]
// bool survarium::trap_is_active(vostok::resources::resource_ptr<survarium::booby_trap_core,vostok::resources::unmanaged_intrusive_base> const&)
bool trap_is_active( resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base> const& trap )
{
	return false;

	// FUNCTION BODY
	// <0x6fd826>|0x006|+0x025:'399'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::serialize_game_world_object_header(survarium::booby_trap_core const&, vostok::network_core::udp_match_packet&) const
void booby_trap_set_core::serialize_game_world_object_header( booby_trap_core const& trap, network_core::udp_match_packet& packet ) const
{
	// FUNCTION BODY
	// <0x6fd9d9>|0x009|+0x00c:'409'
	// <0>
	// <0x6fd9e5>|0x015|+0x018:'411'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::deserialize_game_world_object(vostok::network_core::packet_reader&)
void booby_trap_set_core::deserialize_game_world_object( network_core::packet_reader& reader )
{
	// LOCALS
	// game_world_object& 			object
	// booby_trap_core& 			trap
	// u8 							trap_index
	// ******

	// CALL SITE INFO
	// <0x6fd9bd> -> void <unknown>(network_core::packet_reader&)
	// ******

	// FUNCTION BODY
	// <0x6fd96a>|0x00a|+0x00b:'416'
	// <0>
	// <0x6fd975>|0x015|+0x00c:'418'
	// <0x6fd981>|0x021|+0x01b:'419'
	// <0>
	// <0x6fd99c>|0x03c|+0x00c:'421'
	// <0>
	// <0x6fd9a8>|0x048|+0x006:'423'
	// <0x6fd9ae>|0x04e|+0x011:'424'
	// ******
}

// STATE[STUB]
// unsigned char survarium::booby_trap_set_core::trap_index(survarium::booby_trap_core const&) const
u8 booby_trap_set_core::trap_index( booby_trap_core const& trap ) const
{
	// LOCALS
	// resources::resource_ptr<booby_trap_core,resources::unmanaged_intrusive_base> const* trap_iter
	// ******

	return 0;

	// FUNCTION BODY
	// <0x6fd719>|0x009|+0x035:'429'
	// <0x6fd74e>|0x03e|+0x00c:'430'
	// <0>
	// <0x6fd75a>|0x04a|+0x021:'432'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::remove_trap_impl(survarium::booby_trap_core&)
void booby_trap_set_core::remove_trap_impl( booby_trap_core& trap )
{
	// LOCALS
	// inventory_holder& 			holder
	// ******

	// CALL SITE INFO
	// <0x6fd6e4> -> void <unknown>(game_world_object&)
	// <0x6fd6fa> -> void <unknown>(scheduler&)
	// ******

	// FUNCTION BODY
	// <0x6fd6b9>|0x009|+0x00c:'437'
	// <0>
	// <0x6fd6c5>|0x015|+0x010:'439'
	// <0x6fd6d5>|0x025|+0x011:'440'
	// <0x6fd6e6>|0x036|+0x016:'441'
	// ******
}

// STATE[STUB]
// void survarium::booby_trap_set_core::insert_trap(survarium::booby_trap_core&, vostok::math::float4x4 const&)
void booby_trap_set_core::insert_trap( booby_trap_core& trap, float4x4 const& transform )
{
	// LOCALS
	// physics::world* 				world
	// inventory_holder& 			holder
	// ******

	// CALL SITE INFO
	// <0x6fd660> -> physics::world* <unknown>()
	// <0x6fd68d> -> void <unknown>(physics::world*, float4x4 const&, scheduler&)
	// <0x6fd69e> -> void <unknown>(game_world_object&)
	// ******

	// FUNCTION BODY
	// <0x6fd639>|0x009|+0x00c:'446'
	// <0>
	// <0x6fd645>|0x015|+0x010:'448'
	// <0>
	// <0x6fd655>|0x025|+0x010:'450'
	// <0x6fd665>|0x035|+0x00c:'451'
	// <0>
	// <0x6fd671>|0x041|+0x01e:'453'
	// <0x6fd68f>|0x05f|+0x011:'454'
	// ******
}

} // namespace survarium
