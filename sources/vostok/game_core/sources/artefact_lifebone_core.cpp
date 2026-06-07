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
}

// STRUCTURE DIFF[target 0x57fbb0 | base 0x4550c0]: target 0 / base 0 stmts (empty body)
// target re-writes the primary vtable ptr a 2nd time before chaining to ~artefact_base:
//   0x45 mov eax,[ebp-Ch]; 0x48 mov dword ptr [eax],0   (base omits this store)
// VERDICT: STRUCTURE MATCH (empty body, 0/0 stmts) - MI-destructor vtable re-establishment store emitted by the compiler, not source-steerable from an empty dtor.
// STATE[92.12%|PARTIAL]: MI-destructor 2nd vtable store, not steerable from empty body.
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

// STRUCTURE DIFF[target 0x57ff10 | base 0x455490]: target 14 / base 13 stmts
// 0x009 <0x2c> | 0x009 <0x76> | damage_model_ptr dm = m_inventory->holder( ).damage_model( );   SIZE
// <0>         | --          |    EMPTY only target
// 0x055 <0x25> | 0x09f <0x2a> | dm->cancel_affect( protected_body_patrs[i], protected_affects[i] );   SIZE
// 0x07a <0x2d> | 0x0c9 <0x32> | dm->register_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );   SIZE
// 0x0c1 <0x2d> | 0x115 <0x32> | dm->unregister_body_part_damage_protector( protected_body_patrs[i], &m_damage_protectors[i] );   SIZE
// ; aligned 9, size-diffs 4, quantity-diffs 1
// VERDICT: STRUCTURE MATCH (shape ok; dropped the spurious if-branch braces) - all SIZE diffs are inventory::holder() + intrusive_ptr ctor/operator* inline-vs-call (target out-of-line, base inlines), the accepted accessor LTCG wall; sole quantity-diff is an EMPTY blank-line run. trail: switch_passive_mode_impl.md
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
}

// STRUCTURE DIFF[target 0x57fe90 | base 0x4553b0]: target 6 / base 6 stmts
// 0x009 <0x2c> | 0x009 <0x76> | damage_model_ptr dm = m_inventory->holder( ).damage_model( );   SIZE
// 0x04d <0x1d> | 0x097 <0x28> | body_part_parameters* bp = dm->get_body_part( protected_body_patrs[i] );   SIZE
// ; aligned 4, size-diffs 2, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (6/6 stmts) - both SIZE diffs are accessor inline-vs-call: target keeps inventory::holder(), the intrusive_ptr copy-ctor and operator* out-of-line, base inlines them (the accepted LTCG accessor wall). trail: activate_impl.md
// STATE[12.38%|PARTIAL]: holder()/intrusive_ptr accessor inline-vs-call LTCG wall.
void artefact_lifebone_core::activate_impl( )
{
	damage_model_ptr dm = m_inventory->holder( ).damage_model( );
	for ( u32 i = 0 ; i < DAMAGE_PROTECTORS ; ++i )
	{
		body_part_parameters* bp = dm->get_body_part( protected_body_patrs[i] );
		bp->reset( );
	}
}

// STRUCTURE DIFF[target 0x580080 | base 0x455660]: target 12 / base 12 stmts
// 0x044 <0x16> | 0x044 <0x17> | inventory_item::set_amount( inventory_item::amount( ) - 1 );   SIZE
// ; aligned 11, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (12/12 stmts) - sole SIZE diff is set_amount's LTCG arg-passing convention (target thiscall+stack push w/ movzx u16 cast, base register-call), a bankable call-boundary LTCG diff. trail: action.md
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

// STRUCTURE DIFF[target 0x57fb50 | base 0x455060]: target 4 / base 4 stmts
// 0x01e <0xb> | 0x01e <0xd> | inventory_item::set_amount( (u16)amount );   SIZE
// ; aligned 3, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (4/4 stmts) - sole SIZE diff is set_amount's LTCG arg-passing convention (target thiscall+stack push w/ movzx u16 cast, base register-call), a bankable call-boundary LTCG diff. trail: load_core.md
// STATE[92.12%|DONE]: LTCG arg-passing for set_amount (register vs stack).
void artefact_lifebone_core::load_core( configs::binary_config_value config )
{
	s32 amount		= (s32)config["amount"];
	inventory_item::set_amount( (u16)amount );
	m_unlimited		= amount == -1;
	m_cooldown_ms	= (u32)config["cooldown_ms"];
}

} // namespace survarium
