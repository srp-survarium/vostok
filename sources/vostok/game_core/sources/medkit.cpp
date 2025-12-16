////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/medkit.h>


namespace survarium {

// STATE[STUB]
medkit::medkit( ) :
	inventory_item				( use_silent ),
	m_config_activity_time_ms	( 1 )
{
}

// STATE[STUB]
medkit::~medkit( )
{
	VOSTOK_DELETE_IMPL( g_allocator, m_influences );
	VOSTOK_DELETE_IMPL( g_allocator, m_affects );
	VOSTOK_DELETE_IMPL( g_allocator, m_damage_protect );

	// FUNCTION BODY
	// <0x760192>|0x012|+0x040:'32'
	// <0x7601d2>|0x052|+0x040:'33'
	// <0x760212>|0x092|+0x040:'34'
	// ******
}

// STATE[STUB]
// void survarium::medkit::load(vostok::configs::binary_config_value)
void medkit::load( configs::binary_config_value config )
{
	// LOCALS
	// float 						activation_delay_sec
	// float 						activity_time_sec
	// configs::binary_config_value remove_affects
	// configs::binary_config_value influences
	// configs::binary_config_value damage_protect
	// u32 							i<1>
	// medkit::item_influence& 		infl<2>
	// u32 							i<2>
	// medkit::affect& 				affct<3>
	// u32 							i<3>
	// medkit::damage_protection& 	dmgp<4>
	// ******

	// SKIPPED BLOCKS
	// <0x75fd78><2>
	// <0x75fe9b><3>
	// <0x75ffa3><4>
	// ******

	// FUNCTION BODY
	// <0x75fc30>|0x010|+0x017:'39'
	// <0x75fc47>|0x027|+0x017:'40'
	// <0x75fc5e>|0x03e|+0x027:'41'
	// <0>
	// <0x75fc85>|0x065|+0x017:'43'
	// <0x75fc9c>|0x07c|+0x027:'44'
	// <0>
	// <0x75fcc3>|0x0a3|+0x030:'46'
	// <0>
	// <0x75fcf3>|0x0d3|+0x014:'48'
	// <0x75fd07>|0x0e7|+0x02f:'49'
	// <0>
	// <0x75fd36>|0x116|+0x020:'51'
	// <0>
	// <0x75fd56>|0x136|+0x028|[1]:'53'
	// <0>
	// <0x75fd7e>|0x15e|+0x015:'55'
	// <0x75fd93>|0x173|+0x035:'56'
	// <0x75fdc8>|0x1a8|+0x024:'57'
	// <0x75fdec>|0x1cc|+0x015:'58'
	// <0x75fe01>|0x1e1|+0x005:'59'
	// <0>
	// <1>
	// <0x75fe06>|0x1e6|+0x030:'62'
	// <0>
	// <0x75fe36>|0x216|+0x014:'64'
	// <0x75fe4a>|0x22a|+0x02f:'65'
	// <0>
	// <0x75fe79>|0x259|+0x024|[2]:'67'
	// <0>
	// <0x75fe9d>|0x27d|+0x015:'69'
	// <0x75feb2>|0x292|+0x035:'70'
	// <0x75fee7>|0x2c7|+0x022:'71'
	// <0x75ff09>|0x2e9|+0x005:'72'
	// <0>
	// <1>
	// <0x75ff0e>|0x2ee|+0x030:'75'
	// <0>
	// <0x75ff3e>|0x31e|+0x014:'77'
	// <0x75ff52>|0x332|+0x02f:'78'
	// <0>
	// <0x75ff81>|0x361|+0x028|[3]:'80'
	// <0>
	// <0x75ffa9>|0x389|+0x015:'82'
	// <0x75ffbe>|0x39e|+0x031:'83'
	// <0x75ffef>|0x3cf|+0x0c9:'84'
	// <0x7600b8>|0x498|+0x038:'85'
	// <0x7600f0>|0x4d0|+0x038:'86'
	// <0x760128>|0x508|+0x024:'87'
	// <0x76014c>|0x52c|+0x029:'88'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::medkit::set_active(bool)
void medkit::set_active( bool bactive )
{
	// LOCALS
	// player_stamina& 				stamina<1>
	// u32 							i<1>
	// medkit::damage_protection& 	dmgp<2>
	// ******

	// SKIPPED BLOCKS
	// <0x75f8c2><2>
	// ******

	// CALL SITE INFO
	// <0x75f840> -> base_player* <unknown>()
	// <0x75f850> -> player_stamina& <unknown>()
	// <0x75f90f> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// <0x75f94f> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x75f73d>|0x00d|+0x00c:'94'
	// <0>
	// <0x75f749>|0x019|+0x012:'96'
	// <0>
	// <0x75f75b>|0x02b|+0x012:'98'
	// <0x75f76d>|0x03d|+0x012:'99'
	// <0>
	// <0x75f77f>|0x04f|+0x0a3:'101'
	// <0>
	// <0x75f822>|0x0f2|+0x002:'103'
	// <0>
	// <0x75f824>|0x0f4|+0x031|[1]:'105'
	// <0x75f855>|0x125|+0x02b:'106'
	// <0>
	// <0x75f880>|0x150|+0x023:'108'
	// <0>
	// <1>
	// <0x75f8a3>|0x173|+0x025|[1]:'111'
	// <0>
	// <0x75f8c8>|0x198|+0x012:'113'
	// <0x75f8da>|0x1aa|+0x00e:'114'
	// <0x75f8e8>|0x1b8|+0x035:'115'
	// <0x75f91d>|0x1ed|+0x002:'116'
	// <0x75f91f>|0x1ef|+0x043:'117'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
// void survarium::medkit::action(bool)
void medkit::action( bool key_down )
{
	// FUNCTION BODY
	// <0x75f977>|0x007|+0x008:'125'
	// <0x75f97f>|0x00f|+0x002:'126'
	// <0>
	// <0x75f981>|0x011|+0x00e:'128'
	// <0x75f98f>|0x01f|+0x002:'129'
	// <0>
	// <0x75f991>|0x021|+0x00f:'131'
	// <0>
	// <0x75f9a0>|0x030|+0x00a:'133'
	// <0x75f9aa>|0x03a|+0x016:'134'
	// <0>
	// ******
}

// STATE[STUB]
// void survarium::medkit::active_tick(const unsigned int)
void medkit::active_tick( u32 frame_time_ms )
{
	// LOCALS
	// u32 							medkit_time
	// u32 							time_left_ms
	// u32 							delay_time<1>
	// player_stamina& 				stamina<1>
	// u32 							i<1>
	// float 						health_amount<2>
	// medkit::item_influence const& infl<2>
	// ******

	// SKIPPED BLOCKS
	// <0x75fa75><1>
	// <0x75fad3><1>
	// <0x75fb81><2>
	// ******

	// CALL SITE INFO
	// <0x75faf9> -> base_player* <unknown>()
	// <0x75fb09> -> player_stamina& <unknown>()
	// <0x75fbdb> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x75fa59>|0x009|+0x00c:'140'
	// <0>
	// <0x75fa65>|0x015|+0x006:'142'
	// <0>
	// <0x75fa6b>|0x01b|+0x00c:'144'
	// <0>
	// <0x75fa77>|0x027|+0x014:'146'
	// <0x75fa8b>|0x03b|+0x015:'147'
	// <0>
	// <1>
	// <0x75faa0>|0x050|+0x00c:'150'
	// <0x75faac>|0x05c|+0x005:'151'
	// <0>
	// <0x75fab1>|0x061|+0x009:'153'
	// <0>
	// <0x75faba>|0x06a|+0x002:'155'
	// <0x75fabc>|0x06c|+0x005:'156'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75fac1>|0x071|+0x014:'161'
	// <0>
	// <1>
	// <0x75fad5>|0x085|+0x008:'164'
	// <0>
	// <0x75fadd>|0x08d|+0x031:'166'
	// <0x75fb0e>|0x0be|+0x02b:'167'
	// <0>
	// <1>
	// <0x75fb39>|0x0e9|+0x014:'170'
	// <0x75fb4d>|0x0fd|+0x015:'171'
	// <0>
	// <1>
	// <0x75fb62>|0x112|+0x021|[1]:'174'
	// <0>
	// <0x75fb83>|0x133|+0x012:'176'
	// <0x75fb95>|0x145|+0x01f:'177'
	// <0>
	// <0x75fbb4>|0x164|+0x035:'179'
	// <0>
	// <0x75fbe9>|0x199|+0x002:'181'
	// <0>
	// <0x75fbeb>|0x19b|+0x016:'183'
	// <0>
	// <0x75fc01>|0x1b1|+0x00a:'185'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void medkit::remove_affects( )
{
	for ( u32 i = 0 ; i < m_affects_count; ++i )
	{
		medkit::affect const& affct = m_affects[i];
		// sushi@TODO: Needs `inventory`.
	}

	// FUNCTION BODY
	// <0x75f9d9>|0x009|+0x021|[1]:'192'
	// <0>
	// <0x75f9fa>|0x02a|+0x012|[2]:'194'
	// <0x75fa0c>|0x03c|+0x037:'195'
	// <0>
	// <1>
	// ******
}

// STATE[99.90%|DONE]: 4 less bytes on stack in base
medkit::damage_protection const* medkit::find_damage_protection( pcstr body_part_name, pcstr hit_type )
{
	for ( u32 i = 0 ; i < m_damage_protect_count ; ++i )
	{
		medkit::damage_protection const& dmgp = m_damage_protect[i];

		if ( strings::equal( dmgp.body_part_name, body_part_name ) && strings::equal( dmgp.hit_type, hit_type ) )
			return &dmgp;
	}
	return NULL;

	// FUNCTION BODY
	// <0x75f669>|0x009|+0x021|[1]:'202'
	// <0>
	// <0x75f68a>|0x02a|+0x012|[2]:'204'
	// <0>
	// <0x75f69c>|0x03c|+0x02a:'206'
	// <0x75f6c6>|0x066|+0x005:'207'
	// <0x75f6cb>|0x06b|+0x002:'208'
	// <0x75f6cd>|0x06d|+0x002:'209'
	// ******
}

// STATE[100%|DONE]
float medkit::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing
)
{
	VOSTOK_UNREFERENCED_PARAMETER( armor_piercing );

	medkit::damage_protection const* dmgp = find_damage_protection( body_part_name, damage_type );

	if ( !dmgp )
		return amount;

	if ( dmgp->threshold > amount )
		return 0.0f;

	return ( amount - dmgp->threshold ) * dmgp->hit_coeff;

	// FUNCTION BODY
	// <0>
	// <1>
	// <0x75f6e9>|0x009|+0x013:'219'
	// <0>
	// <0x75f6fc>|0x01c|+0x006:'221'
	// <0x75f702>|0x022|+0x005:'222'
	// <0>
	// <0x75f707>|0x027|+0x00e:'224'
	// <0x75f715>|0x035|+0x004:'225'
	// <0>
	// <0x75f719>|0x039|+0x00f:'227'
	// ******
}

} // namespace survarium
