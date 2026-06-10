////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/animation_analysis_result.h>

namespace survarium {

// STATE[100%|DONE]
animation_analysis_result::animation_analysis_result( u32 legs_count ) :
	m_buffer		( VOSTOK_MALLOC_IMPL( g_allocator, sizeof( leg_key_times ) * legs_count, "animation_result_legs" ) ),
	m_leg_key_times	( m_buffer, legs_count, legs_count )
{
}

// STATE[100%|DONE]
animation_analysis_result::~animation_analysis_result( )
{
	m_leg_key_times.clear( );
	VOSTOK_FREE_IMPL( g_allocator, m_buffer );
}

} // namespace survarium
