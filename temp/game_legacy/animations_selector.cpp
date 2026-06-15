////////////////////////////////////////////////////////////////////////////
//	Created		: 21.11.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "animations_selector.h"
#include <vostok/ai/animation_item.h>
#include <vostok/ai/movement_target.h>

namespace survarium {

animations_selector::animations_selector	( vostok::animation::animation_player& player ) :
	m_animation_player						( player ),
	m_current_controller					( 0 ),
	m_target_controller						( 0 ),
	m_target_controller_parameters			( 0 )
{
}

} // namespace survarium