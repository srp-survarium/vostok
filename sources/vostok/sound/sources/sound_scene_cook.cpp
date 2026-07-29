////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_scene_cook.h"
#include "sound_scene.h"
#include "sound_world.h"
#include <vostok/sound/sound_scene_creation_params.h>

namespace vostok {
namespace sound {

sound_scene_cook::sound_scene_cook	( sound_world& world ) :
	resources::translate_query_cook	( resources::sound_scene_class, reuse_false, use_current_thread_id ),
	m_sound_world				( world ) 
{
}

void sound_scene_cook::translate_query	( resources::query_result_for_cook& parent )
{
	static u32 id						= 0;
	sound_scene_creation_params params;
	bool const result					= parent.user_data( )->try_get( params );
	R_ASSERT							( result );

	sound_scene* created_scene			= NEW( sound_scene )
		( m_sound_world, params, m_sound_world.create_submix_voice( 2, 2 ), id++, parent );

	parent.set_unmanaged_resource		( created_scene, resources::nocache_memory, sizeof( sound_scene ) );
	parent.finish_query					( result_success );
}

void sound_scene_cook::delete_resource	( resources::resource_base* resource )
{
	sound_scene* scene					= static_cast_checked< sound_scene* >( resource );
	scene->clear_resources				( );
	DELETE								( scene );
}


} // namespace sound
} // namespace vostok
