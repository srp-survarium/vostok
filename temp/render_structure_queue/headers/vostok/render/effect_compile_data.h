////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EFFECT_COMPILE_DATA_H_INCLUDED
#define RENDER_EFFECT_COMPILE_DATA_H_INCLUDED

/* INCLUDES */
class vostok::render::effect_descriptor;
typedef vostok::intrusive_ptr<vostok::render::custom_config,vostok::render::custom_config,vostok::threading::simple_lock>
	vostok::render::custom_config_ptr;
class vostok::render::custom_config;

namespace vostok {
namespace render {

struct effect_compile_data {
	inline		effect_compile_data	(
					effect_descriptor*		arg_0,
					custom_config_ptr		arg_1,
					u32						arg_2,
					bool					arg_3
				) { /* no source */ }
	inline		~effect_compile_data( ) { /* no source */ }

	/* 0x0000 */	effect_descriptor*		descriptor;
	/* 0x0004 */	custom_config_ptr		config;
	/* 0x0008 */	u32						crc;
	/* 0x000c */	bool					add_to_array;
}; // struct effect_compile_data

STATIC_SIZE_ASSERT(effect_compile_data, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EFFECT_COMPILE_DATA_H_INCLUDED
