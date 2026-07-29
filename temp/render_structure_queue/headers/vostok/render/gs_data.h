////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GS_DATA_H_INCLUDED
#define RENDER_GS_DATA_H_INCLUDED

/* INCLUDES */
struct ID3D11GeometryShader;
class vostok::fixed_vector<vostok::render::sampler_slot,16>;
class vostok::fixed_vector<vostok::render::texture_slot,128>;
class vostok::render::shader_constant_table;
struct vostok::render::shader_data_base;

namespace vostok {
namespace render {

struct gs_data : public shader_data_base {
	inline		gs_data	( ) { /* no source */ }
	inline		~gs_data( ) { /* no source */ }

	/* 0x0000 */	/* shader_data_base */
	/* 0x0004 */	ID3D11GeometryShader*					hardware_shader;
	/* 0x0008 */	shader_constant_table					constants;
	/* 0x0028 */	fixed_vector< sampler_slot, 16 >		samplers;
	/* 0x0570 */	fixed_vector< texture_slot, 128 >		textures;
}; // struct gs_data

STATIC_SIZE_ASSERT(gs_data, 0x2F78);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GS_DATA_H_INCLUDED
