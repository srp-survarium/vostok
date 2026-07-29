////////////////////////////////////////////////////////////////////////////
//	Created		: 27.04.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/sound/sound_object_commands.h>

namespace vostok {
namespace sound {

sound_order::sound_order		( ) :
	m_next_for_orders			( 0 ),
	m_next_for_postponed_orders	( 0 )
{
}

sound_order::~sound_order		( )
{
}

void sound_order::execute	( )
{
}

sound_response::sound_response	( ) :
	m_next						( 0 )
{
}

sound_response::~sound_response	( )
{
}

void sound_response::execute	( )
{
}

} // namespace sound
} // namespace vostok
