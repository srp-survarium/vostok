////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "rifle_scope.h"

namespace survarium {

// STATE[STUB]
 rifle_scope::rifle_scope(
	render::static_model_ptr const&	idle_scope,
	render::static_model_ptr const&	aimed_scope,
	const float		change_scope_factor,
	const bool		hide_weapon_on_aim,
	const float		fov_factor,
	const float		near_plane_factor
)
	// buildability init (const members)
	:	m_idle_scope			( idle_scope ),
		m_aimed_scope			( aimed_scope ),
		m_change_scope_factor	( change_scope_factor ),
		m_fov_factor			( fov_factor ),
		m_near_plane_factor		( near_plane_factor ),
		m_hide_weapon_on_aim	( hide_weapon_on_aim )
{
	// FUNCTION BODY[0x7b2bc0]: 0
	// <0x7b2bc0>|0x000|+0x08e:'26'	{
	// <0x7b2c4e>|0x08e|      :'27'	}
	// ******
}

} // namespace survarium
