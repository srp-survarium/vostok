// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SOUND_ENVIRONMENT_H_INCLUDED
#define SOUND_ENVIRONMENT_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace collision {
class object;
} // namespace collision

namespace sound {

class sound_environment : public resources::unmanaged_resource, private boost::noncopyable
{
public:
	explicit					sound_environment	( u32 env_params_id );
	virtual						~sound_environment	( );

	inline collision::object*	collision			( ) { return m_collision; }
	inline u32					env_params_id		( ) const { return m_env_params_id; }

private:
	collision::object*			m_collision;
	u32							m_env_params_id;
}; // class sound_environment

STATIC_SIZE_ASSERT			( sound_environment, 0x110 );

} // namespace sound
} // namespace vostok

#endif // #ifndef SOUND_ENVIRONMENT_H_INCLUDED
