#include "pch.h"
#include "effect_material_base.h"
#include "material_manager.h"

namespace vostok {
namespace render {

class material_shader_base : public effect_material_base {
	material_shader_base(
		enum_vertex_input_type vertex_type,
		material_ptr material
	) :
		m_vertex_type( vertex_type ),
		m_mtl( material )
	{
	}

public:
	virtual ~material_shader_base( ) { }

private:
	u32 m_vertex_type;
	material_ptr m_mtl;
	res_effect_ptr m_shader;
};

material_manager::material_manager( ) :
	m_watcher_materials_subscribe_id			( 0 ),
	m_watcher_material_instanced_subscribe_id	( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x784bc0]
}

material_manager::~material_manager( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x784b40]
}

void material_manager::initialize_post_process_parameters(
	post_process_parameters*,
	material_ptr,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x784c00]
}

void material_manager::remove_material_effects(
	material_effects_instance_ptr const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x784990]
}

void material_manager::add_material_effects(
	material_effects_instance_ptr&,
	fs_new::virtual_path_string const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7849f0]
}

} // namespace render
} // namespace vostok
