////////////////////////////////////////////////////////////////////////////
//	Created		: 01.02.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////



namespace vostok {
namespace animation {

inline bool hemi_distant( float angle_dist )
{
	return abs( angle_dist ) >= math::pi_d2-math::epsilon_6;
}

//angle_rotation_representation_reduce





inline void evaluate_frame( float time, const animation_curve_type channels[channel_max], frame &f )
{
	for( u32  ch = channel_translation_x; ch < channel_max; ++ch )
		channels[ch].evaluate( time, f.channels[ch] );
}

inline void evaluate_frame( float time, const animation_curve_type channels[channel_max], frame &f, current_frame_position &frame_position )
{
	for( u32  ch = channel_translation_x; ch < channel_max; ++ch )
		channels[ch].evaluate( time, f.channels[ch], frame_position.current_domain( enum_channel_id( ch ) ) );
}

inline void bone_animation::get_frame( float time, frame &f )const
{
	evaluate_frame( time, m_channels, f );
}




} // namespace animation
} // namespace vostok