#ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED

#include <vostok/fs/virtual_path_string.h>
#include <vostok/resources_unmanaged_resource.h>

#include "material_effects.h"

namespace vostok {
namespace render {

class material_effects_instance_cook;

class material_effects_instance : public resources::unmanaged_resource {
public:
	material_effects& get_material_effects( )
	{
		return m_material_effects;
	}

	void set_material_effects( material_effects const& effects )
	{
		m_material_effects = effects;
	}

	fs_new::virtual_path_string const& get_material_name( ) const
	{
		return m_material_name;
	}

	virtual ~material_effects_instance( ) { }

private:
	friend class material_effects_instance_cook;

	material_effects			m_material_effects;
	fs_new::virtual_path_string	m_material_name;
};

STATIC_SIZE_ASSERT( material_effects_instance, 0x5B0 );

typedef resources::resource_ptr<
	material_effects_instance,
	resources::unmanaged_intrusive_base
> material_effects_instance_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MATERIAL_EFFECTS_INSTANCE_H_INCLUDED
