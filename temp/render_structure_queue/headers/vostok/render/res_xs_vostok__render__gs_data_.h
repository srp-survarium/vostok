////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_XS_H_INCLUDED
#define RENDER_RES_XS_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
typedef vostok::intrusive_ptr<vostok::render::res_sampler_list,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_sampler_list_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_texture_list,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_texture_list_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_xs_hw<vostok::render::gs_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs_hw<vostok::render::gs_data>_ptr;
typedef vostok::intrusive_ptr<vostok::render::shader_constant_table,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::shader_constant_table_ptr;
class vostok::render::res_sampler_list;
class vostok::render::res_texture_list;
class vostok::render::res_xs_hw<vostok::render::gs_data>;
class vostok::render::shader_constant_table;

/* FORWARD REFS */
class vostok::render::xs_descriptor<vostok::render::gs_data>;

namespace vostok {
namespace render {

class res_xs< gs_data > : public resource_intrusive_base {
	inline	explicit						res_xs<vostok::render::gs_data>	( xs_descriptor< gs_data > const& arg_0 ) { /* no source */ }

	inline	void							destroy_impl					( ) const { /* no source */ }

public:
	inline	bool							equal							( xs_descriptor< gs_data > const& arg_0 ) const { /* no source */ }

	inline	s32								compare							( xs_descriptor< gs_data > const& arg_0 ) const { /* no source */ }

	inline	bool							less							( xs_descriptor< gs_data > const& arg_0 ) const { /* no source */ }

			void							apply							( ) const;

	inline	bool							is_registered					( ) const { /* no source */ }
	inline	void							mark_registered					( ) { /* no source */ }

	inline	void							rebind_samplers					( ) { /* no source */ }

	inline	res_xs_hw< gs_data > const*		hardware_shader					( ) { /* no source */ }

	inline									~res_xs<vostok::render::gs_data>( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	res_xs_hw< gs_data >_ptr		m_hardware_shader;
	/* 0x0008 */	shader_constant_table_ptr		m_constants;
public:
	/* 0x000c */	res_texture_list_ptr			m_textures;
private:
	/* 0x0010 */	res_sampler_list_ptr			m_samplers;
	/* 0x0014 */	bool							m_is_registered;
}; // class res_xs< gs_data >

STATIC_SIZE_ASSERT(res_xs< gs_data >, 0x18);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_XS_H_INCLUDED
