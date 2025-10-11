////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "artefact_lifebone_core.h"

namespace survarium {

// STATE[STUB]
// survarium::artefact_lifebone_core::artefact_lifebone_core()
artefact_lifebone_core::artefact_lifebone_core( )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x58fcb9>|0x000|0x000|[1]:'33'
	// 1
	// <0x58fcd5>|0x01c|0x01c:'35'
	// <0x58fd99>|0x0e0|0x0c4:'36'
	// <0x58fe79>|0x1c0|0x0e0:'37'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::~artefact_lifebone_core()
void artefact_lifebone_core::~artefact_lifebone_core( )
{
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::holder_assigned()
void artefact_lifebone_core::holder_assigned( )
{
	// FUNCTION BODY
	// <0x590059>|0x000|0x000:'46'
	// <0x590065>|0x00c|0x00c:'47'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::holder_removed()
void artefact_lifebone_core::holder_removed( )
{
	// FUNCTION BODY
	// <0x590027>|0x000|0x000:'52'
	// <0x590035>|0x00e|0x00e:'53'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::switch_passive_mode_impl(bool)
void artefact_lifebone_core::switch_passive_mode_impl( bool switch_on )
{
	// LOCALS
	// vostok::resources::resource_ptr<damage_model,vostok::resources::unmanaged_intrusive_base> dm
	// u32 							i<1>
	// u32 							i<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57ef35 }, type_index: TypeIndex(0xa828) })
	// ******

	// FUNCTION BODY
	// <0x58ff19>|0x000|0x000:'58'
	// 1
	// <0x58ff45>|0x02c|0x02c:'60'
	// 1
	// <0x58ff4d>|0x034|0x008|[1]:'62'
	// 1
	// <0x58ff65>|0x04c|0x018:'64'
	// <0x58ff8a>|0x071|0x025:'65'
	// <0x58ffb7>|0x09e|0x02d:'66'
	// 1
	// 2
	// <0x58ffb9>|0x0a0|0x002|[1]:'69'
	// <0x58ffd1>|0x0b8|0x018:'70'
	// 1
	// <0x58fffe>|0x0e5|0x02d:'72'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::activate_impl()
void artefact_lifebone_core::activate_impl( )
{
	// LOCALS
	// vostok::resources::resource_ptr<damage_model,vostok::resources::unmanaged_intrusive_base> dm
	// u32 							i<1>
	// body_part_parameters* 		bp<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x57eeb5 }, type_index: TypeIndex(0xa828) })
	// ******

	// FUNCTION BODY
	// <0x58fe99>|0x000|0x000:'77'
	// <0x58fec5>|0x02c|0x02c|[1]:'78'
	// 1
	// <0x58fedd>|0x044|0x018|[2]:'80'
	// <0x58fefa>|0x061|0x01d:'81'
	// <0x58ff02>|0x069|0x008:'82'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::action(bool)
void artefact_lifebone_core::action( bool key_down )
{
	// FUNCTION BODY
	// <0x590087>|0x000|0x000:'87'
	// <0x59008f>|0x008|0x008:'88'
	// 1
	// <0x590091>|0x00a|0x002:'90'
	// 1
	// <0x5900ae>|0x027|0x01d:'92'
	// <0x5900b6>|0x02f|0x008:'93'
	// <0x5900c4>|0x03d|0x00e:'94'
	// 1
	// 2
	// <0x5900da>|0x053|0x016:'97'
	// 1
	// <0x5900f7>|0x070|0x01d:'99'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// bool survarium::artefact_lifebone_core::protect_affect(char const*, survarium::hit_affects_type_enum)
bool artefact_lifebone_core::protect_affect( pcstr __formal, hit_affects_type_enum affect )
{
	return false;
	// FUNCTION BODY
	// <0x58fad9>|0x000|0x000:'106'
	// 1
	// 2
	// 3
	// <0x58faed>|0x014|0x014:'110'
	// <0x58faf1>|0x018|0x004:'111'
	// 1
	// <0x58faf3>|0x01a|0x002:'113'
	// 1
	// ******
}

// STATE[STUB]
// float survarium::artefact_lifebone_core::reduce_damage(char const*, char const*, const float, const float)
float artefact_lifebone_core::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing)
{
	return 0.0f;
	// FUNCTION BODY
	// <0x58fb09>|0x000|0x000:'122'
	// <0x58fb46>|0x03d|0x03d:'123'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::load_core(vostok::configs::binary_config_value)
void artefact_lifebone_core::load_core( vostok::configs::binary_config_value config )
{
	// LOCALS
	// s32 							amount
	// ******

	// FUNCTION BODY
	// <0x58fb59>|0x000|0x000:'128'
	// <0x58fb6e>|0x015|0x015:'129'
	// <0x58fb79>|0x020|0x00b:'130'
	// <0x58fb8b>|0x032|0x012:'131'
	// ******
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium
