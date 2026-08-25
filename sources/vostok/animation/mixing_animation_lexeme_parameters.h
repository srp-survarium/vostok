////////////////////////////////////////////////////////////////////////////
//	Created		: 28.10.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_MIXING_ANIMATION_LEXEME_PARAMETERS_H_INCLUDED
#define VOSTOK_ANIMATION_MIXING_ANIMATION_LEXEME_PARAMETERS_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/animation/mixing.h>
#include <vostok/animation/mixing_animation_interval.h>
#include <vostok/animation/mixing_animation_lexeme.h>

namespace vostok {
namespace animation {

struct base_interpolator;

namespace mixing {

class VOSTOK_ANIMATION_API animation_lexeme_parameters : private core::noncopyable {
public:
	inline									animation_lexeme_parameters			(
												mutable_buffer& buffer,
												pcstr identifier,
												animation_interval const* animation_intervals_begin,
												animation_interval const* animation_intervals_end,
												animation_lexeme* const	time_driving_animation,
												animation_lexeme* const	weight_driving_animation
											);
	template < int AnimationIntervalsCount >
	inline									animation_lexeme_parameters			(
												mutable_buffer& buffer,
												pcstr identifier,
												animation_interval const (&animation_intervals)[ AnimationIntervalsCount ],
												animation_lexeme* const	time_driving_animation,
												animation_lexeme* const	weight_driving_animation
											);
	inline									animation_lexeme_parameters			(
												mutable_buffer& buffer,
												pcstr identifier,
												animation_interval const& animation_interval,
												animation_lexeme* const	time_driving_animation,
												animation_lexeme* const	weight_driving_animation
											);

	inline									animation_lexeme_parameters			(
												mutable_buffer&				buffer,
												pcstr						identifier,
												skeleton_animation_ptr const&	animation,
												animation_lexeme* const		time_driving_animation,
												animation_lexeme* const		weight_driving_animation
											);
	inline									~animation_lexeme_parameters		( );
public:
	typedef fastdelegate::FastDelegate< float( float, float, u32, u32, u32, float ) > time_calculator_fn;

	//
	// setters
	//
	inline	animation_lexeme_parameters&	weight_interpolator					( base_interpolator const& interpolator			) { m_weight_interpolator				= &interpolator						; return *this; }
	inline	animation_lexeme_parameters&	time_scale_interpolator				( base_interpolator const& interpolator			) { m_time_scale_interpolator			= &interpolator						; return *this; }
	inline	animation_lexeme_parameters&	user_data							( u32 const user_data							) { m_user_data							= user_data							; return *this; }
	inline	animation_lexeme_parameters&	start_animation_interval_id			( u32 const start_animation_interval_id			);
	inline	animation_lexeme_parameters&	start_animation_interval_time		( float const start_animation_interval_time		);
	inline	animation_lexeme_parameters&	start_cycle_animation_interval_id	( u32 const start_cycle_animation_interval_id	);
	inline	animation_lexeme_parameters&	time_scale							( float const time_scale						) { m_time_scale						= time_scale						; return *this; }
	// sushi@TODO: remove noinline once the retail cross-library inlining cause is recovered.
	__declspec(noinline) inline
			animation_lexeme_parameters&	playback_type						( playback_enum const playback_type				) { m_playback_type						= playback_type						; return *this; }
	__declspec(noinline) inline
			animation_lexeme_parameters&	time_synchronization_group_id		( u32 const time_synchronization_group_id		) { m_time_synchronization_group_id		= time_synchronization_group_id		; return *this; }
	__declspec(noinline) inline
			animation_lexeme_parameters&	weight_synchronization_group_id		( u32 const weight_synchronization_group_id		) { m_weight_synchronization_group_id	= weight_synchronization_group_id	; return *this; }
	inline	animation_lexeme_parameters&	additivity_priority					( u32 const additivity_priority					) { m_additivity_priority				= additivity_priority				; return *this; }
	__declspec(noinline) inline
			animation_lexeme_parameters&	bones_mask							( u32 const bones_mask							) { m_bones_mask						= bones_mask						; return *this; }
	inline	animation_lexeme_parameters&	override_existing_animation			( bool const override_existing_animation		) { m_override_existing_animation		= override_existing_animation		; return *this; }
	inline	animation_lexeme_parameters&	animated_object						( pcvoid const animated_object					) { m_animated_object					= animated_object					; return *this; }
	inline	animation_lexeme_parameters&	positive_event_direction			( bool const is_positive_event_direction		) { m_is_positive_event_direction		= is_positive_event_direction		; return *this; }
	inline	animation_lexeme_parameters&	unique_animation_id					( u8 const unique_animation_id					) { m_unique_animation_id				= unique_animation_id				; return *this; }
	__declspec(noinline) inline
			animation_lexeme_parameters&	time_calculator						( time_calculator_fn const& time_calculator		) { m_time_calculator					= time_calculator					; return *this; }
	inline	animation_lexeme_parameters&	can_generate_events					( bool const can_generate_events				) { m_can_generate_events				= can_generate_events				; return *this; }

	//
	// acceptors
	//
	inline	mutable_buffer&					buffer								( ) const { return m_buffer								; }
	inline	animation_interval const*		animation_intervals					( ) const { return m_animation_intervals				; }
	inline	time_calculator_fn const&		time_calculator						( ) const { return m_time_calculator					; }
	inline	animation_lexeme* const			time_driving_animation				( ) const { return m_time_driving_animation				; }
	inline	animation_lexeme* const			weight_driving_animation			( ) const { return m_weight_driving_animation			; }
	inline	base_interpolator const*		weight_interpolator					( ) const { return m_weight_interpolator				; }
	inline	base_interpolator const*		time_scale_interpolator				( ) const { return m_time_scale_interpolator			; }
	inline	pcvoid							animated_object						( ) const { return m_animated_object					; }
#ifndef MASTER_GOLD
	inline	pcstr							identifier							( ) const { return m_identifier							; }
#endif // #ifndef MASTER_GOLD
	inline	u32								user_data							( ) const { return m_user_data							; }
	inline	u32 const						animation_intervals_count			( ) const { return m_animation_intervals_count			; }
	inline	u32								start_animation_interval_id			( ) const { return m_start_animation_interval_id		; }
	inline	float							start_animation_interval_time		( ) const { return m_start_animation_interval_time		; }
	inline	u32								start_cycle_animation_interval_id	( ) const { return m_start_cycle_animation_interval_id	; }
	inline	float							time_scale							( ) const { return m_time_scale							; }
	inline	playback_enum					playback_type						( ) const { return m_playback_type						; }
	inline	u32								time_synchronization_group_id		( ) const { return m_time_synchronization_group_id		; }
	inline	u32								weight_synchronization_group_id		( ) const { return m_weight_synchronization_group_id	; }
	inline	u32								additivity_priority					( ) const { return m_additivity_priority				; }
	inline	u32								bones_mask							( ) const { return m_bones_mask							; }
	inline	u8								unique_animation_id					( ) const { return m_unique_animation_id				; }
	inline	bool							override_existing_animation			( ) const { return m_override_existing_animation		; }
	inline	bool							is_positive_event_direction			( ) const { return m_is_positive_event_direction		; }
	inline	bool							can_generate_events					( ) const { return m_can_generate_events				; }

private:
			void							create_animation_intervals			( skeleton_animation_ptr const& animation );
public:
	static	u32								animation_intervals_count			( skeleton_animation_ptr const& animation );
	static	animation_interval				create_animation_interval			( skeleton_animation_ptr const& animation, const u32 interval_id );

private:
	/* 0x0000 */	mutable_buffer&						m_buffer;
	/* 0x0004 */	time_calculator_fn					m_time_calculator;

	// Definition order is codegen-significant.
#ifndef MASTER_GOLD
					pcstr const							m_identifier;
#endif // #ifndef MASTER_GOLD

	/* 0x000c */	animation_lexeme* const				m_time_driving_animation;
	/* 0x0010 */	animation_lexeme* const				m_weight_driving_animation;
	/* 0x0014 */	animation_interval const* const		m_animation_intervals;
	/* 0x0018 */	base_interpolator const*			m_weight_interpolator;
	/* 0x001c */	base_interpolator const*			m_time_scale_interpolator;
	/* 0x0020 */	pcvoid								m_animated_object;

	/* 0x0024 */	u32									m_user_data;

	/* 0x0028 */	const u32							m_animation_intervals_count;
	/* 0x002c */	u32									m_start_cycle_animation_interval_id;
	/* 0x0030 */	u32									m_start_animation_interval_id;
	/* 0x0034 */	float								m_start_animation_interval_time;
	/* 0x0038 */	float								m_time_scale;
	/* 0x003c */	playback_enum						m_playback_type;
	/* 0x0040 */	u32									m_time_synchronization_group_id;
	/* 0x0044 */	u32									m_weight_synchronization_group_id;
	/* 0x0048 */	u32									m_additivity_priority;
	/* 0x004c */	u32									m_bones_mask;
	/* 0x0050 */	u8									m_unique_animation_id;
	/* 0x0051 */	bool								m_override_existing_animation;
	/* 0x0052 */	bool								m_is_positive_event_direction;
	/* 0x0053 */	bool								m_can_generate_events;
}; // class mixing_animation_lexeme_parameters

STATIC_SIZE_ASSERT(animation_lexeme_parameters, 0x54);

} // namespace mixing
} // namespace animation
} // namespace vostok

#include <vostok/animation/mixing_animation_lexeme_parameters_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_MIXING_ANIMATION_LEXEME_PARAMETERS_H_INCLUDED
