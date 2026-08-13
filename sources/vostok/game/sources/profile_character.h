////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PROFILE_CHARACTER_H_INCLUDED
#define PROFILE_CHARACTER_H_INCLUDED

#include <vostok/animation/animation_player.h>
#include <vostok/animation/skeleton.h>
// PDB spells the model members vostok::render::skeleton_model_instance_ptr /
// static_model_instance_ptr / base_scene_ptr - the same resource_ptr types our
// render tree typedefs as skeleton_model_ptr / static_model_ptr / scene_ptr
#include <vostok/render/facade/model.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/resources_managed_resource.h>

namespace vostok {
namespace render {
	class scene_renderer;
} // namespace render
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class items_dictionary;
class player_profile;

void use_game_lobby_scene( );	// /OPT:REF anchor (anchor_game_lobby_scene.cpp), friend below

class profile_character : private boost::noncopyable {
	// codegen-neutral: lets the lobby-scene reachability anchor address-take the
	// private *_ready callbacks until the real query_resources call graph reaches
	// them (their callers - query_character_animations etc. - are still STUBs).
	friend void ::survarium::use_game_lobby_scene( );
public:
	inline			profile_character			(
						items_dictionary&			arg_0,
						render::scene_renderer&		arg_1,
						render::scene_ptr const&	arg_2
					) :
						m_scene_renderer	( arg_1 ),
						m_scene				( arg_2 ),
						m_items_dictionary	( arg_0 )
					{ /* no source */ }
	inline			~profile_character			( ) { /* no source */ }

			void	update						( const u32 current_time_in_ms );

	inline	void	clear_resources				( ) { /* no source */ }

	inline	void	profile_changed				( player_profile const* arg_0 ) { /* no source */ }

private:
	inline	void	query_profile_contents		( player_profile const* arg_0 ) { /* no source */ }
	inline	void	query_character_animations	( ) { /* no source */ }

			void	character_model_ready		( resources::queries_result& data );
			void	character_animation_ready	( resources::queries_result& data );
			void	weapon_resources_ready		( resources::queries_result& data );

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float4x4							m_initial_matrix;
	/* 0x0040 */	animation::animation_player			m_animation_player;
	/* 0x8588 */	render::skeleton_model_ptr			m_character_model;
	/* 0x858c */	resources::managed_resource_ptr		m_character_animation[2];
	/* 0x8594 */	animation::skeleton_ptr				m_skeleton;

public:
	struct preview_weapon {
		inline		preview_weapon	( ) { /* no source */ }
		inline		~preview_weapon	( ) { /* no source */ }

	public:
		/* 0x0000 */	render::skeleton_model_ptr			m_model;
		/* 0x0004 */	render::static_model_ptr			m_addon;
		/* 0x0008 */	resources::managed_resource_ptr		m_animation;
		/* 0x000c */	render::model_locator_item			m_addon_locator;
		/* 0x0070 */	bool								m_visible;
	}; // struct preview_weapon

private:
	/* 0x8598 */	preview_weapon						m_preview_weapon[2];
	/* 0x8680 */	u32									m_weapon_bone_index;
	/* 0x8684 */	render::scene_renderer&				m_scene_renderer;
	/* 0x8688 */	render::scene_ptr const&			m_scene;
	/* 0x868c */	items_dictionary&					m_items_dictionary;
}; // class profile_character

STATIC_SIZE_ASSERT(profile_character, 0x8690);
STATIC_SIZE_ASSERT(profile_character::preview_weapon, 0x74);

} // namespace survarium

#endif // #ifndef PROFILE_CHARACTER_H_INCLUDED
