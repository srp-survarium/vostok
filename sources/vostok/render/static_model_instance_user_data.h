// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef RENDER_STATIC_MODEL_INSTANCE_USER_DATA_H_INCLUDED
#define RENDER_STATIC_MODEL_INSTANCE_USER_DATA_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace configs {
class binary_config_value;
} // namespace configs

namespace sound {
struct world;
} // namespace sound

namespace render {

struct static_model_instance_user_data
{
	inline		static_model_instance_user_data	( ) { }
	inline		~static_model_instance_user_data( ) { }

	configs::binary_config_value const*	config;
	sound::world*						sound_world;
	resources::unmanaged_resource_ptr	sound_scene;
}; // struct static_model_instance_user_data

STATIC_SIZE_ASSERT(static_model_instance_user_data, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_MODEL_INSTANCE_USER_DATA_H_INCLUDED
