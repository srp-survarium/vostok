////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_MANAGER_EFFECT_HOLDER_STRUCT_H_INCLUDED
#define RENDER_EFFECT_MANAGER_EFFECT_HOLDER_STRUCT_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;
class vostok::render::res_effect;
typedef vostok::intrusive_ptr<vostok::render::custom_config,vostok::render::custom_config,vostok::threading::simple_lock>
	vostok::render::custom_config_ptr;
class vostok::render::custom_config;

namespace vostok {
namespace render {

struct effect_manager::effect_holder_struct {
	inline		effect_holder_struct	( ) { /* no source */ }
	inline		~effect_holder_struct	( ) { /* no source */ }

	/* 0x0000 */	effect_descriptor*		descriptor;
	/* 0x0004 */	custom_config_ptr		config;
	/* 0x0008 */	res_effect*				effect;
}; // struct effect_manager::effect_holder_struct

STATIC_SIZE_ASSERT(effect_manager::effect_holder_struct, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_MANAGER_EFFECT_HOLDER_STRUCT_H_INCLUDED
