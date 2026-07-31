#ifndef VOSTOK_RENDER_ENGINE_CLOUD_PARAMETERS_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_CLOUD_PARAMETERS_H_INCLUDED

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace render {

struct cloud_parameters {
	cloud_parameters( ) { }

	void load( configs::binary_config_value const& )
	{
		// claude@NOTE: no legacy ancestor - no cloud_* core file ever existed in the legacy corpus; matcher-phase work.
		// STATE[STUB]
	}

	u32		grid_width;
	u32		grid_height;
	float	volume_width;
	u32		noise_resulution;
	float	noise_period;
};

STATIC_SIZE_ASSERT( cloud_parameters, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_CLOUD_PARAMETERS_H_INCLUDED
