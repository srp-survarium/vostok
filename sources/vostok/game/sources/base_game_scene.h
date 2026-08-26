////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_GAME_SCENE_H_INCLUDED
#define BASE_GAME_SCENE_H_INCLUDED

#include <vostok/game_core/game_scene.h>
#include <vostok/game_core/engine.h>
#include <vostok/physics/engine.h>
#include <vostok/resources_unmanaged_resource.h>

#include <vostok/scaleform/sources/flash_movie_resource.h>

namespace vostok {
namespace physics {
	struct world;
} // namespace physics
namespace render {
	class scene_renderer;
	namespace game { class renderer; }
} // namespace render
} // namespace vostok

namespace survarium {

class camera_director;
class game;
class scheduler;
class swf_input_translator;
struct flash_text_manager;

// pulled ahead by batch 5 (a base of its menu scenes); out-of-line bodies in
// the base_game_scene.cpp compiland (batch 9)
class base_game_scene : public game_scene , public engine , private physics::engine , private boost::noncopyable {
public:
			explicit								base_game_scene		( game& g );
	// PDB spells the params vostok::render::base_scene_ptr / base_scene_view_ptr -
	// the same resource_ptr type our render tree typedefs as scene_ptr / scene_view_ptr
	inline											base_game_scene		(
														game&								arg_0,
														render::scene_ptr const&			arg_1,
														render::scene_view_ptr const&		arg_2
													) : m_game( arg_0 ) { /* no source */ }
	virtual											~base_game_scene	( );

	virtual	void									on_activate			( ) override;
	virtual	void									on_deactivate		( ) override;

	virtual	void									show_ui				( bool __formal )		{ }
	virtual	bool									is_mouse_over_ui	( )						{ return false; }

	virtual	void									tick				( const u32 __formal, const u32 current_time_in_ms, const bool is_game_paused ) override;

	virtual	void									on_after_tick		( ) override;

	virtual	void									clear_resources		( ) = 0;

			void									init_physics		( );
			void									destroy_physics		( );

	inline	bool									is_active			( ) const { /* no source */ return m_is_active; }

			void									apply_camera		( camera_director& cd );

	inline	game&									get_game			( ) const { /* no source */ return m_game; }

			render::game::renderer&					renderer			( ) const;

			render::scene_renderer&					scene_renderer		( ) const;

	inline	physics::world*							get_physics_world	( ) { /* no source */ return m_physics_world; }
	inline	physics::world*							get_physics_world	( ) const { /* no source */ return m_physics_world; }

			swf_input_translator&					input_translator	( );

			bool									point_to_screen		( float3 const& p, float2& result );

			math::uint2 const&						output_window_size	( ) const;

	inline	resources::unmanaged_resource_ptr&		get_sound_scene		( ) { /* no source */ return m_sound_scene; }
	inline	camera_director&						get_camera_director	( ) { /* no source */ return *m_camera_director; }

			scheduler&								scheduler			( );

			void									show_movie			( flash_movie_resource_ptr& movie );
			void									hide_movie			( flash_movie_resource_ptr& movie );

	inline	flash_text_manager*						get_text_manager	( ) const { /* no source */ return m_text_manager; }
	inline	flash_text_manager&						text_manager		( ) { /* no source */ return *m_text_manager; }
			void									show_text_manager	( flash_text_manager* tm );
			void									hide_text_manager	( flash_text_manager* tm );

protected:
			// PDB mangles this IAE (protected), unlike the public methods above
			void									create_text_manager	( );

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
private:
	/* 0x00b0 */	physics::world*							m_physics_world;
	/* 0x00b4 */	bool									m_is_active;
	/* 0x00b8 */	u32										m_block_btn_time;
}; // class base_game_scene

STATIC_SIZE_ASSERT(base_game_scene, 0xBC);

} // namespace survarium

#endif // #ifndef BASE_GAME_SCENE_H_INCLUDED
