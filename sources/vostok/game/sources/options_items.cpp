////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "options_gamma_selector.h"
#include "options_graphics_quality_selector.h"
#include "options_item_base.h"
#include "options_item_bool.h"
#include "options_item_float.h"
#include "options_item_int.h"
#include "options_monitor_index_selector.h"
#include "options_resolution_selector.h"
#include "options_tab.h"
#include "game.h"
#include "text_translator.h"
#include "game_memory.h"
#include <vostok/console_command_processor.h>
#include <vostok/console_command.h>
#include <vostok/fixed_string.h>
#include <vostok/scaleform/sources/flash_value.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_function_handler_params.h>

namespace survarium {

// claude@NOTE: structure matches (3 stmts: find, null-check, LOG_ERROR). Byte
// residual is the flash_function_handler base ctor, which the target INLINES (the
// Scaleform GlobalHeap alloc + flash_function_handler_impl construction); our
// scaleform::flash_function_handler::flash_function_handler is an out-of-line stub
// in value.cpp so the compiler emits a call instead - a cross-module scaleform cap.
 options_item_base::options_item_base(
	options_tab&				parent_tab,
	pcstr						console_command,
	u8							option_item_id,
	option_item_type_enum		type
)
	: m_type( type )
	, m_parent_tab( parent_tab )
	, m_option_item_id( option_item_id )
{
	m_console_command = console_commands::find( console_command );

	if ( m_console_command == NULL )
		LOG_ERROR( "Console command [%s] not found for option item", console_command );
}

// claude@NOTE: structure matches (flash_value[4] + 3 SetUInt + fill_value + Invoke).
// Byte residual is the inlined scaleform flash_value setters/dtors + the inlined
// flash_movie::Invoke (-> Scaleform::GFx::Movie::Invoke), an SDK cap we do not
// reproduce. Matching this (real body, not inlined) also makes the derived
// options_item_{int,float,bool}::revert emit their tail-jmp to it.
void options_item_base::revert( )
{
	flash_value source_data[ 4 ];

	source_data[ 0 ].SetUInt( m_parent_tab.type( ) );
	source_data[ 1 ].SetUInt( m_option_item_id );
	fill_value( source_data[ 2 ] );
	source_data[ 3 ].SetUInt( 1 );

	m_parent_tab.get_movie( )->movie->Invoke( "root.set_value", NULL, source_data, 4 );
}

 options_item_int::options_item_int(
	options_tab&	parent_tab,
	pcstr			console_command,
	u8				option_item_id,
	pcstr*			values,
	u8				values_count
)
	: options_item_base( parent_tab, console_command, option_item_id, string_selector )
	, m_values( values )
	, m_values_count( values_count )
{
}

void options_item_int::initialize( )
{
	m_source_value	= m_console_command ? ( u8 )( ( console_commands::cc_u32* )m_console_command )->get_value( ) : 0;
	m_current_value	= m_source_value;
}

void options_item_int::fill_data( flash_value& val )
{
	for ( u8 i = 0; i < m_values_count; ++i )
	{
		wchar_t val_txt[ 512 ];
		m_parent_tab.get_game( ).text_translator( ).translate_text( m_values[ i ], val_txt );

		flash_value str_val;
		str_val.SetStringW( val_txt );

		val.SetElement( i, str_val );
	}
}

// claude@NOTE: scaleform cap (applies to every flash_value setter call in this TU -
// fill_value/fill_data/apply/call/revert). The target INLINES flash_value::SetUInt/
// SetNumber/SetBoolean/... (canonical flash_value.h declares them inline; the GFx::
// Value body lives in the Scaleform SDK we do not reproduce). Our value.cpp has them
// out-of-line, so the compiler emits a call - byte residual on an otherwise-correct
// 1-statement structure.
void options_item_int::fill_value( flash_value& val )
{
	val.SetUInt( m_current_value );
}

void options_item_int::apply( )
{
	m_source_value = m_current_value;

	if ( m_console_command )
	{
		fixed_string< 8 > args;
		args.assignf( "%d", m_current_value );
		m_console_command->execute( args.c_str( ) );
	}
}

void options_item_int::revert( )
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

// STATE[STUB]
void options_item_int::call( flash_function_handler_params& params )
{
	// LOCALS
	// flash_value[4] 					new_resolution_data
	// ******

	// FUNCTION BODY[0x5ca1f0]: 34
	// <0x5ca1f0>|0x000|+0x011:'185'
	// <0>
	// <0x5ca201>|0x011|+0x00d:'187'
	// <0>
	// <1>
	// <0x5ca20e>|0x01e|+0x008:'190'
	// <0>
	// <0x5ca216>|0x026|+0x004:'192'
	// <0>
	// <0x5ca21a>|0x02a|+0x037:'194'
	// <0x5ca251>|0x061|-0x021:'194'
	// <0>
	// <0x5ca230>|0x040|+0x005:'196'
	// <0x5ca235>|0x045|+0x06e:'197'
	// <0>
	// <0x5ca2a3>|0x0b3|+0x017:'199'
	// <0x5ca2ba>|0x0ca|+0x02a:'200'
	// <0x5ca2e4>|0x0f4|+0x030:'201'
	// <0x5ca314>|0x124|+0x030:'202'
	// <0x5ca344>|0x154|+0x030:'203'
	// <0>
	// <0x5ca374>|0x184|-0x11c:'205'
	// <0x5ca258>|0x068|+0x146:'206'
	// <0x5ca39e>|0x1ae|-0x144:'206'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5ca25a>|0x06a|+0x00c:'213'
	// <0x5ca266>|0x076|+0x005:'214'
	// <0>
	// <1>
	// <2>
	// <0x5ca26b>|0x07b|+0x032:'218'
	// ******
}

 options_item_float::options_item_float(
	options_tab&	parent_tab,
	pcstr			console_command,
	u8				option_item_id,
	float			step
)
	: options_item_base( parent_tab, console_command, option_item_id, slider_selector )
	, m_step( step )
{
}

void options_item_float::initialize( )
{
	m_source_value	= m_console_command ? ( ( console_commands::cc_value< float >* )m_console_command )->get_value( ) : 0.0f;
	m_current_value	= m_source_value;
}

// claude@NOTE: structure is the 3 SetMember pairs (snapInterval/minimum/maximum).
// Base shows one extra statement vs target: the single reused slider_data_member's
// default ctor folds into line 245 in the target (flash_value ctor inlined there) but
// emits a distinct statement here because our flash_value ctor is out-of-line - the
// same scaleform cap that bounds the bytes.
void options_item_float::fill_data( flash_value& val )
{
	flash_value slider_data_member;

	slider_data_member.SetNumber( m_step );
	val.SetMember( "snapInterval", slider_data_member );

	slider_data_member.SetNumber( m_console_command ? ( ( console_commands::cc_value< float >* )m_console_command )->get_min( ) : 0.0f );
	val.SetMember( "minimum", slider_data_member );

	slider_data_member.SetNumber( m_console_command ? ( ( console_commands::cc_value< float >* )m_console_command )->get_max( ) : 100.0f );
	val.SetMember( "maximum", slider_data_member );
}

void options_item_float::fill_value( flash_value& val )
{
	val.SetNumber( m_current_value );
}

void options_item_float::apply( )
{
	m_source_value = m_current_value;

	if ( m_console_command )
	{
		fixed_string< 8 > args;
		args.assignf( "%.2f", m_current_value );
		m_console_command->execute( args.c_str( ) );
	}
}

void options_item_float::revert( )
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_float::call( flash_function_handler_params& params )
{
	m_current_value = ( float )params.pArgs[ 0 ].GetNumber( );
	params.pRetVal->SetNumber( m_current_value );
}

 options_item_bool::options_item_bool( options_tab& parent_tab, pcstr console_command, u8 option_item_id )
	: options_item_base( parent_tab, console_command, option_item_id, bool_selector )
{
}

void options_item_bool::initialize( )
{
	m_source_value	= m_console_command ? ( ( console_commands::cc_value< bool >* )m_console_command )->get_value( ) : false;
	m_current_value	= m_source_value;
}

void options_item_bool::fill_data( flash_value& __formal )
{
	VOSTOK_UNREFERENCED_PARAMETER( __formal );
}

void options_item_bool::fill_value( flash_value& val )
{
	val.SetBoolean( m_current_value );
}

void options_item_bool::apply( )
{
	m_source_value = m_current_value;

	if ( m_console_command )
	{
		fixed_string< 8 > args;
		args.assignf( "%d", m_current_value );
		m_console_command->execute( args.c_str( ) );
	}
}

void options_item_bool::revert( )
{
	m_current_value = m_source_value;
	options_item_base::revert( );
}

void options_item_bool::call( flash_function_handler_params& params )
{
	params.pRetVal->SetBoolean( m_current_value = params.pArgs[ 0 ].GetBool( ) );
}

// claude@NOTE: STATE[STUB]. Body reads params.pArgs[0].GetNumber() into
// m_current_value then calls the same render gamma chain as revert - blocked by the
// same missing render::scene_renderer::set_gamma_correction_factor (cross-module
// render cap).
void options_gamma_selector::call( flash_function_handler_params& params )
{
}

// claude@NOTE: STATE[STUB]. Body is options_item_float::revert() then
// m_parent_tab.get_game().renderer().scene().set_gamma_correction_factor(m_current_value),
// but render::scene_renderer::set_gamma_correction_factor(const float) is in the
// canonical render structure yet NOT declared/defined in our render module
// (render/facade scene_renderer.h+.cpp) - calling it would LNK2001. Cross-module
// render cap: blocked until the render module exposes set_gamma_correction_factor.
void options_gamma_selector::revert( )
{
}

// STATE[STUB]
 options_resolution_selector::options_resolution_selector( options_tab& parent_tab )
	// buildability: base has no default ctor; matcher supplies real args
	: options_item_int( parent_tab, NULL, 0, NULL, 0 )
{
	// FUNCTION BODY[0x5cb1e0]: 5
	// <0x5cb217>|0x037|+0x00a:'355'
	// <0>
	// <0x5cb221>|0x041|+0x003:'357'
	// <0>
	// <0x5cb224>|0x044|+0x00a:'359'
	// ******
}

// STATE[STUB]
void options_resolution_selector::fill_resolutions( u8 monitor_number )
{
	// LOCALS
	// fixed_string< 32 > 				old_resolution
	// u32 								old_resolution_index
	// flash_value[4] 					new_resolution_data
	// ******

	// FUNCTION BODY[0x5ca9e0]: 41
	// <0x5ca9e0>|0x000|+0x00e:'368'	{
	// <0x5ca9ee>|0x00e|+0x005:'369'
	// <0>
	// <0x5ca9f3>|0x013|+0x029:'371'
	// <0x5caa1c>|0x03c|+0x03f:'372'
	// <0x5caa5b>|0x07b|+0x022:'373'
	// <0>
	// <1>
	// <0x5caa7d>|0x09d|+0x017:'376'
	// <0>
	// <1>
	// <0x5caa94>|0x0b4|+0x003:'379'
	// <0x5caa97>|0x0b7|+0x012:'380'
	// <0>
	// <1>
	// <0x5caaa9>|0x0c9|+0x026:'383'
	// <0>
	// <1>
	// <0x5caacf>|0x0ef|+0x064:'386'
	// <0x5cab33>|0x153|-0x017:'386'
	// <0>
	// <1>
	// <0x5cab1c>|0x13c|+0x061:'389'
	// <0x5cab7d>|0x19d|-0x047:'389'
	// <0>
	// <0x5cab36>|0x156|+0x009:'391'
	// <0>
	// <0x5cab3f>|0x15f|+0x03a:'393'
	// <0x5cab79>|0x199|+0x00e:'394'
	// <0>
	// <0x5cab87>|0x1a7|+0x03a:'396'
	// <0x5cabc1>|0x1e1|+0x00d:'397'
	// <0x5cabce>|0x1ee|+0x00b:'398'
	// <0x5cabd9>|0x1f9|+0x01a:'399'
	// <0x5cabf3>|0x213|+0x025:'400'
	// <0x5cac18>|0x238|+0x036:'401'
	// <0x5cac4e>|0x26e|+0x038:'402'
	// <0x5cac86>|0x2a6|+0x03f:'403'
	// <0>
	// <0x5cacc5>|0x2e5|+0x035:'405'
	// <0x5cacfa>|0x31a|+0x043:'406'
	// <0>
	// <0x5cad3d>|0x35d|-0x009:'408'
	// <0>
	// <0x5cad34>|0x354|+0x00f:'410'
	// <0x5cad43>|0x363|      :'410'	}
	// ******
}

// STATE[STUB]
void options_resolution_selector::initialize( )
{
	// FUNCTION BODY[0x5ca6a0]: 10
	// <0x5ca6a0>|0x000|+0x003:'413'	{
	// <0x5ca6a3>|0x003|+0x003:'414'
	// <0>
	// <0x5ca6a6>|0x006|+0x010:'416'
	// <0>
	// <0x5ca6b6>|0x016|+0x048:'418'
	// <0x5ca6fe>|0x05e|-0x00e:'419'
	// <0>
	// <1>
	// <2>
	// <0x5ca6f0>|0x050|+0x011:'423'
	// <0x5ca701>|0x061|+0x005:'423'
	// <0x5ca706>|0x066|-0x00c:'423'
	// <0x5ca6fa>|0x05a|+0x00b:'424'
	// <0x5ca705>|0x065|+0x009:'424'
	// <0x5ca70e>|0x06e|      :'424'	}
	// ******
}

void options_resolution_selector::apply( )
{
	m_source_value = m_current_value;
	m_console_command->execute( m_values[ m_current_value ] );
}

// STATE[STUB]
 options_monitor_index_selector::options_monitor_index_selector( options_tab& parent_tab )
	// buildability: base has no default ctor; matcher supplies real args
	: options_item_int( parent_tab, NULL, 0, NULL, 0 )
{
	// FUNCTION BODY[0x5cb100]: 12
	// <0>
	// <1>
	// <0x5cb140>|0x040|+0x018:'438'
	// <0>
	// <1>
	// <0x5cb158>|0x058|+0x04c:'441'
	// <0x5cb1a4>|0x0a4|+0x009:'442'
	// <0>
	// <0x5cb1ad>|0x0ad|+0x00b:'444'
	// <0>
	// <0x5cb1b8>|0x0b8|+0x018:'446'
	// <0>
	// ******
}

void options_monitor_index_selector::call( flash_function_handler_params& params )
{
	options_item_int::call( params );
	refill_resolutions_data( );
}

void options_monitor_index_selector::revert( )
{
	options_item_int::revert( ); refill_resolutions_data( );
}

// claude@NOTE: refill_resolutions_data is STRUCTURE MATCH; revert/call inline it.
// Byte residual: fill_resolutions is still a STUB (render-blocked - it enumerates
// resolutions via render display-mode data we do not expose), and refill_item_data is
// tail-called with this passed in eax + the two u8 args elided by LTCG (the allowed
// argument-passing exception). refill_item_data itself is scaleform-capped in
// game_options.cpp.
void options_monitor_index_selector::refill_resolutions_data( )
{
	( ( options_resolution_selector* )m_parent_tab.option_by_id( 1 ) )->fill_resolutions( m_current_value );
	m_parent_tab.get_game( ).get_game_options( ).refill_item_data( m_parent_tab.type( ), m_option_item_id );
}

// claude@NOTE: graphics_quality_data is a .rdata pcstr[6] table absent from the
// canonical dump; the exact preset strings are NOT recoverable from the available
// binary/PDB, so these placeholders are best-guesses. The ctor match only needs the
// table SYMBOL (it relocates the pointer); the table's own string bytes score as a
// separate data symbol and will not match until the real strings are recovered.
static pcstr graphics_quality_data[ 6 ] =
{
	"minimal",
	"low",
	"medium",
	"high",
	"max",
	"extreme"
};

 options_graphics_quality_selector::options_graphics_quality_selector( options_tab& parent_tab )
	: options_item_int( parent_tab, "r_graphics_quality", 8, graphics_quality_data, 6 )
{
}

// STATE[STUB]
void options_graphics_quality_selector::call( flash_function_handler_params& params )
{
	// LOCALS
	// flash_value[4] 					new_resolution_data
	// ******

	// FUNCTION BODY[0x5ca4f0]: 17
	// <0x5ca4f0>|0x000|+0x010:'486'
	// <0>
	// <0x5ca500>|0x010|+0x013:'488'
	// <0>
	// <1>
	// <0x5ca513>|0x023|+0x00f:'491'
	// <0>
	// <0x5ca522>|0x032|+0x015:'493'
	// <0>
	// <0x5ca537>|0x047|+0x01a:'495'
	// <0x5ca551>|0x061|+0x025:'496'
	// <0x5ca576>|0x086|+0x037:'497'
	// <0x5ca5ad>|0x0bd|+0x034:'498'
	// <0x5ca5e1>|0x0f1|+0x030:'499'
	// <0>
	// <0x5ca611>|0x121|+0x02f:'501'
	// <0x5ca640>|0x150|+0x04d:'502'
	// ******
}

// STATE[STUB]
 options_tab::options_tab( game& g, flash_movie_resource_ptr& movie, options_enum type )
	: m_game( g )       // buildability: ref members must be init'd
	, m_movie( movie )
{
	// STATICS
	// static < NoType > 				 = <0x5cba78>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba264 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN4") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba44b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba80b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN2") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5ba8ac }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1") })
	// ******

	// FUNCTION BODY[0x5cb240]: 66
	// <0x5cb240>|0x000|+0x014:'509'	{
	// <0x5cb254>|0x014|+0x010:'510'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cb264>|0x024|+0x01e:'515'
	// <0>
	// <0x5cb282>|0x042|+0x032:'517'
	// <0x5cb2b4>|0x074|+0x02e:'518'
	// <0x5cb2e2>|0x0a2|+0x031:'519'
	// <0x5cb313>|0x0d3|+0x034:'520'
	// <0x5cb347>|0x107|+0x02e:'521'
	// <0x5cb375>|0x135|+0x031:'522'
	// <0x5cb3a6>|0x166|+0x030:'523'
	// <0x5cb3d6>|0x196|+0x02e:'524'
	// <0x5cb404>|0x1c4|+0x038:'525'
	// <0x5cb43c>|0x1fc|+0x00f:'525'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cb44b>|0x20b|+0x01f:'530'
	// <0>
	// <0x5cb46a>|0x22a|+0x025:'532'
	// <0x5cb48f>|0x24f|+0x028:'533'
	// <0x5cb4b7>|0x277|+0x02e:'534'
	// <0x5cb4e5>|0x2a5|+0x031:'535'
	// <0x5cb516>|0x2d6|+0x033:'536'
	// <0x5cb549>|0x309|+0x02d:'537'
	// <0x5cb576>|0x336|+0x03e:'538'
	// <0x5cb5b4>|0x374|+0x041:'539'
	// <0x5cb5f5>|0x3b5|+0x020:'540'
	// <0x5cb615>|0x3d5|+0x033:'541'
	// <0x5cb648>|0x408|+0x02d:'542'
	// <0x5cb675>|0x435|+0x033:'543'
	// <0x5cb6a8>|0x468|+0x02d:'544'
	// <0x5cb6d5>|0x495|+0x033:'545'
	// <0x5cb708>|0x4c8|+0x02d:'546'
	// <0x5cb735>|0x4f5|+0x033:'547'
	// <0x5cb768>|0x528|+0x02d:'548'
	// <0x5cb795>|0x555|+0x033:'549'
	// <0x5cb7c8>|0x588|+0x034:'550'
	// <0x5cb7fc>|0x5bc|+0x00f:'550'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5cb80b>|0x5cb|+0x01e:'555'
	// <0>
	// <0x5cb829>|0x5e9|+0x032:'557'
	// <0x5cb85b>|0x61b|+0x042:'558'
	// <0x5cb89d>|0x65d|+0x00f:'558'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5cb8ac>|0x66c|+0x01e:'565'
	// <0>
	// <0x5cb8ca>|0x68a|+0x03f:'567'
	// <0x5cb909>|0x6c9|+0x03b:'568'
	// <0x5cb944>|0x704|+0x03e:'569'
	// <0x5cb982>|0x742|+0x041:'570'
	// <0x5cb9c3>|0x783|+0x02e:'571'
	// <0x5cb9f1>|0x7b1|+0x03e:'572'
	// <0x5cba2f>|0x7ef|+0x038:'573'
	// <0x5cba67>|0x827|-0x633:'573'
	// <0>
	// <1>
	// <0x5cb434>|0x1f4|+0x00f:'576'
	// <0x5cb443>|0x203|+0x3b1:'576'
	// <0x5cb7f4>|0x5b4|+0x00f:'576'
	// <0x5cb803>|0x5c3|+0x092:'576'
	// <0x5cb895>|0x655|+0x00f:'576'
	// <0x5cb8a4>|0x664|+0x1bb:'576'
	// <0x5cba5f>|0x81f|+0x00f:'576'
	// <0x5cba6e>|0x82e|      :'576'	}
	// ******
}

 options_tab::~options_tab( )
{
	for ( u8 i = 0; i < m_options_count; ++i )
		DELETE( m_options[ i ] );

	DELETE( m_options );
}

// claude@NOTE: STATE[STUB]. Body: for-each option apply(); if(m_type==
// video_options_type) m_game.renderer().scene().end_render_options_changing(...);
// console_commands::find("cfg_save_user")->execute(""); initialize_data(movie).
// Blocked by render::scene_renderer::end_render_options_changing (cross-module
// render cap, same as the gamma chain - not declared/defined in our render module)
// and depends on initialize_data (still STUB).
void options_tab::apply( flash_movie_resource_ptr& movie )
{
}

void options_tab::revert( flash_movie_resource_ptr& movie )
{
	VOSTOK_UNREFERENCED_PARAMETER( movie );

	for ( u8 i = 0; i < m_options_count; ++i )
		m_options[ i ]->revert( );
}

// STATE[STUB]
void options_tab::initialize_data( flash_movie_resource_ptr& movie )
{
	// LOCALS
	// flash_value[3] 					options_args
	// u8 								i
	// flash_value 						option_item_member
	// flash_value 						option_item
	// ******

	// CALL SITE INFO
	// <0x5c9efd> -> void < unknown >()
	// <0x5c9f6b> -> void < unknown >( flash_value& )
	// ******

	// FUNCTION BODY[0x5c9e30]: 25
	// <0x5c9e43>|0x013|+0x01e:'616'
	// <0x5c9e61>|0x031|+0x027:'617'
	// <0x5c9e88>|0x058|+0x033:'618'
	// <0>
	// <1>
	// <0x5c9ebb>|0x08b|+0x026:'621'
	// <0>
	// <0x5c9ee1>|0x0b1|+0x00d:'623'
	// <0>
	// <0x5c9eee>|0x0be|+0x011:'625'
	// <0>
	// <1>
	// <0x5c9eff>|0x0cf|+0x021:'628'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5c9f20>|0x0f0|+0x03c:'633'
	// <0>
	// <0x5c9f5c>|0x12c|+0x011:'635'
	// <0x5c9f6d>|0x13d|+0x02e:'636'
	// <0>
	// <0x5c9f9b>|0x16b|+0x016:'638'
	// <0x5c9fb1>|0x181|+0x05b:'639'
	// <0x5ca00c>|0x1dc|+0x01e:'640'
	// ******
}

} // namespace survarium
