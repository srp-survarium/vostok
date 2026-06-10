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

// STATE[99.81%|DONE]: LTCG frame-size noise - target sub esp,0FCh vs base 0F4h (8-byte boost::bind temp slot); structure 5/5 identical.
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

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts (0x278 both) - no diverging rows
	// VERDICT: STRUCTURE MATCH - residual is LTCG frame-size noise (target sub esp,0FCh vs base 0F4h boost::bind temp slot).
}

// STATE[92.12%|PARTIAL]: MI-destructor 2nd vtable store, not steerable from empty body.
artefact_lifebone_core::~artefact_lifebone_core( )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (empty body; target 0x5f vs base 0x68 bytes)
	// VERDICT: STRUCTURE MATCH - byte delta is the compiler-generated MI-dtor vtable re-establishment store sequencing, not steerable from an empty body.
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

// STATE[46.30%|PARTIAL]: holder()/intrusive_ptr accessor inline-vs-call LTCG wall.
void artefact_lifebone_core::switch_passive_mode_impl( bool switch_on )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );

	if ( switch_on )
		for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
		{
			dm->cancel_affect( protected_body_patrs[i], protected_affects[i] );
			dm->register_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );
		}
	else
		for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
			dm->unregister_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );

	m_passive_mode = switch_on;

	// STRUCTURE DIFF: target 9 stmts / base 9 stmts
	// SIZE +0x4a | 71 | damage_model_ptr dm = m_inventory->holder( ).damage_model( );
	// SIZE +0x5  | 76 | dm->cancel_affect( protected_body_patrs[i], protected_affects[i] );
	// SIZE +0x5  | 77 | dm->register_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );
	// SIZE +0x5  | 81 | dm->unregister_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );
	// VERDICT: STRUCTURE MATCH (9/9) - all SIZE rows are inventory::holder()/intrusive_ptr ctor/operator-> inline-vs-call LTCG (base inlines them incl. the assert eater, target calls out-of-line); non-steerable from this TU.
}

// STATE[12.38%|PARTIAL]: holder()/intrusive_ptr accessor inline-vs-call LTCG wall.
void artefact_lifebone_core::activate_impl( )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );
	for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
	{
		body_part_parameters* bp = dm->get_body_part( protected_body_patrs[i] );
		bp->reset( );
	}

	// STRUCTURE DIFF: target 5 stmts / base 5 stmts
	// SIZE +0x4a | 94 | damage_model_ptr dm = m_inventory->holder( ).damage_model( );
	// SIZE +0xb  | 97 | body_part_parameters* bp = dm->get_body_part( protected_body_patrs[i] );
	// VERDICT: STRUCTURE MATCH (5/5) - holder()+intrusive_ptr copy-ctor and operator-> (with its assert eater) inlined in base, out-of-line calls in target; the accessor LTCG wall, non-steerable from this TU.
}

// STATE[96.14%|DONE]: LTCG arg-passing for set_amount (register vs stack).
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

	// STRUCTURE DIFF: target 8 stmts / base 8 stmts
	// SIZE +0x1 | 116 | inventory_item::set_amount( inventory_item::amount( ) - 1 );
	// VERDICT: STRUCTURE MATCH (8/8) - set_amount LTCG promoted convention (target passes the arg in ecx, base thiscall+push); call-boundary residual.
}

// STATE[100%|DONE]: the 4 statements are a switch - the 2-value case cluster lowers to a
// jl/jle range check on the switch temp; the unreachable break after the return still
// emits its 2-byte jmp (/Od keeps unreachable statements).
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

// STATE[92.12%|DONE]: LTCG arg-passing for set_amount (register vs stack).
void artefact_lifebone_core::load_core( configs::binary_config_value config )
{
	s32 amount		= (s32)config["amount"];
	inventory_item::set_amount( (u16)amount );
	m_unlimited		= amount == -1;
	m_cooldown_ms	= (u32)config["cooldown_ms"];

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE +0x2 | 164 | inventory_item::set_amount( (u16)amount );
	// VERDICT: STRUCTURE MATCH (4/4) - set_amount LTCG promoted convention (arg in register vs push); call-boundary residual.
}

} // namespace survarium
