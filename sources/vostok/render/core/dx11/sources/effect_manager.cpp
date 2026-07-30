#include "pch.h"
#include <vostok/render/core/effect_loader.h>
#include <vostok/render/core/effect_manager.h>

namespace vostok {
namespace render {

void effect_loader::on_effect_ready( resources::queries_result& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f6a0]
}

effect_manager::effect_manager( )
	: m_is_effects_query_processing( false ),
#ifdef MASTER_GOLD
	  m_shader_cache_info( memory::g_mt_allocator ),
#else // #ifdef MASTER_GOLD
	  m_shader_cache_info( debug::g_mt_allocator ),
#endif // #ifdef MASTER_GOLD
	  force_sync( false ),
	  m_loading_incomplete( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f110]
}

effect_manager::~effect_manager( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77ef10]
}

res_pass* effect_manager::create_pass( res_pass const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f790]
	return 0;
}

void effect_manager::delete_pass( res_pass const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f5a0]
}

effect_descriptor* effect_manager::get_effect_descriptor_by_name( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77ed30]
	return 0;
}

void effect_manager::add_effect(
	effect_descriptor*,
	custom_config_ptr const&,
	res_effect*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77ee70]
}

void effect_manager::remove_effect( res_effect* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77ede0]
}

void effect_manager::on_effect_created(
	res_effect_ptr*,
	resources::queries_result&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f490]
}

void effect_manager::on_async_effect_created(
	resources::queries_result&,
	res_effect_ptr*,
	effect_descriptor*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f390]
}

void effect_manager::on_async_effect_created_callback(
	resources::queries_result&,
	res_effect_ptr*,
	effect_descriptor*,
	boost::function<void( )>
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f230]
}

void effect_manager::create_new_effect(
	res_effect_ptr*,
	effect_descriptor*,
	custom_config_ptr const&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f9e0]
}

res_effect_ptr effect_manager::create_new_effect(
	effect_descriptor&,
	custom_config_ptr const&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77ff50]
	return res_effect_ptr( );
}

void effect_manager::on_effects_recompiled(
	vectora<effect_to_recompile_struct>*,
	resources::queries_result&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f900]
}

res_shader_technique* effect_manager::create_effect_technique(
	res_shader_technique const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f850]
	return 0;
}

void effect_manager::delete_effect_technique( res_shader_technique const* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77f810]
}

void effect_manager::recompile_shaders_async(
	vector<fs_new::virtual_path_string> const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x77fb40]
}

void effect_manager::register_effect_desctiptor( pcstr, effect_descriptor* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x77ed60]
}

} // namespace render
} // namespace vostok
