////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_CREATE_SOUND_PROPAGATOR_PARAMS_H_INCLUDED
#define SOUND_CREATE_SOUND_PROPAGATOR_PARAMS_H_INCLUDED

#include <vostok/sound/sound.h>

namespace vostok {
namespace sound {

class sound_instance_proxy_internal;
class sound_producer;
class sound_receiver;

struct create_sound_propagator_params
{
	create_sound_propagator_params
	(
		playback_mode mode,
		sound_instance_proxy_internal& proxy,
		u32 playback_id,
		sound_producer const* const producer = 0,
		sound_receiver const* const ignorable_receiver = 0
	) :
		m_mode					( mode ),
		m_proxy					( proxy ),
		m_playback_id			( playback_id ),
		m_producer				( producer ),
		m_ignorable_receiver	( ignorable_receiver ),
		m_type					( point )
	{
	}

	create_sound_propagator_params
	(
		source_params const& params,
		playback_mode mode,
		sound_instance_proxy_internal& proxy,
		u32 playback_id,
		sound_producer const* const producer = 0,
		sound_receiver const* const ignorable_receiver = 0
	) :
		m_direction				( params.m_direction ),
		m_cone_inner_angle		( params.m_cone_inner_angle ),
		m_cone_outer_angle		( params.m_cone_outer_angle ),
		m_outer_gain			( params.m_outer_gain ),
		m_mode					( mode ),
		m_proxy					( proxy ),
		m_playback_id			( playback_id ),
		m_producer				( producer ),
		m_ignorable_receiver	( ignorable_receiver ),
		m_type					( cone )
	{
	}

private:
	create_sound_propagator_params const& operator=	( create_sound_propagator_params const& );

public:
	float3								m_direction;
	float								m_cone_inner_angle;
	float								m_cone_outer_angle;
	float								m_outer_gain;
	u32									m_playback_id;
	sound_instance_proxy_internal&		m_proxy;
	sound_producer const* const			m_producer;
	sound_receiver const*				m_ignorable_receiver;
	playback_mode						m_mode;
	enum type
	{
		point,
		cone
	}									m_type;
}; // struct create_sound_propagator_params

STATIC_SIZE_ASSERT( create_sound_propagator_params, 0x30 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_CREATE_SOUND_PROPAGATOR_PARAMS_H_INCLUDED
