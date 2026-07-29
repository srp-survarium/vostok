////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONFIGURATION_H_INCLUDED
#define RENDER_SHADER_CONFIGURATION_H_INCLUDED

/* INCLUDES */
struct vostok::render::union_base::shader_configuration;

namespace vostok {
namespace render {

struct shader_configuration : public union_base::shader_configuration {
	inline			shader_configuration( ) { /* no source */ }

	inline	void	reset				( ) { /* no source */ }
}; // struct shader_configuration

STATIC_SIZE_ASSERT(shader_configuration, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONFIGURATION_H_INCLUDED
