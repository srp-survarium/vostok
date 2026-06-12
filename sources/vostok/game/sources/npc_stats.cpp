////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "npc_stats.h"

namespace survarium {

// STATE[STUB]
// no init-list yet (ref + const members); a matcher supplies the real values
// when this TU is enabled
 npc_stats::npc_stats( ui::world& ui_world )
{
	// CALL SITE INFO
	// <0x5cbcff> -> ui::window* < unknown >()
	// <0x5cbd0d> -> void < unknown >( bool )
	// <0x5cbd28> -> void < unknown >( float2 const& )
	// <0x5cbd51> -> void < unknown >( float2 const& )
	// ******

	// FUNCTION BODY[0x5cbcc0]: 4
	// <0x5cbcfa>|0x03a|+0x00a:'25'
	// <0x5cbd04>|0x044|+0x00b:'26'
	// <0x5cbd0f>|0x04f|+0x01b:'27'
	// <0x5cbd2a>|0x06a|+0x029:'28'
	// ******
}

// STATE[STUB]
ui::text* npc_stats::create_new_group(
	const npc_stats::column_types_enum		arg_0 /* npc_stats::column_types_enum column_number */,
	const u32								font_color,
	pcstr									text,
	ui::window const*						upper_window
)
{
	// LOCALS
	// const float[4] 					offsets
	// float2 							position
	// const float 						column_width
	// ******

	// CALL SITE INFO
	// <0x5cbbc1> -> ui::text* < unknown >()
	// <0x5cbbcc> -> ui::window* < unknown >()
	// <0x5cbbd7> -> void < unknown >( bool )
	// <0x5cbbe4> -> float2 const& < unknown >() const
	// <0x5cbbf4> -> float2 const& < unknown >() const
	// <0x5cbc21> -> ui::window* < unknown >()
	// <0x5cbc2e> -> void < unknown >( float2 const& )
	// <0x5cbc37> -> ui::window* < unknown >()
	// <0x5cbc5c> -> void < unknown >( float2 const& )
	// <0x5cbc66> -> void < unknown >( ui::enum_font )
	// <0x5cbc71> -> void < unknown >( ui::enum_text_mode )
	// <0x5cbc7f> -> void < unknown >( u32 )
	// <0x5cbc8f> -> ui::window* < unknown >()
	// <0x5cbc98> -> void < unknown >( ui::window*, bool )
	// <0x5cbca6> -> void < unknown >( pcstr )
	// ******

	return NULL;

	// FUNCTION BODY[0x5cbb60]: 24
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cbb63>|0x003|+0x003:'42'
	// <0>
	// <1>
	// <2>
	// <0x5cbb66>|0x006|+0x03c:'46'
	// <0>
	// <0x5cbba2>|0x042|+0x018:'48'
	// <0x5cbbba>|0x05a|+0x00b:'49'
	// <0x5cbbc5>|0x065|+0x014:'50'
	// <0>
	// <0x5cbbd9>|0x079|+0x041:'52'
	// <0x5cbc1a>|0x0ba|+0x016:'53'
	// <0x5cbc30>|0x0d0|+0x02e:'54'
	// <0x5cbc5e>|0x0fe|+0x00a:'55'
	// <0x5cbc68>|0x108|+0x00b:'56'
	// <0x5cbc73>|0x113|+0x00e:'57'
	// <0x5cbc81>|0x121|+0x019:'58'
	// <0x5cbc9a>|0x13a|+0x00e:'59'
	// <0>
	// <0x5cbca8>|0x148|+0x002:'61'
	// ******
}

// STATE[STUB]
 npc_stats::~npc_stats( )
{
	// CALL SITE INFO
	// <0x5cbb5b> -> void < unknown >( ui::window* )
	// ******

	// FUNCTION BODY[0x5cbb50]: 1
	// <0x5cbb50>|0x000|+0x00d:'66'
	// ******
}

// STATE[STUB]
void npc_stats::draw( render::ui::renderer& ui_renderer, render::base_scene_view_ptr const& scene_view )
{
	// CALL SITE INFO
	// <0x5cbb48> -> void < unknown >( render::ui::renderer&, render::base_scene_view_ptr const& )
	// ******

	// FUNCTION BODY[0x5cbb40]: 0
	// <0x5cbb40>|0x000|+0x000:'70'	{
	// <0x5cbb40>|0x000|      :'71'	}
	// ******
}

// STATE[STUB]
void npc_stats::set_stats( human_npc const* const owner )
{
	// LOCALS
	// ui::text* 						last_item
	// ai::npc_statistics 				stats
	// u32 								i
	// u32 								j
	// u32 								i
	// u32 								j
	// u32 								i
	// u32 								j
	// ******

	// CALL SITE INFO
	// <0x5cbd7f> -> void < unknown >()
	// <0x5cbddb> -> ui::window* < unknown >()
	// <0x5cbe36> -> ui::window* < unknown >()
	// <0x5cbedc> -> ui::window* < unknown >()
	// <0x5cbf3d> -> ui::window* < unknown >()
	// <0x5cbfe9> -> ui::window* < unknown >()
	// <0x5cc038> -> ui::window* < unknown >()
	// <0x5cc089> -> ui::window* < unknown >()
	// <0x5cc119> -> ui::window* < unknown >()
	// <0x5cc19f> -> ui::window* < unknown >()
	// <0x5cc205> -> ui::window* < unknown >()
	// ******

	// FUNCTION BODY[0x5cbd60]: 124
	// <0x5cbd77>|0x017|+0x00a:'76'
	// <0>
	// <0x5cbd81>|0x021|+0x008:'78'
	// <0>
	// <1>
	// <0x5cbd89>|0x029|+0x047:'81'
	// <0x5cbdd0>|0x070|-0x03e:'81'
	// <0x5cbd92>|0x032|+0x00c:'82'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cbd9e>|0x03e|+0x0cb:'87'
	// <0x5cbe69>|0x109|-0x097:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cbdd2>|0x072|+0x026:'94'
	// <0x5cbdf8>|0x098|+0x035:'95'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cbe2d>|0x0cd|+0x06b:'102'
	// <0>
	// <1>
	// <2>
	// <0x5cbe98>|0x138|+0x0d7:'106'
	// <0x5cbf6f>|0x20f|-0x09c:'106'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cbed3>|0x173|+0x029:'113'
	// <0x5cbefc>|0x19c|+0x038:'114'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cbf34>|0x1d4|+0x06d:'121'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x5cbfa1>|0x241|+0x03a:'133'
	// <0x5cbfdb>|0x27b|-0x025:'133'
	// <0x5cbfb6>|0x256|+0x02a:'134'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cbfe0>|0x280|+0x04f:'141'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5cc02f>|0x2cf|+0x048:'149'
	// <0x5cc077>|0x317|-0x025:'149'
	// <0x5cc052>|0x2f2|+0x02e:'150'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cc080>|0x320|+0x04f:'157'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x5cc0cf>|0x36f|+0x03a:'168'
	// <0x5cc109>|0x3a9|-0x025:'168'
	// <0x5cc0e4>|0x384|+0x02c:'169'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cc110>|0x3b0|+0x07e:'176'
	// <0>
	// <1>
	// <0x5cc18e>|0x42e|-0x02f:'179'
	// <0>
	// <1>
	// <0x5cc15f>|0x3ff|+0x0dc:'182'
	// <0x5cc23b>|0x4db|-0x0a5:'182'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cc196>|0x436|+0x02a:'189'
	// <0x5cc1c0>|0x460|+0x038:'190'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cc1f8>|0x498|+0x073:'197'
	// <0>
	// <1>
	// ******
}

} // namespace survarium
