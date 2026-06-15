////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "stats_graph.h"

namespace survarium {

// STATE[STUB]
 stats_graph::stats_graph(
	const float		time_interval,
	const float		invalid_value,
	const float		important_value0,
	const float		important_value1,
	const u32		color
) :
	// legacy m_current_value -> canonical m_newest_value; m_cumulative_value /
	// m_weighted_average_value are canonical-only (no legacy source) -> buildability 0
	m_newest_value			( 0 ),
	m_values_pool			( 0 ),
	m_time_interval			( time_interval ),
	m_invalid_value			( invalid_value ),
	m_important_value0		( important_value0 ),
	m_important_value1		( important_value1 ),
	m_cumulative_value		( 0 ),			// buildability: matcher supplies real value
	m_weighted_average_value( 0 ),			// buildability: matcher supplies real value
	m_count					( 0 ),
	m_color					( color )
{
	// FUNCTION BODY[0x5d9d80]: 3
	// <0x5d9d80>|0x000|+0x03f:'45'	{
	// <0>
	// <1>
	// <2>
	// <0x5d9dbf>|0x03f|      :'49'	}
	// ******
}

// STATE[STUB]
 stats_graph::~stats_graph( )
{
	// legacy m_current_value -> canonical m_newest_value
	for ( u32 i=0; i < m_count; ++i ) {
		stats_value* value			= m_newest_value;
		m_newest_value				= m_newest_value->next;
		DELETE						( value );
	}

	while ( m_values_pool ) {
		stats_value* value			= m_values_pool;
		m_values_pool				= m_values_pool->next;
		DELETE						( value );
	}

	// FUNCTION BODY[0x5d9e30]: 13
	// <0x5d9e32>|0x002|+0x00e:'53'
	// <0>
	// <0x5d9e40>|0x010|+0x002:'55'
	// <0x5d9e42>|0x012|+0x004:'56'
	// <0x5d9e46>|0x016|+0x01b:'57'
	// <0>
	// <1>
	// <0x5d9e61>|0x031|+0x022:'60'
	// <0x5d9e83>|0x053|-0x01d:'60'
	// <0>
	// <0x5d9e66>|0x036|+0x003:'62'
	// <0x5d9e69>|0x039|+0x005:'63'
	// <0x5d9e6e>|0x03e|+0x01d:'64'
	// <0>
	// ******
}

// STATE[STUB]
void stats_graph::adjust_time_interval( )
{
	// legacy m_current_value -> canonical m_newest_value
	ASSERT							( m_newest_value->time >= m_newest_value->next->time );
	if ( m_newest_value->time - m_newest_value->next->time <= m_time_interval )
		return;

	while ( m_newest_value->time - m_newest_value->next->next->time >= m_time_interval ) {
		stats_value* old_value		= m_newest_value->next;
		m_newest_value->next		= old_value->next;
		old_value->next->previous	= m_newest_value;
		--m_count;

		ASSERT						( m_count >= 2 );
		ASSERT						( m_newest_value->time - m_newest_value->next->time >= m_time_interval );

		if ( !m_values_pool ) {
			m_values_pool			= old_value;
			m_values_pool->next		= 0;
		}
		else {
			old_value->next			= m_values_pool;
			m_values_pool			= old_value;
		}
	}

	// FUNCTION BODY[0x5d9d00]: 28
	// <0>
	// <0x5d9d00>|0x000|+0x01b:'71'
	// <0>
	// <1>
	// <0x5d9d1b>|0x01b|+0x015:'74'
	// <0>
	// <0x5d9d30>|0x030|+0x004:'76'
	// <0>
	// <0x5d9d34>|0x034|+0x00f:'78'
	// <0>
	// <0x5d9d43>|0x043|+0x004:'80'
	// <0x5d9d47>|0x047|+0x007:'81'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5d9d4e>|0x04e|+0x00a:'87'
	// <0>
	// <0x5d9d58>|0x058|+0x003:'89'
	// <0x5d9d5b>|0x05b|+0x002:'90'
	// <0>
	// <0x5d9d5d>|0x05d|+0x002:'92'
	// <0>
	// <0x5d9d5f>|0x05f|+0x002:'94'
	// <0x5d9d61>|0x061|+0x01a:'95'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void stats_graph::add_value( const float time, const float value )
{
	// legacy m_current_value -> canonical m_newest_value
	if ( (m_count > 1) && (time - m_newest_value->next->next->time >= m_time_interval) ) {
		m_newest_value				= m_newest_value->next;
		m_newest_value->time		= time;
		m_newest_value->value		= value;
		adjust_time_interval		( );
		return;
	}

	stats_value*					new_value;
	if ( !m_values_pool )
		new_value					= NEW( stats_value );
	else {
		new_value					= m_values_pool;
		m_values_pool				= m_values_pool->next;
	}

	new_value->time					= time;
	new_value->value				= value;
	++m_count;

	if ( !m_newest_value ) {
		new_value->next				= new_value;
		new_value->previous			= new_value;
		m_newest_value				= new_value;
		return;
	}

	new_value->next					= m_newest_value->next;
	new_value->previous				= m_newest_value;
	m_newest_value->next			= new_value;
	new_value->next->previous		= new_value;
	m_newest_value					= new_value;

	// FUNCTION BODY[0x5d9e90]: 53
	// <0x5d9e90>|0x000|+0x001:'101'	{
	// <0>
	// <1>
	// <2>
	// <0x5d9e91>|0x001|+0x020:'105'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5d9eb1>|0x021|+0x005:'112'
	// <0x5d9eb6>|0x026|+0x00d:'113'
	// <0>
	// <0x5d9ec3>|0x033|+0x00f:'115'
	// <0>
	// <1>
	// <0x5d9ed2>|0x042|+0x005:'118'
	// <0x5d9ed7>|0x047|+0x002:'119'
	// <0>
	// <0x5d9ed9>|0x049|+0x010:'121'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d9ee9>|0x059|+0x007:'126'
	// <0x5d9ef0>|0x060|+0x00d:'127'
	// <0x5d9efd>|0x06d|+0x008:'128'
	// <0>
	// <1>
	// <0x5d9f05>|0x075|+0x005:'131'
	// <0>
	// <1>
	// <0x5d9f0a>|0x07a|+0x005:'134'
	// <0x5d9f0f>|0x07f|+0x00b:'135'
	// <0>
	// <1>
	// <0x5d9f1a>|0x08a|+0x005:'138'
	// <0>
	// <1>
	// <2>
	// <0x5d9f1f>|0x08f|+0x012:'142'
	// <0>
	// <0x5d9f31>|0x0a1|+0x002:'144'
	// <0x5d9f33>|0x0a3|+0x009:'145'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5d9f3c>|0x0ac|+0x004:'150'
	// <0x5d9f40>|0x0b0|+0x005:'151'
	// <0x5d9f45>|0x0b5|+0x004:'152'
	// <0x5d9f49>|0x0b9|-0x013:'153'
	// <0x5d9f36>|0x0a6|+0x018:'154'
	// <0x5d9f4e>|0x0be|-0x068:'154'
	// <0x5d9ee6>|0x056|+0x053:'155'
	// <0x5d9f39>|0x0a9|+0x018:'155'
	// <0x5d9f51>|0x0c1|      :'155'	}
	// ******
}

// STATE[STUB]
void stats_graph::set_time_interval( float new_time_interval )
{
	m_time_interval					= new_time_interval;
	adjust_time_interval			( );

	// FUNCTION BODY[0x5d9dd0]: 1
	// <0x5d9dd0>|0x000|+0x005:'159'
	// ******
}

// STATE[STUB]
float stats_graph::cumulative_time( ) const
{
	return 0.0f;

	// FUNCTION BODY[0x5d9cf0]: 4
	// <0>
	// <1>
	// <2>
	// <0x5d9cf0>|0x000|+0x00e:'168'
	// ******
}

// STATE[STUB]
float stats_graph::average_value( ) const
{
	// legacy stats_time() -> canonical cumulative_time()
	float const time				= cumulative_time();
	if ( math::is_zero(time) )
		return						( 0.f );

	return							m_count / time;

	// FUNCTION BODY[0x5d9de0]: 5
	// <0x5d9de0>|0x000|+0x001:'172'	{
	// <0x5d9de1>|0x001|+0x00e:'173'
	// <0x5d9def>|0x00f|+0x01e:'174'
	// <0x5d9e0d>|0x02d|+0x005:'175'
	// <0>
	// <0x5d9e12>|0x032|-0x002:'177'
	// <0x5d9e10>|0x030|+0x010:'178'
	// <0x5d9e20>|0x040|      :'178'	}
	// ******
}

// STATE[STUB]
void stats_graph::render(
	render::ui::renderer&					renderer,
	render::scene_view_ptr const&			scene_view,
	const u32								top_margin,
	const u32								height,
	u32										arg_4,
	u32										arg_5,
	u32										arg_6
)
{
	// LOCALS
	// const float 						value_height
	// buffer_vector< render::ui::vertex > vertices
	// const float 						value_range
	// stats_graph::stats_value* 		i
	// u32 								count
	// const render::ui::vertex[2] 		vertices
	// ******

	// STATICS
	// static < NoType > 				 = <0x5da2b4>;
	// ******

	// TYPEDEFS
	// typedef
	// 	buffer_vector< render::ui::vertex >
	// 	vertices_type;

	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c8fd7 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN9") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c8fe3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN8") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c8fef }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5c8ff9 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: false, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN6") })
	// ******

	// FUNCTION BODY[0x5d9f60]: 130
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
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <0x5d9f63>|0x003|+0x009:'213'
	// <0x5d9f6c>|0x00c|+0x003:'214'
	// <0>
	// <0x5d9f6f>|0x00f|+0x009:'216'
	// <0x5d9f78>|0x018|+0x008:'217'
	// <0>
	// <1>
	// <0x5d9f80>|0x020|+0x00d:'220'
	// <0x5d9f8d>|0x02d|+0x009:'221'
	// <0>
	// <0x5d9f96>|0x036|+0x008:'223'
	// <0x5d9f9e>|0x03e|+0x001:'224'
	// <0>
	// <1>
	// <0x5d9f9f>|0x03f|+0x002:'227'
	// <0x5d9fa1>|0x041|+0x008:'228'
	// <0>
	// <0x5d9fa9>|0x049|+0x009:'230'
	// <0>
	// <1>
	// <2>
	// <0x5d9fb2>|0x052|+0x01c:'234'
	// <0>
	// <0x5d9fce>|0x06e|+0x002:'236'
	// <0>
	// <1>
	// <2>
	// <0x5d9fd0>|0x070|+0x007:'240'
	// <0>
	// <1>
	// <2>
	// <0x5d9fd7>|0x077|+0x005:'244'
	// <0x5d9fdc>|0x07c|+0x005:'245'
	// <0x5d9fe1>|0x081|+0x002:'246'
	// <0>
	// <1>
	// <2>
	// <0x5d9fe3>|0x083|+0x005:'250'
	// <0x5d9fe8>|0x088|+0x005:'251'
	// <0x5d9fed>|0x08d|+0x002:'252'
	// <0>
	// <1>
	// <2>
	// <0x5d9fef>|0x08f|+0x003:'256'
	// <0x5d9ff2>|0x092|+0x005:'257'
	// <0x5d9ff7>|0x097|+0x002:'258'
	// <0>
	// <1>
	// <2>
	// <0x5d9ff9>|0x099|+0x007:'262'
	// <0x5da000>|0x0a0|+0x005:'263'
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
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <0x5da005>|0x0a5|+0x037:'280'
	// <0>
	// <1>
	// <0x5da03c>|0x0dc|+0x00b:'283'
	// <0>
	// <1>
	// <0x5da047>|0x0e7|+0x068:'286'
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
	// <11>
	// <12>
	// <13>
	// <0x5da0af>|0x14f|+0x015:'301'
	// <0x5da0c4>|0x164|+0x023:'302'
	// <0>
	// <0x5da0e7>|0x187|+0x02c:'304'
	// <0>
	// <1>
	// <0x5da113>|0x1b3|+0x013:'307'
	// <0>
	// <0x5da126>|0x1c6|+0x054:'309'
	// <0x5da17a>|0x21a|+0x09a:'310'
	// <0x5da214>|0x2b4|-0x010:'310'
	// <0x5da204>|0x2a4|+0x012:'311'
	// <0x5da216>|0x2b6|+0x06b:'311'
	// <0>
	// <0x5da281>|0x321|+0x005:'313'
	// <0x5da286>|0x326|+0x008:'314'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5da28e>|0x32e|+0x018:'319'
	// <0>
	// ******
}

// STATE[STUB]
void stats_graph::stop_rendering( )
{
	// FUNCTION BODY[0x5d9ce0]: 10
	// <0x5d9ce0>|0x000|+0x000:'324'	{
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
	// <0x5d9ce0>|0x000|      :'335'	}
	// ******
}

} // namespace survarium
