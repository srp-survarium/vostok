////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_VS_DATA_H_INCLUDED
#define RENDER_VS_DATA_H_INCLUDED

/* INCLUDES */
struct ID3D11VertexShader;
class vostok::fixed_vector<vostok::render::sampler_slot,16>;
class vostok::fixed_vector<vostok::render::texture_slot,128>;
class vostok::render::shader_constant_table;
struct vostok::render::shader_data_base;
typedef vostok::intrusive_ptr<vostok::render::res_signature const ,vostok::render::res_signature const ,vostok::threading::single_threading_policy>
	vostok::render::res_signature const _ptr;
class vostok::render::res_signature const ;

namespace vostok {
namespace render {

struct vs_data : public shader_data_base {
	inline		vs_data	( ) { /* no source */ }
	inline		~vs_data( ) { /* no source */ }

	/* 0x0000 */	/* shader_data_base */
	/* 0x0004 */	ID3D11VertexShader*						hardware_shader;
	/* 0x0008 */	shader_constant_table					constants;
	/* 0x0028 */	fixed_vector< sampler_slot, 16 >		samplers;
	/* 0x0570 */	fixed_vector< texture_slot, 128 >		textures;
	/* 0x2f78 */	res_signature const _ptr				signature;
}; // struct vs_data

STATIC_SIZE_ASSERT(vs_data, 0x2F7C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_VS_DATA_H_INCLUDED
