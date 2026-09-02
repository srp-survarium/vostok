// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_CAMERA_H_INCLUDED
#define GAME_CAMERA_H_INCLUDED

namespace survarium {

class base_game_scene;
class camera_director;

extern float s_camera_far_plane;
extern float default_vertical_fov;

// pulled ahead by batch 6 (a base of its player_input_handler); out-of-line
// bodies in the game_camera.cpp compiland (batch 9)
class game_camera : private boost::noncopyable {
public:
			explicit			game_camera				( base_game_scene& w );

	virtual	float4x4			get_projection_matrix	( float2 const& window_size ) const;
	inline	float4x4 const&		get_inverted_view_matrix( ) const { /* no source */ return m_inverted_view_matrix; }

			void				set_position_direction	( float3 const& p, float3 const& d );
	inline	void				set_fov_factor			( const float arg_0 ) { m_fov_factor = arg_0; }
	inline	float				get_fov_factor			( ) const { /* no source */ return m_fov_factor; }

	inline	void				set_near_plane			( const float arg_0 ) { m_near_plane = arg_0; }
	inline	float				get_near_plane			( ) const { /* no source */ return m_near_plane; }
	inline	float				get_far_plane			( ) const { /* no source */ return m_far_plane; }

	virtual	void				on_activate				( camera_director* cd );

	virtual	void				on_deactivate			( ) { /* no source */ }
	virtual	void				on_focus				( bool __formal ) { /* no source */ }

	virtual	void				tick					( );

	inline	base_game_scene&	get_game_scene			( ) { /* no source */ return m_game_scene; }
			float				get_vertical_fov		( ) const;

protected:
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
