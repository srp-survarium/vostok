#ifndef VOSTOK_RENDER_CORE_EFFECT_COMPILE_DATA_H_INCLUDED
#define VOSTOK_RENDER_CORE_EFFECT_COMPILE_DATA_H_INCLUDED

#include <vostok/render/core/custom_config.h>

namespace vostok {
namespace render {

class effect_descriptor;

struct effect_compile_data {
	effect_compile_data(
		effect_descriptor* descriptor,
		custom_config_ptr config,
		u32 crc,
		bool add_to_array = true
	)
		: descriptor( descriptor ),
		  config( config ),
		  crc( crc ),
		  add_to_array( add_to_array )
	{
	}

	effect_descriptor* descriptor;
	custom_config_ptr config;
	u32 crc;
	bool add_to_array;
};

STATIC_SIZE_ASSERT( effect_compile_data, 0x10 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_EFFECT_COMPILE_DATA_H_INCLUDED
