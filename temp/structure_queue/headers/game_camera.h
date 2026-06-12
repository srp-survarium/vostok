////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_CAMERA_H_INCLUDED
#define GAME_CAMERA_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;

/* FORWARD REFS */
class survarium::camera_director;

namespace survarium {

class game_camera : public boost::noncopyable {
public:
			explicit			game_camera				( base_game_scene& w );

	virtual	float4x4			get_projection_matrix	( float2 const& window_size ) const;
	inline	float4x4 const&		get_inverted_view_matrix( ) const { /* no source */ }

			void				set_position_direction	( float3 const& p, float3 const& d );
	inline	void				set_fov_factor			( const float arg_0 ) { /* no source */ }
	inline	float				get_fov_factor			( ) const { /* no source */ }

	inline	void				set_near_plane			( const float arg_0 ) { /* no source */ }
	inline	float				get_near_plane			( ) const { /* no source */ }
	inline	float				get_far_plane			( ) const { /* no source */ }

	virtual	void				on_activate				( camera_director* cd );
	virtual	void				on_deactivate			( ) { /* no source */ }
	virtual	void				on_focus				( bool __formal ) { /* no source */ }

	virtual	void				tick					( );

	inline	base_game_scene&	get_game_scene			( ) { /* no source */ }
			float				get_vertical_fov		( ) const;

	inline						~game_camera			( ) { /* no source */ }

private:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	float4x4			m_inverted_view_matrix;
	/* 0x0044 */	base_game_scene&	m_game_scene;
	/* 0x0048 */	float				m_near_plane;
	/* 0x004c */	float				m_far_plane;
	/* 0x0050 */	float				m_fov_factor;
}; // class game_camera

STATIC_SIZE_ASSERT(game_camera, 0x54);

} // namespace survarium

#endif // #ifndef GAME_CAMERA_H_INCLUDED
