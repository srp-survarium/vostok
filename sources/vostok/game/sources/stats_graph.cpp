////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "stats_graph.h"
#include <vostok/render/facade/ui_renderer.h>

namespace survarium {

stats_graph::stats_graph(
	const float		time_interval,
	const float		invalid_value,
	const float		important_value0,
	const float		important_value1,
	const u32		color
) :
	m_newest_value			( 0 ),
	m_values_pool			( 0 ),
	m_time_interval			( time_interval ),
	m_invalid_value			( invalid_value ),
	m_important_value0		( important_value0 ),
	m_important_value1		( important_value1 ),
	m_cumulative_value		( 0 ),
	m_count					( 0 ),
	m_color					( color )
{
}

stats_graph::~stats_graph( )
{
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
}

void stats_graph::adjust_time_interval( )
{
	ASSERT							( UNKNOWN_EXPRESSION_T( m_newest_value->time >= m_newest_value->next->time ) );
	if ( m_newest_value->time - m_newest_value->next->time <= m_time_interval )
		return;

	while ( m_newest_value->time - m_newest_value->next->next->time >= m_time_interval ) {
		stats_value* const old_value	= m_newest_value->next;

		m_cumulative_value			-= old_value->value;
		m_newest_value->next		= old_value->next;
		old_value->next->previous	= m_newest_value;
		--m_count;

		ASSERT						( UNKNOWN_EXPRESSION_T( m_count >= 2 ) );
		ASSERT						( UNKNOWN_EXPRESSION_T( m_newest_value->time - m_newest_value->next->time >= m_time_interval ) );

		if ( !m_values_pool ) {
			m_values_pool			= old_value;
			m_values_pool->next		= 0;
		}
		else {
			old_value->next			= m_values_pool;
			m_values_pool			= old_value;
		}
	}
}

void stats_graph::add_value( const float time, const float value )
{
	if ( (m_count > 1) && (time - m_newest_value->next->next->time >= m_time_interval) ) {
		m_cumulative_value			-= m_newest_value->next->value;
		m_cumulative_value			+= value;
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
	m_cumulative_value				+= value;
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
}

void stats_graph::set_time_interval( float new_time_interval )
{
	m_time_interval					= new_time_interval;
	adjust_time_interval			( );
}

float stats_graph::cumulative_time( ) const
{
	ASSERT							( UNKNOWN_EXPRESSION_T( m_count ) );
	ASSERT							( UNKNOWN_EXPRESSION_T( m_newest_value ) );
	ASSERT							( UNKNOWN_EXPRESSION_T( m_newest_value->next ) );
	return							m_newest_value->time - m_newest_value->next->time;
}

float stats_graph::average_value( ) const
{
	const float current_cumulative_time	= cumulative_time( );
	if ( math::is_zero( current_cumulative_time ) )
		return						( 0.f );

	return							( m_cumulative_value - m_newest_value->next->value ) / current_cumulative_time;
}

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
	VOSTOK_UNREFERENCED_PARAMETER	( arg_4 );
	VOSTOK_UNREFERENCED_PARAMETER	( arg_5 );
	VOSTOK_UNREFERENCED_PARAMETER	( arg_6 );

	if ( !m_newest_value )
		return;

	float min_value					= 0.f;
	float max_value					= m_important_value1;

	stats_value *i					= m_newest_value->next;
	u32 count						= 0;
	do {
		min_value					= math::min( i->value, min_value );
		if ( i->value != m_invalid_value ) {
			max_value				= math::max( i->value, max_value );
			++count;
		}

		i							= i->next;
	} while ( i->previous != m_newest_value );

	if ( count < 2 )
		return;

	const float value_range			= max_value - min_value;
	const float value_height		= float(height);

	for ( u32 i=0; i<4; ++i ) {
		float						value;
		u32							color;
		switch ( i ) {
			case 0 : {
				value				= m_important_value0;
				color				= math::color_xrgb( 0, 0, 255 );
				break;
			}
			case 1 : {
				value				= m_important_value1;
				color				= math::color_xrgb( 0, 255, 255 );
				break;
			}
			case 2 : {
				value				= 0.f;
				color				= math::color_xrgb( 0, 0, 0 );
				break;
			}
			case 3 : {
				value				= average_value();
				color				= math::color_xrgb( 255, 255, 0 );
				break;
			}
			default :				{
				NODEFAULT();
			}
		}

		float y						= float(top_margin + math::floor( (1.f - value/value_range)*value_height ));
		render::ui::vertex const vertices[] = {
			render::ui::vertex( float(10),				y, 0.f, color, 0, 0 ),
			render::ui::vertex( float(10 + 1260 - 1),	y, 0.f, color, 0, 0 ),
		};
		renderer.draw_vertices		( scene_view, array_begin(vertices), array_end(vertices), 1, 0 );
	}

	typedef buffer_vector< render::ui::vertex >	vertices_type;
	vertices_type vertices( ALLOCA(count*sizeof(render::ui::vertex)), count );
	float total_time				= math::max	( cumulative_time(), m_time_interval );
	i								= m_newest_value->next;
	float const start_time			= i->time;
	do {
		if ( i->value != m_invalid_value ) {
			u32 const x				= 10 + math::floor( (i->time - start_time)/total_time*float(1260 - 1) );
			u32 const y				= top_margin + math::floor( (1.f - i->value/value_range)*value_height );
			vertices.push_back		( render::ui::vertex( float(x), float(y), 0.f, m_color, 0, 0 ) );
		}
		i							= i->next;
	} while ( i != m_newest_value );

	renderer.draw_vertices			( scene_view, &*vertices.begin(), &*vertices.end(), 1, 0 );
}

void stats_graph::stop_rendering( )
{
}

} // namespace survarium
