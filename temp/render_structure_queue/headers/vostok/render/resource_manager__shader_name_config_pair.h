////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_SHADER_NAME_CONFIG_PAIR_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_SHADER_NAME_CONFIG_PAIR_H_INCLUDED

/* INCLUDES */
struct vostok::render::shader_configuration;

namespace vostok {
namespace render {

struct resource_manager::shader_name_config_pair {
	inline			shader_name_config_pair	( pcstr arg_0, shader_configuration const& arg_1 ) { /* no source */ }

	inline	bool	operator<				( resource_manager::shader_name_config_pair const& other ) const { /* no source */ }

	inline	bool	operator==				( resource_manager::shader_name_config_pair const& arg_0 ) const { /* no source */ }

	/* 0x0000 */	pcstr					name;
	/* 0x0008 */	shader_configuration	config;
}; // struct resource_manager::shader_name_config_pair

STATIC_SIZE_ASSERT(resource_manager::shader_name_config_pair, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_SHADER_NAME_CONFIG_PAIR_H_INCLUDED
