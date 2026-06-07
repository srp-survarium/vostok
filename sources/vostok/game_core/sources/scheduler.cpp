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

	// FUNCTION BODY
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

	// STRUCTURE DIFF[target 0x77de80 | base 0x573f20]: target 5 / base 6 stmts
	// .. same ..
	// 0x00f <0x3d> | 0x009 <0x41> | for ( m_current_index = 0 ; m_current_index < m_active_objects.size( ) ; ++m_current_index )   SIZE
	// 0x04c <0x2d> | 0x04a <0x28> | on_frame( m_active_objects[m_current_index], frame_delta, current_time );   SIZE
	// --          | <0>         |    EMPTY only base
	// --          | 0x072 <0xa> | m_current_index = u32(-1);   ONLY base
	// .. same ..
	// 0x079 <0xd> | --          | L52   ONLY target
	// ; aligned 2, size-diffs 2, quantity-diffs 3
	// claude@STRUCTURE: shape matches. The two SIZE diffs are the LTCG inline-vs-call
	// already documented above (size()/operator[]). The "ONLY base m_current_index =
	// u32(-1)" + "ONLY target L52" pair is one statement double-attributed: the final
	// mov dword ptr [eax+28h], 0FFFFFFFFh is present in BOTH disassemblies (diff view),
	// just charged to a different srcline each side. Not a real quantity divergence.
}

} // namespace survarium
