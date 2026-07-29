////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CLOUDS_H_INCLUDED
#define RENDER_CLOUDS_H_INCLUDED

/* INCLUDES */
class vostok::tasks::task;
class vostok::tasks::task_type;
class vostok::uninitialized_reference<vostok::render::cloud_simulation>;
struct vostok::render::cloud_key_parameters;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::render::cloud_parameters;

namespace vostok {
namespace render {

struct clouds {
									clouds				( );

			void					initialize			( cloud_parameters const& parameters );

			void					set_time			( float time );
			void					set_key				( const u32 index, cloud_key_parameters const& in_cloud_key_parameters );
	inline	cloud_key_parameters	get_key				( const u32 arg_0 ) const { /* no source */ }
	inline	u32						get_num_keys		( ) const { /* no source */ }
			void					set_num_keys		( const u32 num_keys );

			void					invalidate			( );

	inline	bool					is_updated			( ) const { /* no source */ }

			void					set_sun_direction	( float3 const& sun_direction );

	inline	void					load				( configs::binary_config_value const& arg_0 ) { /* no source */ }

	inline	void					save				( configs::binary_config_value& arg_0 ) { /* no source */ }

			void					generate_cloud_right( u32 key_index );
	inline	void					generate_cloud_both	( u32 arg_0, u32 arg_1 ) { /* no source */ }

			void					set_editor_mode		( bool value );
	inline	bool					is_editor_mode		( ) const { /* no source */ }

private:
	inline	cloud_key_parameters	get_next_key_of		( const u32 arg_0 ) const { /* no source */ }
			u32						get_next_index_of	( const u32 index ) const;
	inline	u32						get_prev_index_of	( const u32 arg_0 ) const { /* no source */ }

public:
	inline							~clouds				( ) { /* no source */ }

	/* 0x0000 */	cloud_key_parameters	m_keys[32];
	/* 0x0880 */	u32						m_num_keys;
	/* 0x0888 */	uninitialized_reference< cloud_simulation >	m_cloud_simulation_0;
	/* 0x08f8 */	uninitialized_reference< cloud_simulation >	m_cloud_simulation_1;
	/* 0x0968 */	uninitialized_reference< cloud_simulation >	m_cloud_simulation_2;
	/* 0x09d8 */	bool					m_is_updated;
	/* 0x09dc */	cloud_key_parameters	m_interp_key;
	/* 0x0a20 */	float					m_interp_alpha;
private:
	/* 0x0a24 */	tasks::task_type*		m_tasks_type;
	/* 0x0a28 */	tasks::task				m_parent_task;
	/* 0x0a88 */	float					m_key_time_step;
	/* 0x0a8c */	float					m_time;
	/* 0x0a90 */	u32						m_current_key_0;
	/* 0x0a94 */	u32						m_current_key_1;
	/* 0x0a98 */	bool					m_invalid;
	/* 0x0a9c */	float3					m_sun_direction;
	/* 0x0aa8 */	bool					m_is_editor_mode;
}; // struct clouds

STATIC_SIZE_ASSERT(clouds, 0xAB0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CLOUDS_H_INCLUDED
