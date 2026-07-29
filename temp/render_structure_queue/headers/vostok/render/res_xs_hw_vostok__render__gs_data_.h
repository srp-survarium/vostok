////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_XS_HW_H_INCLUDED
#define RENDER_RES_XS_HW_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
class vostok::shared_string;
struct vostok::render::gs_data;

/* FORWARD REFS */
class vostok::fixed_vector<vostok::render::sampler_slot,16>;
class vostok::fixed_vector<vostok::render::texture_slot,128>;

namespace vostok {
namespace render {

class res_xs_hw< gs_data > : public resource_intrusive_base {
	inline								res_xs_hw<vostok::render::gs_data>	( ) { /* no source */ }
	inline								~res_xs_hw<vostok::render::gs_data>	( ) { /* no source */ }

	inline	void						destroy_impl						( ) const { /* no source */ }

public:
	inline	HRESULT						create_hw_shader					( ID3D10Blob* shader_code ) { /* no source */ }
private:
			HRESULT						create_hw_shader					( ID3D10Blob* shader_code, ID3D11GeometryShader** hardware_shader );
public:
	inline	ID3D11GeometryShader*		hardware_shader						( ) const { /* no source */ }

	inline	gs_data const&				data								( ) const { /* no source */ }

	inline	bool						is_registered						( ) const { /* no source */ }
	inline	void						mark_registered						( ) { /* no source */ }

	inline	shared_string const&		name								( ) const { /* no source */ }

	inline	void						set_name							( shared_string const& arg_0 ) { /* no source */ }

private:
	inline	void						parse_resources						(
											ID3D11ShaderReflection*					shader_reflection,
											fixed_vector< sampler_slot, 16 >&		samplers,
											fixed_vector< texture_slot, 128 >&		textures
										) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	gs_data				m_shader_data;
	/* 0x2f7c */	shared_string		m_name;
	/* 0x2f80 */	bool				m_is_registered;
}; // class res_xs_hw< gs_data >

STATIC_SIZE_ASSERT(res_xs_hw< gs_data >, 0x2F84);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_XS_HW_H_INCLUDED
