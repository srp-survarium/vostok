////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PROFILE_CHARACTER_H_INCLUDED
#define PROFILE_CHARACTER_H_INCLUDED

/* INCLUDES */
class vostok::animation::animation_player;
class vostok::render::scene_renderer;
class survarium::items_dictionary;
struct survarium::profile_character::preview_weapon;
typedef vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base>
	vostok::animation::skeleton_ptr;
typedef vostok::resources::resource_ptr<vostok::render::base_scene,vostok::resources::unmanaged_intrusive_base>
	vostok::render::base_scene_ptr;
typedef vostok::resources::resource_ptr<vostok::render::skeleton_model_instance,vostok::resources::unmanaged_intrusive_base>
	vostok::render::skeleton_model_instance_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::animation::skeleton;
class vostok::render::base_scene;
class vostok::render::skeleton_model_instance;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class vostok::resources::queries_result;
class survarium::player_profile;

namespace survarium {

class profile_character : public boost::noncopyable {
public:
	inline			profile_character			(
						items_dictionary&					arg_0,
						render::scene_renderer&				arg_1,
						render::base_scene_ptr const&		arg_2
					) { /* no source */ }
	inline			~profile_character			( ) { /* no source */ }

	inline	void	update						( const u32 arg_0 ) { /* no source */ }

	inline	void	clear_resources				( ) { /* no source */ }

	inline	void	profile_changed				( player_profile const* arg_0 ) { /* no source */ }

	inline	void	query_profile_contents		( player_profile const* arg_0 ) { /* no source */ }
	inline	void	query_character_animations	( ) { /* no source */ }

			void	character_model_ready		( resources::queries_result& data );
			void	character_animation_ready	( resources::queries_result& data );
			void	weapon_resources_ready		( resources::queries_result& data );

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	float4x4								m_initial_matrix;
	/* 0x0040 */	animation::animation_player				m_animation_player;
	/* 0x8588 */	render::skeleton_model_instance_ptr		m_character_model;
	/* 0x858c */	resources::managed_resource_ptr			m_character_animation[2];
	/* 0x8594 */	animation::skeleton_ptr					m_skeleton;
	/* 0x8598 */	profile_character::preview_weapon		m_preview_weapon[2];
	/* 0x8680 */	u32										m_weapon_bone_index;
	/* 0x8684 */	render::scene_renderer&					m_scene_renderer;
	/* 0x8688 */	render::base_scene_ptr const&			m_scene;
	/* 0x868c */	items_dictionary&						m_items_dictionary;
}; // class profile_character

STATIC_SIZE_ASSERT(profile_character, 0x8690);

} // namespace survarium

#endif // #ifndef PROFILE_CHARACTER_H_INCLUDED
