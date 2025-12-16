////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/artefact_lifebone_core.h>

namespace survarium {

// STATE[STUB]
// survarium::artefact_lifebone_core::artefact_lifebone_core()
artefact_lifebone_core::artefact_lifebone_core( )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x58fcb9>|0x0a9|+0x01c|[1]:'33'
	// <0>
	// <0x58fcd5>|0x0c5|+0x0c4:'35'
	// <0x58fd99>|0x189|+0x0e0:'36'
	// <0x58fe79>|0x269|+0x005:'37'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::~artefact_lifebone_core()
artefact_lifebone_core::~artefact_lifebone_core( )
{
	// FUNCTION BODY
	// <0x58fbb0>|0x000|+0x01f:'41'	{
	// <0x58fbcf>|0x01f|      :'42'	}
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::holder_assigned()
void artefact_lifebone_core::holder_assigned( )
{
	// FUNCTION BODY
	// <0x590059>|0x009|+0x00c:'46'
	// <0x590065>|0x015|+0x00a:'47'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::holder_removed()
void artefact_lifebone_core::holder_removed( )
{
	// FUNCTION BODY
	// <0x590027>|0x007|+0x00e:'52'
	// <0x590035>|0x015|+0x00a:'53'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::switch_passive_mode_impl(bool)
void artefact_lifebone_core::switch_passive_mode_impl( bool switch_on )
{
	// LOCALS
	// resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> dm
	// u32 							i<1>
	// u32 							i<1>
	// ******

	// CALL SITE INFO
	// <0x58ff35> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

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

// STATE[STUB]
// void survarium::artefact_lifebone_core::activate_impl()
void artefact_lifebone_core::activate_impl( )
{
	// LOCALS
	// resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> dm
	// u32 							i<1>
	// body_part_parameters* 		bp<2>
	// ******

	// CALL SITE INFO
	// <0x58feb5> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x58fe99>|0x009|+0x02c:'77'
	// <0x58fec5>|0x035|+0x018|[1]:'78'
	// <0>
	// <0x58fedd>|0x04d|+0x01d|[2]:'80'
	// <0x58fefa>|0x06a|+0x008:'81'
	// <0x58ff02>|0x072|+0x002:'82'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::action(bool)
void artefact_lifebone_core::action( bool key_down )
{
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

// STATE[STUB]
// bool survarium::artefact_lifebone_core::protect_affect(char const*, survarium::hit_affects_type_enum)
bool artefact_lifebone_core::protect_affect( pcstr __formal, hit_affects_type_enum affect )
{
	return false;

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

// STATE[STUB]
// float survarium::artefact_lifebone_core::reduce_damage(char const*, char const*, const float, const float)
float artefact_lifebone_core::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing
)
{
	return 0.0f;

	// FUNCTION BODY
	// <0x58fb09>|0x009|+0x03d:'122'
	// <0x58fb46>|0x046|+0x003:'123'
	// ******
}

// STATE[STUB]
// void survarium::artefact_lifebone_core::load_core(vostok::configs::binary_config_value)
void artefact_lifebone_core::load_core( configs::binary_config_value config )
{
	// LOCALS
	// s32 							amount
	// ******

	// FUNCTION BODY
	// <0x58fb59>|0x009|+0x015:'128'
	// <0x58fb6e>|0x01e|+0x00b:'129'
	// <0x58fb79>|0x029|+0x012:'130'
	// <0x58fb8b>|0x03b|+0x01b:'131'
	// ******
}

} // namespace survarium
