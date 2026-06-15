////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_model_stats.h"

namespace survarium {

// STATE[STUB]
damage_model_stats::damage_model_stats( ui::world& ui_world ) :
	// ref member from the same-named param; the const colors/widths get
	// buildability zeros - a matcher supplies the real values
	m_ui_world( ui_world ),
	m_caption_color( 0 ),
	m_text_color( 0 ),
	m_line_height( 0.f ),
	m_medium_column_width( 0.f ),
	m_wide_column_width( 0.f )
{
	// CALL SITE INFO
	// <0x70253f> -> ui::window* < unknown >()
	// <0x70254d> -> void < unknown >( bool )
	// <0x702568> -> void < unknown >( float2 const& )
	// <0x702591> -> void < unknown >( float2 const& )
	// ******

	// FUNCTION BODY[0x702500]: 4
	// <0x70253a>|0x03a|+0x00a:'23'
	// <0x702544>|0x044|+0x00b:'24'
	// <0x70254f>|0x04f|+0x01b:'25'
	// <0x70256a>|0x06a|+0x029:'26'
	// ******
}

// STATE[STUB]
 damage_model_stats::~damage_model_stats( )
{
	// CALL SITE INFO
	// <0x7024fb> -> void < unknown >( ui::window* )
	// ******

	// FUNCTION BODY[0x7024f0]: 1
	// <0x7024f0>|0x000|+0x00d:'31'
	// ******
}

} // namespace survarium
