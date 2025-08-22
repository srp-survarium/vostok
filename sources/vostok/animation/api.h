////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_API_H_INCLUDED
#define VOSTOK_ANIMATION_API_H_INCLUDED

#include <vostok/animation/type_definitions.h>
#include <vostok/render/engine/base_classes.h>

#ifndef VOSTOK_ANIMATION_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_ANIMATION_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_ANIMATION_BUILDING
#			define VOSTOK_ANIMATION_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_ANIMATION_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_ANIMATION_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_ANIMATION_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_ANIMATION_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_ANIMATION_API

namespace vostok {

namespace render {

class scene_renderer;

namespace debug {
	class renderer;
} // namespace game

} // namespace render

namespace animation {

struct engine;
struct world;

#ifndef MASTER_GOLD

class skeleton_animation;
struct i_editor_animation;
class i_editor_mixer;
class skeleton;
class animation_player;

namespace mixing {
	struct binary_tree_visitor;
} // namespace mixing

typedef	resources::resource_ptr <
	animation_player,
	resources::unmanaged_intrusive_base
> animation_player_ptr;

#endif // #ifndef MASTER_GOLD

typedef memory::doug_lea_allocator_type				allocator_type;

VOSTOK_ANIMATION_API	world*							create_world				(
														engine& engine,
														render::scene_ptr const& test_scene,
														render::scene_renderer* scene_renderer,
														render::debug::renderer* debug_renderer
													);
VOSTOK_ANIMATION_API	void							destroy_world				( world*& world );
VOSTOK_ANIMATION_API	void							set_memory_allocator		( allocator_type& allocator );

#ifndef MASTER_GOLD
VOSTOK_ANIMATION_API	i_editor_animation*				create_editor_animation		( allocator_type& allocator, skeleton_animation_ptr const& anim );
VOSTOK_ANIMATION_API	i_editor_animation*				create_editor_animation		( allocator_type& allocator, pcstr anim_file );
VOSTOK_ANIMATION_API	void							destroy_editor_animation	( allocator_type& allocator, i_editor_animation* &anim );

VOSTOK_ANIMATION_API	i_editor_animation*				create_editor_animation		( pcstr anim_file );
VOSTOK_ANIMATION_API	void							destroy_editor_animation	( i_editor_animation* &anim );

VOSTOK_ANIMATION_API	i_editor_mixer*					create_editor_mixer			( allocator_type& allocator, animation_player& animation_player );
VOSTOK_ANIMATION_API	void							destroy_editor_mixer		( allocator_type& allocator, i_editor_mixer* mxr );

VOSTOK_ANIMATION_API	mixing::binary_tree_visitor*	create_binary_tree_writer	( allocator_type& allocator, vostok::configs::lua_config_value& cfg, float2& pos );
VOSTOK_ANIMATION_API	void							destroy_binary_tree_writer	( allocator_type& allocator, mixing::binary_tree_visitor*& wr );
#endif // #ifndef MASTER_GOLD

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_API_H_INCLUDED