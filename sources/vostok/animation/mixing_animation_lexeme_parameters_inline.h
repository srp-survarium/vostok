////////////////////////////////////////////////////////////////////////////
//	Created		: 28.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_MIXING_ANIMATION_LEXEME_PARAMETERS_INLINE_H_INCLUDED
#define VOSTOK_ANIMATION_MIXING_ANIMATION_LEXEME_PARAMETERS_INLINE_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

inline animation_lexeme_parameters::animation_lexeme_parameters	(
		mutable_buffer& buffer,
		pcstr identifier,
		animation_interval const* animation_intervals_begin,
		animation_interval const* animation_intervals_end,
		animation_lexeme* const	time_driving_animation,
		animation_lexeme* const	weight_driving_animation
	) :
	m_buffer							( buffer ),
	m_time_driving_animation			( time_driving_animation ),
	m_weight_driving_animation			( weight_driving_animation ),
#ifndef MASTER_GOLD
	m_identifier						( identifier ),
#endif // #ifndef MASTER_GOLD
	m_user_data							( memory::uninitialized_value<u32>() ),
	m_animation_intervals				( animation_intervals_begin ),
	m_animation_intervals_count			( u32(animation_intervals_end - animation_intervals_begin) ),
	m_weight_interpolator				( 0 ),
	m_time_scale_interpolator			( 0 ),
	m_start_animation_interval_id		( 0 ),
	m_start_animation_interval_time		( 0.f ),
	m_start_cycle_animation_interval_id	( 0 ),
	m_time_scale						( 1.f ),
	m_playback_type						( play_cyclically ),
	m_time_synchronization_group_id		( u32(-1) ),
	m_weight_synchronization_group_id	( u32(-1) ),
	m_additivity_priority				( 0 ),
	m_override_existing_animation		( false )
{
#ifdef MASTER_GOLD
	VOSTOK_UNREFERENCED_PARAMETERS		( identifier );
#endif
}

template < int AnimationIntervalsCount >
inline animation_lexeme_parameters::animation_lexeme_parameters	(
		mutable_buffer& buffer,
		pcstr identifier,
		animation_interval const (&animation_intervals)[ AnimationIntervalsCount ],
		animation_lexeme* const	time_driving_animation,
		animation_lexeme* const	weight_driving_animation
	) :
	m_buffer							( buffer ),
	m_time_driving_animation			( time_driving_animation ),
	m_weight_driving_animation			( weight_driving_animation ),
#ifndef MASTER_GOLD
	m_identifier						( identifier ),
#endif // #ifndef MASTER_GOLD
	m_user_data							( memory::uninitialized_value<u32>() ),
	m_animation_intervals				( &animation_intervals[0] ),
	m_animation_intervals_count			( AnimationIntervalsCount ),
	m_weight_interpolator				( 0 ),
	m_time_scale_interpolator			( 0 ),
	m_start_animation_interval_id		( 0 ),
	m_start_animation_interval_time		( 0.f ),
	m_start_cycle_animation_interval_id	( 0 ),
	m_time_scale						( 1.f ),
	m_playback_type						( play_cyclically ),
	m_time_synchronization_group_id		( u32(-1) ),
	m_weight_synchronization_group_id	( u32(-1) ),
	m_additivity_priority				( 0 ),
	m_override_existing_animation		( false )
{
}

inline animation_lexeme_parameters::animation_lexeme_parameters	(
		mutable_buffer& buffer,
		pcstr identifier,
		animation_interval const& animation_interval,
		animation_lexeme* const	time_driving_animation,
		animation_lexeme* const	weight_driving_animation
	) :
	m_buffer							( buffer ),
	m_time_driving_animation			( time_driving_animation ),
	m_weight_driving_animation			( weight_driving_animation ),
#ifndef MASTER_GOLD
	m_identifier						( identifier ),
#endif // #ifndef MASTER_GOLD
	m_user_data							( memory::uninitialized_value<u32>() ),
	m_animation_intervals				( &animation_interval ),
	m_animation_intervals_count			( 1 ),
	m_weight_interpolator				( 0 ),
	m_time_scale_interpolator			( 0 ),
	m_start_animation_interval_id		( 0 ),
	m_start_animation_interval_time		( 0.f ),
	m_start_cycle_animation_interval_id	( 0 ),
	m_time_scale						( 1.f ),
	m_playback_type						( play_cyclically ),
	m_time_synchronization_group_id		( u32(-1) ),
	m_weight_synchronization_group_id	( u32(-1) ),
	m_additivity_priority				( 0 ),
	m_override_existing_animation		( false )
{
#ifndef MASTER_GOLD
#else
	VOSTOK_UNREFERENCED_PARAMETERS		( identifier );
#endif // #ifndef MASTER_GOLD
}

inline animation_lexeme_parameters::animation_lexeme_parameters	(
		mutable_buffer& buffer,
		pcstr identifier,
		skeleton_animation_ptr const& animation,
		animation_lexeme* const	time_driving_animation,
		animation_lexeme* const	weight_driving_animation
	) :
	m_buffer							( buffer ),
	m_time_driving_animation			( time_driving_animation ),
	m_weight_driving_animation			( weight_driving_animation ),
#ifndef MASTER_GOLD
	m_identifier						( identifier ),
#endif // #ifndef MASTER_GOLD
	m_user_data							( memory::uninitialized_value<u32>() ),
	m_animation_intervals				( static_cast<animation_interval const*>( buffer.c_ptr() ) ),
	m_animation_intervals_count			( animation_intervals_count(animation) ),
	m_weight_interpolator				( 0 ),
	m_time_scale_interpolator			( 0 ),
	m_start_animation_interval_id		( 0 ),
	m_start_animation_interval_time		( 0.f ),
	m_start_cycle_animation_interval_id	( 0 ),
	m_time_scale						( 1.f ),
	m_playback_type						( play_cyclically ),
	m_time_synchronization_group_id		( u32(-1) ),
	m_weight_synchronization_group_id	( u32(-1) ),
	m_additivity_priority				( 0 ),
	m_override_existing_animation		( false )
{
	create_animation_intervals			( animation );
#ifndef MASTER_GOLD
#else
	VOSTOK_UNREFERENCED_PARAMETERS		( identifier );
#endif // #ifndef MASTER_GOLD
}


inline animation_lexeme_parameters&	animation_lexeme_parameters::start_animation_interval_id		( u32 const start_animation_interval_id )
{
	R_ASSERT_CMP						( start_animation_interval_id, <, m_animation_intervals_count );
	m_start_animation_interval_id		= start_animation_interval_id;
	return								*this;
}

inline animation_lexeme_parameters&	animation_lexeme_parameters::start_animation_interval_time		( float const start_animation_interval_time )
{
	R_ASSERT_CMP						( start_animation_interval_time, >=, 0.f );
	R_ASSERT_CMP						( start_animation_interval_time, <=, m_animation_intervals[m_start_animation_interval_id].length() );
	m_start_animation_interval_time		= start_animation_interval_time;
	return								*this;
}

inline animation_lexeme_parameters&	animation_lexeme_parameters::start_cycle_animation_interval_id	( u32 const start_cycle_animation_interval_id )
{
	R_ASSERT_CMP						( start_cycle_animation_interval_id, <, m_animation_intervals_count );
	m_start_cycle_animation_interval_id	= start_cycle_animation_interval_id;
	return								*this;
}

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_MIXING_ANIMATION_LEXEME_PARAMETERS_INLINE_H_INCLUDED