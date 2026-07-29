#ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_SURFACE_H_INCLUDED

#include "material_effects_instance.h"

namespace vostok {

namespace configs {

class binary_config_value;

} // namespace configs

namespace memory {

class chunk_reader;

} // namespace memory

namespace render {

class material_effects;

class render_surface : private boost::noncopyable {
public:
	render_surface( ) { }
	virtual ~render_surface( );

	virtual void load( configs::binary_config_value const& properties, memory::chunk_reader& chunk );
	void set_material_effects( material_effects_instance_ptr mtl_instance_ptr, pcstr material_name );
	void set_default_material( );
	virtual bool is_dynamic( ) const = 0;
	material_effects& get_material_effects( );
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_SURFACE_H_INCLUDED
