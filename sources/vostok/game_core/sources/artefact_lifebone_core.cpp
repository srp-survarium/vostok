////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/artefact_lifebone_core.h>

#include <vostok/game_core/inventory.h>
#include <vostok/game_core/inventory_holder.h>

namespace survarium {

static hit_affects_type_enum protected_affects[artefact_lifebone_core::protects_count] = {
	affects_type_hand_damage, affects_type_hand_damage,
	affects_type_leg_damage,  affects_type_leg_damage
};

static pcstr protected_body_patrs[artefact_lifebone_core::protects_count] = {
	"left_hand", "right_hand",
	"left_leg", "right_leg"
};

artefact_lifebone_core::artefact_lifebone_core( ) :
	m_unlimited			( true ),
	m_passive_mode		( false ),
	m_cooldown_ms		( 0 ),
	m_last_used_time_ms	( 0 )
{
	for ( u32 i = 0 ; i < protects_count ; ++i )
	{
		m_damage_protectors[i].protect_affect_functor	= boost::bind( &artefact_lifebone_core::protect_affect, this, _1, _2 );
		m_damage_protectors[i].reduce_damage_functor	= boost::bind( &artefact_lifebone_core::reduce_damage, this, _1, _2, _3, _4 );
	}
}

artefact_lifebone_core::~artefact_lifebone_core( )
{
}

void artefact_lifebone_core::holder_assigned( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	switch_passive_mode_impl( true );
}

void artefact_lifebone_core::holder_removed( )
{
	if ( m_passive_mode )
		switch_passive_mode_impl( false );
}

// claude@NOTE: walled by inventory::holder() LTCG inlining - the target keeps holder()
// out-of-line (call survarium::inventory::holder) so dm is a real intrusive_ptr and the
// loop derefs it via operator*. Our /GL inlines holder() at every call site (m_holder
// read inline), which cascades and inlines the whole intrusive_ptr machinery. Fixable
// only by out-lining holder() in inventory.cpp (another unit). Source kept faithful (*dm).
void artefact_lifebone_core::switch_passive_mode_impl( bool switch_on )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );

	if ( switch_on )
		for ( u32 i = 0 ; i < protects_count ; ++i )
		{
			( *dm ).cancel_affect( protected_body_patrs[i], protected_affects[i] );
			( *dm ).register_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );
		}
	else
		for ( u32 i = 0 ; i < protects_count ; ++i )
			( *dm ).unregister_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );

	m_passive_mode = switch_on;
}

// claude@NOTE: same inventory::holder() LTCG-inline wall as switch_passive_mode_impl.
void artefact_lifebone_core::activate_impl( )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );
	for ( u32 i = 0 ; i < protects_count ; ++i )
	{
		body_part_parameters* bp = ( *dm ).get_body_part( protected_body_patrs[i] );
		bp->reset( );
	}
}

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
}

bool artefact_lifebone_core::protect_affect( pcstr, hit_affects_type_enum affect )
{
	switch ( affect )
	{
	case affects_type_hand_damage:
	case affects_type_leg_damage:
		return true;
		break;
	default:
		return false;
	}
}

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

void artefact_lifebone_core::load_core( configs::binary_config_value config )
{
	s32 amount		= (s32)config["amount"];
	inventory_item::set_amount( (u16)amount );
	m_unlimited		= amount == -1;
	m_cooldown_ms	= (u32)config["cooldown_ms"];
}

} // namespace survarium
