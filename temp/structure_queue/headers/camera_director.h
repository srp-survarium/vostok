////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_DIRECTOR_H_INCLUDED
#define CAMERA_DIRECTOR_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;
class survarium::game_camera;

/* FORWARD REFS */
class vostok::configs::binary_config_value;

namespace survarium {

class camera_director : public boost::noncopyable {
public:
			explicit				camera_director			( base_game_scene& w );

	virtual	void					load					( configs::binary_config_value const& __formal ) { /* no source */ }

			void					switch_to_camera		( game_camera* c, pcstr camera_name );
	inline	game_camera const*		get_active_camera		( ) const { /* no source */ }

			void					tick					( );

			void					apply					( );

	inline	float4x4 const&			get_inverted_view_matrix( ) const { /* no source */ }
	inline	float4x4 const&			get_projection_matrix	( ) const { /* no source */ }

			void					set_position_direction	( float3 const& p, float3 const& d );

	virtual	void					on_focus				( bool b_focus_enter );

	inline							~camera_director		( ) { /* no source */ }

private:
	/* 0x0004 */	/* boost::noncopyable */
	/* 0x0004 */	float4x4			m_inverted_view;
	/* 0x0044 */	float4x4			m_projection;
	/* 0x0084 */	game_camera*		m_active_camera;
	/* 0x0088 */	base_game_scene&	m_game_scene;
}; // class camera_director

STATIC_SIZE_ASSERT(camera_director, 0x8C);

} // namespace survarium

#endif // #ifndef CAMERA_DIRECTOR_H_INCLUDED
