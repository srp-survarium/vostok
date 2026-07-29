////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_MANAGER_EFFECT_TO_RECOMPILE_STRUCT_H_INCLUDED
#define RENDER_EFFECT_MANAGER_EFFECT_TO_RECOMPILE_STRUCT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;
typedef vostok::intrusive_ptr<vostok::render::custom_config,vostok::render::custom_config,vostok::threading::simple_lock>
	vostok::render::custom_config_ptr;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::custom_config;
class vostok::render::res_effect;

namespace vostok {
namespace render {

struct effect_manager::effect_to_recompile_struct {
	inline		effect_to_recompile_struct	(
					res_effect_ptr			in_effect,
					effect_descriptor*		in_descriptor,
					custom_config_ptr		in_config,
					u32						in_crc
				) { /* no source */ }
	inline		~effect_to_recompile_struct	( ) { /* no source */ }

	/* 0x0000 */	res_effect_ptr			effect;
	/* 0x0004 */	effect_descriptor*		descriptor;
	/* 0x0008 */	custom_config_ptr		config;
	/* 0x000c */	u32						crc;
}; // struct effect_manager::effect_to_recompile_struct

STATIC_SIZE_ASSERT(effect_manager::effect_to_recompile_struct, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_MANAGER_EFFECT_TO_RECOMPILE_STRUCT_H_INCLUDED
