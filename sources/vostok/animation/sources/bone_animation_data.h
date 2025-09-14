////////////////////////////////////////////////////////////////////////////
//	Created		: 19.01.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef BONE_ANIMATION_DATA_H_INCLUDED
#define BONE_ANIMATION_DATA_H_INCLUDED

#include "animation_channel_data.h"
#include <vostok/animation/anim_track_common.h>
#include <vostok/animation/i_bone_animation_data.h>

namespace vostok {
namespace animation {

class bone_animation;

class bone_animation_data:
	public	i_bone_animation_data
{

public:
	virtual	animation_channel_data&				channel			( enum_channel_id ch );
	virtual animation_channel_data const&		channel			( enum_channel_id ch )const ;

public:
	virtual void								save			( vostok::configs::lua_config_value config  )const ;
	virtual void								load			( vostok::configs::lua_config_value const& config  );
	virtual	bool								compare			( i_bone_animation_data const &d )const;
			bool								compare			( bone_animation_data const &d )const;
		
private:
	animation_channel_data m_data[ channel_max ];

}; // class bone_animation_data

} // namespace animation
} // namespace vostok
#endif // #ifndef BONE_ANIMATION_DATA_H_INCLUDED