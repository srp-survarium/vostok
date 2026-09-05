// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROFILE_CHARACTER_H_INCLUDED
#define PROFILE_CHARACTER_H_INCLUDED

#include <vostok/animation/animation_player.h>
#include <vostok/animation/skeleton.h>
// PDB spells the model members vostok::render::skeleton_model_instance_ptr /
// static_model_instance_ptr / base_scene_ptr - the same resource_ptr types our
// render tree typedefs as skeleton_model_ptr / static_model_ptr / scene_ptr
#include <vostok/render/facade/model.h>
#include <vostok/render/facade/scene_renderer.h>
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


class profile_character : private boost::noncopyable {
public:
	// sushi@TODO: constructor model; verify transform/visibility defaults and animation-query timing.
	inline			profile_character			(
						items_dictionary&			items,
						render::scene_renderer&		scene_renderer,
						render::scene_ptr const&	scene
					) :
						m_scene_renderer	( scene_renderer ),
						m_scene				( scene ),
						m_items_dictionary	( items )
					{
						m_initial_matrix.identity( );
						for ( u32 i = 0; i < 2; ++i )
							m_preview_weapon[ i ].m_visible = false;
					}
	// sushi@TODO: recover explicit destructor body; member cleanup does not prove scene removal.
	inline			~profile_character			( ) { /* no source */ }

			void	update						( const u32 current_time_in_ms );

	// sushi@TODO: callback-derived cleanup model; verify reset policy, ordering and async lifetime.
	inline	void	clear_resources				( )
	{
		m_animation_player.reset( true );
		if ( m_character_model )
			m_scene_renderer.remove_model( m_scene, m_character_model->m_render_model );
		m_character_model = NULL;

		for ( u32 i = 0; i < 2; ++i )
		{
			if ( m_preview_weapon[ i ].m_visible )
			{
				if ( m_preview_weapon[ i ].m_model )
					m_scene_renderer.remove_model( m_scene, m_preview_weapon[ i ].m_model->m_render_model );
				if ( m_preview_weapon[ i ].m_addon )
					m_scene_renderer.remove_model( m_scene, m_preview_weapon[ i ].m_addon->m_render_model );
			}
			m_preview_weapon[ i ].m_model = NULL;
			m_preview_weapon[ i ].m_addon = NULL;
			m_preview_weapon[ i ].m_animation = NULL;
			m_preview_weapon[ i ].m_visible = false;
		}
		m_character_animation[ 0 ] = NULL;
		m_character_animation[ 1 ] = NULL;
		m_skeleton = NULL;
	}

	// sushi@TODO: sibling forwarding model; recover original consumer, null and reset policy.
	inline	void	profile_changed				( player_profile const* profile ) { query_profile_contents( profile ); }

private:
	// sushi@TODO: recover skin/weapon request paths, optional-slot handling and profile lifetime.
	inline	void	query_profile_contents		( player_profile const* profile ) { /* no source */ }
	// sushi@TODO: recover the two clip requests and the original scheduling consumer.
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
