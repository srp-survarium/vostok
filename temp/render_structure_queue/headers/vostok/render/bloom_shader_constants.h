////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_BLOOM_SHADER_CONSTANTS_H_INCLUDED
#define RENDER_BLOOM_SHADER_CONSTANTS_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;

namespace vostok {
namespace render {

struct bloom_shader_constants {
			bloom_shader_constants	( );

	void	set						( float bloom_scale, float bloom_max_color, float3 const& halo_color );

private:
	/* 0x0000 */	shader_constant_host*		m_bloom_parameters;
	/* 0x0004 */	shader_constant_host*		m_bloom_parameters1;
}; // struct bloom_shader_constants

STATIC_SIZE_ASSERT(bloom_shader_constants, 0x8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_BLOOM_SHADER_CONSTANTS_H_INCLUDED
