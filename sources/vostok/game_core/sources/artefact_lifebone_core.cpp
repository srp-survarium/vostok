////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/artefact_lifebone_core.h>

#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

static hit_affects_type_enum protected_affects[artefact_lifebone_core::DAMAGE_PROTECTORS] = {
	affects_type_hand_damage, affects_type_hand_damage,
	affects_type_leg_damage,  affects_type_leg_damage
};

static pcstr protected_body_patrs[artefact_lifebone_core::DAMAGE_PROTECTORS] = {
	"left_hand", "right_hand",
	"left_leg", "right_leg"
};

// STATE[99.81%|DONE]: Stack differences
artefact_lifebone_core::artefact_lifebone_core( ) :
	m_unlimited			( true ),
	m_passive_mode		( false ),
	m_cooldown_ms		( 0 ),
	m_last_used_time_ms	( 0 )
{
	for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
	{
		m_damage_protectors[i].protect_affect_functor	= boost::bind( &artefact_lifebone_core::protect_affect, this, _1, _2 );
		m_damage_protectors[i].reduce_damage_functor	= boost::bind( &artefact_lifebone_core::reduce_damage, this, _1, _2, _3, _4 );
	}

	// FUNCTION BODY
	// <0x58fcb9>|0x0a9|+0x01c|[1]:'33'
	// <0>
	// <0x58fcd5>|0x0c5|+0x0c4:'35'
	// <0x58fd99>|0x189|+0x0e0:'36'
	// <0x58fe79>|0x269|+0x005:'37'
	// ******
}

// STATE[92.12%|PARTIAL]: target has vtable, because there is also `artefact_lifebone` class. I think
artefact_lifebone_core::~artefact_lifebone_core( )
{
}

// STATE[100%|DONE]
void artefact_lifebone_core::holder_assigned( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	switch_passive_mode_impl( true );
}

// STATE[100%|DONE]
void artefact_lifebone_core::holder_removed( )
{
	if ( m_passive_mode )
		switch_passive_mode_impl( false );
}

// STATE[46.30%|PARTIAL]: Almost nothing inlined in target!
void artefact_lifebone_core::switch_passive_mode_impl( bool switch_on )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );

	if ( switch_on )
	{
		for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
		{
			dm->cancel_affect( protected_body_patrs[i], protected_affects[i] );
			dm->register_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );
		}
	}
	else
	{
		for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
			dm->unregister_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );
	}

	m_passive_mode = switch_on;

	// FUNCTION BODY
	// <0x58ff19>|0x009|+0x02c:'58'
	// <0>
	// <0x58ff45>|0x035|+0x008:'60'
	// <0>
	// <0x58ff4d>|0x03d|+0x018|[1]:'62'
	// <0>
	// <0x58ff65>|0x055|+0x025:'64'
	// <0x58ff8a>|0x07a|+0x02d:'65'
	// <0x58ffb7>|0x0a7|+0x002:'66'
	// <0>
	// <1>
	// <0x58ffb9>|0x0a9|+0x018|[1]:'69'
	// <0x58ffd1>|0x0c1|+0x02d:'70'
	// <0>
	// <0x58fffe>|0x0ee|+0x00c:'72'
	// ******
}

// STATE[12.38%|PARTIAL]
void artefact_lifebone_core::activate_impl( )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );	// sushi@MATCH: This part didn't inline at all in target
	for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
	{
		body_part_parameters* bp = dm->get_body_part( protected_body_patrs[i] );
		bp->reset( );
	}

	// FUNCTION BODY
	// <0x58fe99>|0x009|+0x02c:'77'
	// <0x58fec5>|0x035|+0x018|[1]:'78'
	// <0>
	// <0x58fedd>|0x04d|+0x01d|[2]:'80'
	// <0x58fefa>|0x06a|+0x008:'81'
	// <0x58ff02>|0x072|+0x002:'82'
	// ******
}

// STATE[75.57%|PARTIAL]
void artefact_lifebone_core::action( bool key_down )
{
	if ( !key_down )
		return;

	if ( m_unlimited || inventory_item::amount( ) > 0 )
	{
		artefact_lifebone_core::activate_impl( );
		if ( !m_unlimited )
			inventory_item::set_amount( inventory_item::amount( ) - 1 );
	}

	if ( !m_unlimited && inventory_item::amount( ) == 0 )
	{
		artefact_lifebone_core::switch_passive_mode_impl( false );
	}

	// FUNCTION BODY
	// <0x590087>|0x007|+0x008:'87'
	// <0x59008f>|0x00f|+0x002:'88'
	// <0>
	// <0x590091>|0x011|+0x01d:'90'
	// <0>
	// <0x5900ae>|0x02e|+0x008:'92'
	// <0x5900b6>|0x036|+0x00e:'93'
	// <0x5900c4>|0x044|+0x016:'94'
	// <0>
	// <1>
	// <0x5900da>|0x05a|+0x01d:'97'
	// <0>
	// <0x5900f7>|0x077|+0x00a:'99'
	// <0>
	// <1>
	// ******
}

// STATE[SKIPPED]: This is written somehow with 4 statements! sushi@TODO: Multiple returns?
bool artefact_lifebone_core::protect_affect( pcstr __formal, hit_affects_type_enum affect )
{
	return affect >= affects_type_hand_damage && affect <= affects_type_leg_damage;


	// FUNCTION BODY
	// <0x58fad9>|0x009|+0x014:'106'
	// <0>
	// <1>
	// <2>
	// <0x58faed>|0x01d|+0x004:'110'
	// <0x58faf1>|0x021|+0x002:'111'
	// <0>
	// <0x58faf3>|0x023|+0x002:'113'
	// <0>
	// ******
}

// STATE[100%|DONE]
float artefact_lifebone_core::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	const float	amount,
	const float	armor_piercing
)
{
	VOSTOK_UNREFERENCED_PARAMETERS( body_part_name, damage_type, amount, armor_piercing );
	return amount;
}

// STATE[95%|PARTIAL]: `set_amount` didn't inline
void artefact_lifebone_core::load_core( configs::binary_config_value config )
{
	s32 amount		= (s32)config["amount"];
	inventory_item::set_amount( (u16)amount ); // sushi@TODO: HUH
	m_unlimited		= amount == -1;
	m_cooldown_ms	= (u32)config["cooldown_ms"];

	// FUNCTION BODY
	// <0x58fb59>|0x009|+0x015:'128'
	// <0x58fb6e>|0x01e|+0x00b:'129'
	// <0x58fb79>|0x029|+0x012:'130'
	// <0x58fb8b>|0x03b|+0x01b:'131'
	// ******
}

} // namespace survarium
