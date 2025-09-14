////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.02.2011
//	Author		: Dmitriy Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_WORLD_USER_BASE_H_INCLUDED
#define VOSTOK_WORLD_USER_BASE_H_INCLUDED

#include <vostok/sound/api.h>
#include <vostok/sound/sound.h>

namespace vostok {
namespace sound {

struct sound_order;
class sound_receiver;
struct sound_producer;
struct sound_response;

class VOSTOK_NOVTABLE world_user_base
{
public:
	virtual	void		initialize				( ) = 0;
	virtual	void		finalize				( ) = 0;

	virtual void		mute_all_sounds			( bool mute ) = 0;

	virtual void		set_listener_properties	( sound_scene_ptr& scene, float4x4 const& inv_view_matrix ) = 0;

	virtual	void		set_active_sound_scene	(	sound_scene_ptr& scene,
													u32 fade_in_time,
													u32 fade_out_old_scene_time  )	= 0;


	virtual void		register_receiver		( sound_scene_ptr& scene, sound_receiver& receiver ) = 0;
	virtual void		move_receiver			( sound_scene_ptr& scene, sound_receiver& receiver ) = 0;
	virtual void		unregister_receiver		( sound_scene_ptr& scene, sound_receiver& receiver ) = 0;

	virtual void		dispatch_callbacks		( ) = 0;
protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR		( world_user_base )
}; // class world_user_base

} // namespace sound
} // namespace vostok

#endif // #ifndef VOSTOK_WORLD_USER_BASE_H_INCLUDED