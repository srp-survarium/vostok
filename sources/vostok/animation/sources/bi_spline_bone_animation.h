////////////////////////////////////////////////////////////////////////////
//	Created		: 19.01.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef BI_SPLINE_BONE_ANIMATION_H_INCLUDED
#define BI_SPLINE_BONE_ANIMATION_H_INCLUDED

#include <vostok/animation/anim_track_common.h>
#include <vostok/animation/bi_spline_data.h>

namespace vostok {
namespace animation {

class bone_animation;

class bi_spline_bone_animation {
public:
	typedef bi_spline_data< float >			animation_channel_data;

public:
	animation_channel_data&			channel	( enum_channel_id ch );
	animation_channel_data const&	channel	( enum_channel_id ch )const ;
	bool							compare	( bi_spline_bone_animation const &d )const;
	void							write	( stream &file )const;
	void							load	( vostok::configs::binary_config_value const& config  );
#ifndef MASTER_GOLD
	void							save	( vostok::configs::lua_config_value config  )const ;
#endif

private:
	animation_channel_data			m_data[ channel_max ];
}; // class bi_spline_bone_animation

} // namespace animation
} // namespace vostok

#endif // #ifndef BI_SPLINE_BONE_ANIMATION_H_INCLUDED