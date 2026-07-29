////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURES_HANDLER_H_INCLUDED
#define RENDER_TEXTURES_HANDLER_H_INCLUDED

/* INCLUDES */
struct ID3D11ShaderResourceView;
class vostok::render::res_texture_list;
class vostok::render::res_xs_hw<vostok::render::vs_data>;
typedef vostok::intrusive_ptr<vostok::render::res_texture_list const ,vostok::render::resource_intrusive_base const ,vostok::threading::single_threading_policy>
	vostok::render::res_texture_list const _ptr;
class vostok::render::res_texture_list const ;

/* FORWARD REFS */
class vostok::render::res_texture;
class vostok::render::res_xs_hw<vostok::render::vs_data> const*;

namespace vostok {
namespace render {

class textures_handler< 0 > : public boost::noncopyable {
	inline	explicit	textures_handler<0>		( res_xs_hw< vs_data > const* const& arg_0 ) { /* no source */ }

			void		assign					( res_texture_list const* list );

			bool		set_overwrite			( pcstr name, res_texture* texture );

			void		apply					( );

			void		fill_changes_buffer		( ID3D11ShaderResourceView** buffer, s32& out_num_textures );

	inline	void		check_for_unset_textures( ) { /* no source */ }

public:
	inline				~textures_handler<0>	( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
private:
	/* 0x0000 */	res_texture_list const _ptr				m_current;
	/* 0x0004 */	u32										m_diff_range_start;
	/* 0x0008 */	u32										m_diff_range_end;
public:
	/* 0x000c */	ID3D11ShaderResourceView*				m_tmp_buffer[128];
private:
	/* 0x020c */	res_xs_hw< vs_data > const* const&		m_shader;
	/* 0x0210 */	res_texture_list						m_custom_list;
}; // class textures_handler< 0 >

STATIC_SIZE_ASSERT(textures_handler< 0 >, 0x224);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURES_HANDLER_H_INCLUDED
