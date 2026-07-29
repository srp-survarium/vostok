////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STATIC_MODEL_INSTANCE_USER_DATA_H_INCLUDED
#define RENDER_STATIC_MODEL_INSTANCE_USER_DATA_H_INCLUDED

/* INCLUDES */
class vostok::configs::binary_config_value;
struct vostok::sound::world;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct static_model_instance_user_data {
	inline		static_model_instance_user_data	( ) { /* no source */ }
	inline		~static_model_instance_user_data( ) { /* no source */ }

	/* 0x0000 */	configs::binary_config_value const*		config;
	/* 0x0004 */	sound::world*							sound_world;
	/* 0x0008 */	resources::unmanaged_resource_ptr		sound_scene;
}; // struct static_model_instance_user_data

STATIC_SIZE_ASSERT(static_model_instance_user_data, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STATIC_MODEL_INSTANCE_USER_DATA_H_INCLUDED
