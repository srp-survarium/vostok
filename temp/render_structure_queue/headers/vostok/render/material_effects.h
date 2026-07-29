////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_EFFECTS_H_INCLUDED
#define RENDER_MATERIAL_EFFECTS_H_INCLUDED

/* INCLUDES */
struct vostok::render::post_process_parameters;
class vostok::render::enum_vertex_input_type;
typedef vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base>
	vostok::render::res_effect_ptr;
class vostok::render::res_effect;

/* FORWARD REFS */
class vostok::render::vector<vostok::render::texture_named_instance>;

namespace vostok {
namespace render {

enum D3D11_CULL_MODE
{
	D3D11_CULL_NONE		= 0x1,
	D3D11_CULL_FRONT	= 0x2,
	D3D11_CULL_BACK		= 0x3,
};
enum vostok::render::enum_vertex_input_type
{
};

struct material_effects {
										material_effects				( );

	inline	u32							get_render_complexity			( ) const { /* no source */ }
	inline	void						get_max_used_texture_dimension	( u32& arg_0, u32& arg_1 ) const { /* no source */ }
			enum_vertex_input_type		get_vertex_input_type			( ) const;
			void						get_used_textures				( vector< texture_named_instance >& out_array ) const;

	inline								~material_effects				( ) { /* no source */ }

	/* 0x0000 */	post_process_parameters		m_post_process_stage_parameters;
	/* 0x02d4 */	bool						stage_enable[29];
	/* 0x02f1 */	bool						is_emissive;
	/* 0x02f2 */	bool						is_organic;
	/* 0x02f3 */	bool						use_subsurface_scattering;
	/* 0x02f4 */	bool						is_cast_shadow;
	/* 0x02f5 */	bool						is_use_bound_normals;
	/* 0x02f6 */	bool						is_use_alpha_test;
	/* 0x02f7 */	bool						is_wind_swings;
	/* 0x02f8 */	bool						is_background_sky;
	/* 0x02f9 */	bool						has_local_reflections;
	/* 0x02fa */	bool						has_translucency;
	/* 0x02fb */	bool						has_transparency;
	/* 0x02fc */	bool						is_forward_after_fog;
	/* 0x0300 */	u32							blend_mode;
	/* 0x0304 */	float4						organic_clear_color;
	/* 0x0314 */	enum_vertex_input_type		m_vertex_input_type;
	/* 0x0318 */	D3D11_CULL_MODE				m_cull_mode;
	/* 0x031c */	res_effect_ptr				m_effects[29];
}; // struct material_effects

STATIC_SIZE_ASSERT(material_effects, 0x390);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_EFFECTS_H_INCLUDED
