////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/scheduler.h>

namespace survarium {

// STATE[82.17%|DONE]
void scheduler::on_frame( scheduler::record& record, const u32 frame_delta, const u32 current_time )
{
	u32 last_update_time = record.m_last_update_time;
	if ( current_time <= last_update_time )
		return;

	if ( record.m_type == 0 )
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
	record.m_last_update_time += update_delta * count;
	count = math::min( count, (s32)record.m_max_update_count );
	ASSERT( UNKNOWN_EXPRESSION_T( record.m_id ) );

	scheduler::identifier* id = record.m_id;
	scheduler::callback callback = record.m_callback;
	for ( s32 i = 0 ; i < count && id->m_active ; ++i )
		callback( update_delta, current_time );

	// STRUCTURE DIFF: target 20 / base 20 stmts
	// SIZE +0x2e | 32 | count = math::min( count, (s32)record.m_max_update_count );
	// SIZE +0x9  | 36 | scheduler::callback callback = record.m_callback;
	// VERDICT: STRUCTURE MATCH - both SIZE rows are math::min / boost::function copy out-of-lined (call) in target vs inlined in base, LTCG inline-vs-call, non-steerable.
}

// STATE[46.39%|PARTIAL]: body source-correct; residual is LTCG inline-vs-call of
// vectora<record>::size()/operator[] (target out-of-lines, base inlines). trail: scheduler_on_frame.md
// claude@MATCH: loop iterates m_active_objects (offset 0x10), not m_inactive_objects (0x00).
void scheduler::on_frame( const u32 frame_delta, const u32 current_time )
{   // sushi@NOTE: `size` didn't inline in target
	for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )
		on_frame( m_active_objects[m_current_index], frame_delta, current_time );

	m_current_index = u32(-1);

	// STRUCTURE DIFF: target 3 / base 3 stmts
	// SIZE +0x4 | 59 | for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )
	// SIZE -0x5 | 60 | on_frame( m_active_objects[m_current_index], frame_delta, current_time );
	// SIZE -0x3 | 62 | m_current_index = u32(-1);
	// VERDICT: STRUCTURE MATCH - all rows are vectora<record>::size()/operator[] out-of-lined in target vs inlined (imul 0x38) in base + the resulting frame-size disp noise, LTCG inline-vs-call, non-steerable.
}

} // namespace survarium
