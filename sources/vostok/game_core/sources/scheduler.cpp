// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/scheduler.h>

namespace survarium {

void scheduler::on_frame( scheduler::record& record, const u32 frame_delta, const u32 current_time )
{
	u32 last_update_time = record.m_last_update_time;
	if ( current_time <= last_update_time )
		return;

	if ( record.m_type == type_on_frame )
	{
		record.m_last_update_time = current_time;
		record.m_callback( frame_delta, current_time );
		return;
	}

	ASSERT( UNKNOWN_EXPRESSION_T( record.m_update_delta != 0 ) );
	u32 update_delta = record.m_update_delta;
	if ( current_time < update_delta + last_update_time )
		return;

	u32 time_delta = current_time - last_update_time;
	s32 count = math::floor( (float)time_delta / (float)update_delta ); // sushi@NOTE: `math::floor` didn't inline in target
	record.m_last_update_time += count * update_delta;
	count = math::min( count, (s32)record.m_max_update_count ); // claude@NOTE: math::min(s32) out-of-line in target, inlined by our /GL (wall); also the callback copy ctor inlines assign_to_own in the target but calls it here (same inline wall)
	ASSERT( UNKNOWN_EXPRESSION_T( record.m_id ) );

	scheduler::identifier* id = record.m_id;
	scheduler::callback_type callback = record.m_callback;
	for ( s32 i = 0 ; i < count && id->m_active ; ++i )
		callback( update_delta, current_time );
}

// claude@MATCH: loop iterates m_active_objects (offset 0x10), not m_inactive_objects (0x00).
void scheduler::on_frame( const u32 frame_delta, const u32 current_time )
{   // claude@NOTE: walled - target keeps vectora<record>::size() and vector::operator[]
	// out-of-line (they exist standalone in the target PDB); our /GL inlines both at the
	// call site ((end-begin)/0x38 and direct index). Not steerable from this TU.
	for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )
		on_frame( m_active_objects[m_current_index], frame_delta, current_time );

	m_current_index = u32(-1);
}

} // namespace survarium
