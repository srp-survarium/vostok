////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SAMPLERS_HANDLER_H_INCLUDED
#define RENDER_SAMPLERS_HANDLER_H_INCLUDED

/* INCLUDES */
struct ID3D11SamplerState;
class vostok::render::res_xs_hw<vostok::render::ps_data>;
typedef vostok::intrusive_ptr<vostok::render::res_sampler_list const ,vostok::render::resource_intrusive_base const ,vostok::threading::single_threading_policy>
	vostok::render::res_sampler_list const _ptr;
class vostok::render::res_sampler_list const ;

/* FORWARD REFS */
class vostok::render::res_sampler_list;
class vostok::render::res_xs_hw<vostok::render::ps_data> const*;

namespace vostok {
namespace render {

class samplers_handler< 1 > : public boost::noncopyable {
	inline	explicit	samplers_handler<1>		( res_xs_hw< ps_data > const* const& arg_0 ) { /* no source */ }

	inline	void		assign					( res_sampler_list const* arg_0 ) { /* no source */ }

			void		apply					( );

	inline	void		fill_changes_buffer		( ID3D11SamplerState** arg_0, u32& arg_1 ) { /* no source */ }

	inline	void		check_for_unset_samplers( ) { /* no source */ }

public:
	inline				~samplers_handler<1>	( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
private:
	/* 0x0000 */	u32										m_diff_range_start;
	/* 0x0004 */	u32										m_diff_range_end;
	/* 0x0008 */	ID3D11SamplerState*						m_tmp_buffer[16];
	/* 0x0048 */	res_sampler_list const _ptr				m_current;
	/* 0x004c */	res_xs_hw< ps_data > const* const&		m_shader;
}; // class samplers_handler< 1 >

STATIC_SIZE_ASSERT(samplers_handler< 1 >, 0x50);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SAMPLERS_HANDLER_H_INCLUDED
