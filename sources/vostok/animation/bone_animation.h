////////////////////////////////////////////////////////////////////////////
//	Created		: 19.01.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_BONE_ANIMATION_H_INCLUDED
#define VOSTOK_ANIMATION_BONE_ANIMATION_H_INCLUDED

#include <vostok/animation/poly_curve.h>
#include <vostok/animation/anim_track_common.h>

namespace vostok {
namespace animation {

typedef poly_curve< poly_curve_order3_domain<float,1> > animation_curve_type;



 struct VOSTOK_ANIMATION_API current_frame_position
 {
	 inline	current_frame_position()
	{

			std::fill_n( &m_current_domains[0], u32( channel_max ), 0 );
	}

	inline	u32 &current_domain( enum_channel_id ch )
	{
		return m_current_domains[ ch ];
	}

	 u32 m_current_domains[ channel_max ];

 }; //  struct current_frame_position

STATIC_SIZE_ASSERT(current_frame_position, 0x24);

class bi_spline_bone_animation_baked;

class bone_animation {

public:
static					u32		count_internal_memory_size			( bi_spline_bone_animation_baked const& data );
						void	create_internals_in_place			( bi_spline_bone_animation_baked const& data, void* memory );


public:
	const	animation_curve_type	&channel				( enum_channel_id id )const		{ return m_channels[id]; }
			animation_curve_type	&channel				( enum_channel_id id )			{ return m_channels[id]; }

public:
inline		float					max_time				( )const { return m_channels[channel_translation_x].max_param(); }
inline		float					min_time				( )const { return m_channels[channel_translation_x].min_param(); }
inline		void					get_frame				( float time, frame &f, current_frame_position &frame_pos )const;
inline		void					get_end_frame_position	( current_frame_position &frame_pos)const;
			void					bone_matrix				( float time, const float3 &parent_scale, float4x4 &matrix, float3 &scale, current_frame_position &frame_pos )const;
			frame					bone_frame				( float const time, current_frame_position &frame_position ) const;

private:
	animation_curve_type	m_channels[channel_max];

}; // class bone_animation

STATIC_SIZE_ASSERT(bone_animation, 0x48);

} // namespace animation
} // namespace vostok

#include <vostok/animation/bone_animation_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_BONE_ANIMATION_H_INCLUDED
