////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/scheduler.h>

namespace survarium {

// STATE[82.17%|DONE]
void scheduler::on_frame( scheduler::record& record, u32 frame_delta, u32 current_time )
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

	// STRUCTURE DIFF[target 0xa8dd0 | base 0x574100]: target 25 / base 25 stmts
	// .. same ..
	// --          | <0>         |    EMPTY only base
	// .. same ..
	// <0>         | --          |    EMPTY only target
	// .. same ..
	// 0x0df <0x11> | 0x0de <0x3f> | count = math::min( count, (s32)record.m_max_update_count );   SIZE
	// .. same ..
	// 0x104 <0xe> | 0x131 <0x17> | scheduler::callback callback = record.m_callback;   SIZE
	// .. same ..
	// ; aligned 22, size-diffs 2, quantity-diffs 2
	// VERDICT: STRUCTURE MATCH (shape ok) - 25/25 stmts aligned; the two SIZE diffs are math::min and the boost::function copy-ctor out-of-lined (call) in target vs inlined in base, an LTCG inline-vs-call decision (drives the esi push / sub esp 2D0h frame), non-steerable. trail: scheduler_on_frame.md
}

// STATE[46.39%|PARTIAL]: body source-correct; residual is LTCG inline-vs-call of
// vectora<record>::size()/operator[] (target out-of-lines, base inlines). trail: scheduler_on_frame.md
// claude@MATCH: loop iterates m_active_objects (offset 0x10), not m_inactive_objects (0x00).
void scheduler::on_frame( u32 frame_delta, u32 current_time )
{   // sushi@NOTE: `size` didn't inline in target
	for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )
		on_frame( m_active_objects[m_current_index], frame_delta, current_time );

	m_current_index = u32(-1);

	// STRUCTURE DIFF[target 0x77de80 | base 0x574290]: target 4 / base 4 stmts
	// 0x00f <0x3d> | 0x009 <0x41> | for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )   SIZE
	// 0x04c <0x2d> | 0x04a <0x28> | on_frame( m_active_objects[m_current_index], frame_delta, current_time );   SIZE
	// .. same ..
	// 0x079 <0xd> | 0x072 <0xa> | m_current_index = u32(-1);   SIZE
	// ; aligned 1, size-diffs 3, quantity-diffs 0
	// VERDICT: STRUCTURE MATCH (shape ok) - 4/4 stmts aligned; all three SIZE diffs are vectora<record>::size()/operator[] out-of-lined (call) in target vs inlined (idiv/imul 0x38) in base, an LTCG inline-vs-call decision, non-steerable. trail: scheduler_on_frame.md
}

} // namespace survarium
