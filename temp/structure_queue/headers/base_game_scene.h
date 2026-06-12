////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_GAME_SCENE_H_INCLUDED
#define BASE_GAME_SCENE_H_INCLUDED

/* INCLUDES */
class vostok::math::int2;
class survarium::camera_director;
class survarium::game;
class survarium::game_scene;
struct vostok::physics::engine;
struct vostok::physics::world;
struct survarium::engine;
struct survarium::flash_text_manager;
class vostok::render::base_scene;
class vostok::render::base_scene_view;
class survarium::flash_movie_resource;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class vostok::math::uint2;
class vostok::render::game::renderer;
class vostok::render::scene_renderer;
class survarium::scheduler;
class survarium::swf_input_translator;

namespace survarium {

class base_game_scene : public game_scene , public engine , public physics::engine , public boost::noncopyable {
public:
			explicit								base_game_scene		( game& g );
	inline											base_game_scene		(
														game&									arg_0,
														render::base_scene_ptr const&			arg_1,
														render::base_scene_view_ptr const&		arg_2
													) { /* no source */ }
	virtual											~base_game_scene	( );

	virtual	void									on_activate			( ) override;
	virtual	void									on_deactivate		( ) override;

	virtual	void									show_ui				( bool __formal ) { /* no source */ }
	virtual	bool									is_mouse_over_ui	( ) { /* no source */ }

	virtual	void									tick				( const u32 __formal, const u32 current_time_in_ms, const bool is_game_paused ) override;

	virtual	void									on_after_tick		( ) override;

	virtual	void									clear_resources		( ) = 0;

			void									init_physics		( );
			void									destroy_physics		( );

	inline	bool									is_active			( ) const { /* no source */ }

			void									apply_camera		( camera_director& cd );

	inline	game&									get_game			( ) const { /* no source */ }

			render::game::renderer&					renderer			( ) const;

			render::scene_renderer&					scene_renderer		( ) const;

	inline	physics::world*							get_physics_world	( ) { /* no source */ }
	inline	physics::world*							get_physics_world	( ) const { /* no source */ }

			swf_input_translator&					input_translator	( );

			bool									point_to_screen		( float3 const& p, float2& result );

			math::uint2 const&						output_window_size	( ) const;

	inline	resources::unmanaged_resource_ptr&		get_sound_scene		( ) { /* no source */ }
	inline	camera_director&						get_camera_director	( ) { /* no source */ }

			scheduler&								scheduler			( );

			void									show_movie			( flash_movie_resource_ptr& movie );
			void									hide_movie			( flash_movie_resource_ptr& movie );

	inline	flash_text_manager*						get_text_manager	( ) const { /* no source */ }
	inline	flash_text_manager&						text_manager		( ) { /* no source */ }
			void									show_text_manager	( flash_text_manager* tm );
			void									hide_text_manager	( flash_text_manager* tm );
			void									create_text_manager	( );

private:
	/* 0x0000 */	/* game_scene */
	/* 0x000c */	/* engine */
	/* 0x0010 */	/* physics::engine */
	/* 0x0011 */	/* boost::noncopyable */
	/* 0x0014 */	float4x4								m_inverted_view_matrix;
	/* 0x0054 */	float4x4								m_projection_matrix;
	/* 0x0094 */	math::int2								m_mouse_pos;
	/* 0x009c */	resources::unmanaged_resource_ptr		m_sound_scene;
	/* 0x00a0 */	camera_director*						m_camera_director;
	/* 0x00a4 */	flash_text_manager*						m_text_manager;
	/* 0x00a8 */	game&									m_game;
	/* 0x00ac */	bool									m_is_ui_shown;
	/* 0x00b0 */	physics::world*							m_physics_world;
	/* 0x00b4 */	bool									m_is_active;
	/* 0x00b8 */	u32										m_block_btn_time;
}; // class base_game_scene

STATIC_SIZE_ASSERT(base_game_scene, 0xBC);

} // namespace survarium

#endif // #ifndef BASE_GAME_SCENE_H_INCLUDED
