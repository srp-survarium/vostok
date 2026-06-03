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

	// FUNCTION BODY[0xb8dd0]: 26
	// <0xb8de0>|0x000|0x000:'16'
	// <0xb8de9>|0x009|0x009:'17'
	// <0xb8df1>|0x011|0x008:'18'
	// <1>
	// <0xb8df6>|0x016|0x005:'20'
	// <0xb8e04>|0x024|0x00e:'21'
	// <0xb8e0d>|0x02d|0x009:'22'
	// <0xb8e20>|0x040|0x013:'23'
	// <1>
	// <2>
	// <0xb8e25>|0x045|0x005:'26'
	// <1>
	// <0xb8e31>|0x051|0x00c:'28' u32 update_delta = record.m_update_delta;
	// <0xb8e40>|0x060|0x00f:'29' if ( current_time < update_delta + last_update_time )
	// <0xb8e4b>|0x06b|0x00b:'30'	return;
	// <1>
	// <0xb8e50>|0x070|0x005:'32' u32 time_delta = current_time - last_update_time;
	// <0xb8e59>|0x079|0x009:'33'
	// <0xb8e9c>|0x0bc|0x043:'34'
	// <0xb8eaf>|0x0cf|0x013:'35'
	// <0xb8ec0>|0x0e0|0x011:'36' ASSERT
	// <1>
	// <0xb8ecc>|0x0ec|0x00c:'38' scheduler::identifier* id = record.m_id;
	// <0xb8ed4>|0x0f4|0x008:'39' scheduler::callback callback = record.m_callback;
	// <0xb8ee2>|0x102|0x00e|[1]:'40'
	// <0xb8f09>|0x129|0x027:'41'	 callback( update_delta, current_time );
	// ******
}

// STATE[46.39%|PARTIAL]: body source-correct; residual diff is LTCG inlining.
// claude@MATCH: loop iterates over m_active_objects (offset 0x10), not
// m_inactive_objects (offset 0x00) - target's size()/operator[] both read
// &m_active_objects. The prior STUB had the wrong container in the condition
// (it read m_inactive_objects, offset 0x00); fixing it dropped the fuzzy %
// (52->46) only because the now-correct offsets are reached via the inlined
// body while target out-of-lines the calls (see NOTE below).
// claude@NOTE: target keeps vectora<record>::size (0x083010) and
// vector<record>::operator[] (0x082ed0) out-of-line and *calls* them; our base
// (still /Od) inlines both (idiv/imul 0x38), and base has no out-of-line
// vectora<record>::size at all. This emit-and-call vs inline of a trivial COMDAT
// template method is an LTCG decision we cannot steer from this function's
// source - the body, loop, member access and call args otherwise match. PARTIAL.
void scheduler::on_frame( u32 frame_delta, u32 current_time )
{   // sushi@NOTE: `size` didn't inline in target
	for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )
		on_frame( m_active_objects[m_current_index], frame_delta, current_time );

	m_current_index = u32(-1);

	// FUNCTION BODY[0x78de80]: 7
	// <1>
	// <2>
	// <3>
	// <0x78de8f>|0x000|0x000:'49'
	// <0x78decc>|0x03d|0x03d:'50'
	// <1>
	// <0x78def9>|0x06a|0x02d:'52'
	// ******
}

} // namespace survarium
