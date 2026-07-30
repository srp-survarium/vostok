#ifndef VOSTOK_RENDER_ENGINE_CLOUDS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CLOUDS_H_INCLUDED

#include <vostok/math_float3.h>
#include <vostok/tasks_task.h>
#include <vostok/uninitialized_reference.h>

#include "cloud_key_parameters.h"
#include "cloud_parameters.h"
#include "cloud_simulation.h"

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

struct clouds {
	clouds( );
	~clouds( ) { }

	void initialize( cloud_parameters const& parameters );
	void set_time( float time );
	void set_key( u32 index, cloud_key_parameters const& in_cloud_key_parameters );

	cloud_key_parameters get_key( u32 index ) const
	{
		return m_keys[index];
	}

	u32 get_num_keys( ) const
	{
		return m_num_keys;
	}

	void set_num_keys( u32 num_keys );
	void invalidate( );

	bool is_updated( ) const
	{
		return m_is_updated;
	}

	void set_sun_direction( float3 const& sun_direction );

	void load( configs::binary_config_value const& )
	{
		// STATE[STUB]
	}

	void save( configs::binary_config_value& )
	{
		// STATE[STUB]
	}

	void generate_cloud_right( u32 key_index );

	void generate_cloud_both( u32, u32 )
	{
		// STATE[STUB]
	}

	void set_editor_mode( bool value );

	bool is_editor_mode( ) const
	{
		return m_is_editor_mode;
	}

private:
	cloud_key_parameters get_next_key_of( u32 index ) const
	{
		return m_keys[get_next_index_of( index )];
	}

	u32 get_next_index_of( u32 index ) const;

	u32 get_prev_index_of( u32 index ) const
	{
		return index ? index - 1 : m_num_keys - 1;
	}

public:
	cloud_key_parameters					m_keys[32];
	u32										m_num_keys;
	uninitialized_reference<cloud_simulation>	m_cloud_simulation_0;
	uninitialized_reference<cloud_simulation>	m_cloud_simulation_1;
	uninitialized_reference<cloud_simulation>	m_cloud_simulation_2;
	bool									m_is_updated;
	cloud_key_parameters					m_interp_key;
	float									m_interp_alpha;

private:
	tasks::task_type*	m_tasks_type;
	tasks::task			m_parent_task;
	float				m_key_time_step;
	float				m_time;
	u32					m_current_key_0;
	u32					m_current_key_1;
	bool				m_invalid;
	float3				m_sun_direction;
	bool				m_is_editor_mode;
};

STATIC_SIZE_ASSERT( clouds, 0xAB0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CLOUDS_H_INCLUDED
