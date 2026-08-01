#include "pch.h"

#include "clouds.h"

namespace vostok {
namespace render {

clouds::clouds( ) :
	m_num_keys			( 0 ),
	m_is_updated		( true ),
	m_interp_alpha		( 0.0f ),
	m_tasks_type		( tasks::create_new_task_type( "clouds_task", 0 ) ),
	m_time				( 0.0f ),
	m_current_key_0	( u32(-1) ),
	m_current_key_1	( u32(-1) ),
	m_invalid			( false ),
	m_sun_direction	( 0.0f, -1.0f, 0.0f ),
	m_is_editor_mode	( false )
{
	m_is_updated = false;
}

void clouds::generate_cloud_right( u32 key_index )
{
	m_cloud_simulation_2->generate( get_next_key_of( key_index ), -m_sun_direction );
}

void clouds::set_editor_mode( bool value )
{
	m_is_editor_mode = value;
}

void clouds::initialize( cloud_parameters const& parameters )
{
	if ( m_cloud_simulation_0.initialized( ) )
		m_cloud_simulation_0.destroy( );

	VOSTOK_CONSTRUCT_REFERENCE( m_cloud_simulation_0, cloud_simulation )(
		parameters.grid_width,
		parameters.grid_height,
		parameters.grid_width
	);

	if ( m_cloud_simulation_1.initialized( ) )
		m_cloud_simulation_1.destroy( );

	VOSTOK_CONSTRUCT_REFERENCE( m_cloud_simulation_1, cloud_simulation )(
		parameters.grid_width,
		parameters.grid_height,
		parameters.grid_width
	);

	if ( m_cloud_simulation_2.initialized( ) )
		m_cloud_simulation_2.destroy( );

	VOSTOK_CONSTRUCT_REFERENCE( m_cloud_simulation_2, cloud_simulation )(
		parameters.grid_width,
		parameters.grid_height,
		parameters.grid_width
	);

	invalidate( );
}

u32 clouds::get_next_index_of( u32 index ) const
{
	return index + 1 < m_num_keys ? index + 1 : 0;
}

void clouds::invalidate( )
{
	m_current_key_0 = u32(-1);
	m_current_key_1 = u32(-1);
}

void clouds::set_sun_direction( float3 const& sun_direction )
{
	if ( m_sun_direction != sun_direction )
		invalidate( );

	m_sun_direction = sun_direction;
}

void clouds::set_time( float time )
{
	if ( !m_num_keys )
		return;

	time = math::abs( time ) - math::abs( static_cast<int>( time ) );
	bool const async = !m_is_editor_mode;

	if ( m_num_keys == 1 )
	{
		m_is_updated = false;

		if ( m_current_key_0 != 0 )
		{
			m_cloud_simulation_0->generate( m_keys[0], -m_sun_direction );
			m_cloud_simulation_1->copy_from( *m_cloud_simulation_0 );
			m_is_updated = true;
		}

		m_interp_alpha = 0.0f;
		m_current_key_0 = 0;
		m_current_key_1 = 0;
		m_interp_key = m_keys[0];
		return;
	}

	cloud_key_parameters first_key = m_keys[0];
	cloud_key_parameters last_key = m_keys[m_num_keys - 1];
	u32 k0 = m_current_key_0;
	u32 k1 = m_current_key_1;
	m_interp_key = m_keys[k0];

	if ( time >= first_key.linear_time && time <= last_key.linear_time )
	{
		for ( u32 i = 0; i < m_num_keys - 1; ++i )
		{
			if ( time > m_keys[i].linear_time && m_keys[i + 1].linear_time > time )
			{
				m_current_key_1 = i + 1;
				m_current_key_0 = i;

				cloud_key_parameters key_0 = m_keys[m_current_key_0];
				cloud_key_parameters key_1 = m_keys[m_current_key_1];
				m_interp_alpha = (time - key_0.linear_time) / (key_1.linear_time - key_0.linear_time);
			}
		}
	}
	else
	{
		m_current_key_0 = m_num_keys - 1;
		m_current_key_1 = 0;

		cloud_key_parameters key_0 = m_keys[m_current_key_0];
		cloud_key_parameters key_1 = m_keys[m_current_key_1];
		float const interval = 1.0f - key_0.linear_time + key_1.linear_time;

		if ( time > key_0.linear_time )
			m_interp_alpha = (time - key_0.linear_time) / interval;

		if ( key_1.linear_time > time )
			m_interp_alpha = (time + 1.0f - key_0.linear_time) / interval;
	}

	if ( async )
	{
		m_is_updated = false;

		if ( k0 != m_current_key_0 || k1 != m_current_key_1 )
		{
			if ( m_current_key_0 == k1 )
			{
				tasks::wait_for_task_list( &m_parent_task );
				m_cloud_simulation_0->copy_from( *m_cloud_simulation_1 );
				m_cloud_simulation_1->copy_from( *m_cloud_simulation_2 );
				tasks::spawn_task(
					boost::bind( &clouds::generate_cloud_right, this, m_current_key_1 ),
					m_tasks_type,
					&m_parent_task
				);
			}
			else if ( m_current_key_1 != k0 )
			{
				m_cloud_simulation_0->generate( m_keys[m_current_key_0], -m_sun_direction );
				m_cloud_simulation_1->generate( m_keys[m_current_key_1], -m_sun_direction );
				m_cloud_simulation_2->generate( get_next_key_of( m_current_key_1 ), -m_sun_direction );
			}

			m_is_updated = true;
		}
	}
	else
	{
		if ( k0 != m_current_key_0 || k1 != m_current_key_1 )
		{
			m_is_updated = false;

			if ( m_current_key_0 == k1 )
			{
				m_cloud_simulation_0->copy_from( *m_cloud_simulation_1 );
				m_cloud_simulation_1->generate( m_keys[m_current_key_1], -m_sun_direction );
			}
			else if ( m_current_key_1 == k0 )
			{
				m_cloud_simulation_1->copy_from( *m_cloud_simulation_0 );
				m_cloud_simulation_0->generate( m_keys[m_current_key_0], -m_sun_direction );
			}
			else
			{
				m_cloud_simulation_0->generate( m_keys[m_current_key_0], -m_sun_direction );
				m_cloud_simulation_1->generate( m_keys[m_current_key_1], -m_sun_direction );
			}

			m_is_updated = true;
		}
	}

	m_interp_key = cloud_key_parameters::lerp(
		m_keys[m_current_key_0],
		m_keys[m_current_key_1],
		m_interp_alpha
	);
}

void clouds::set_num_keys( u32 num_keys )
{
	m_num_keys = num_keys;
}

void clouds::set_key( u32 index, cloud_key_parameters const& in_cloud_key_parameters )
{
	m_keys[index] = in_cloud_key_parameters;
}

} // namespace render
} // namespace vostok
