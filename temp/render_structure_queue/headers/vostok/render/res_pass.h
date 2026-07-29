////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RES_PASS_H_INCLUDED
#define RENDER_RES_PASS_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
typedef vostok::intrusive_ptr<vostok::render::res_input_layout,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_input_layout_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_state,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_state_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_xs<vostok::render::gs_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs<vostok::render::gs_data>_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_xs<vostok::render::ps_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs<vostok::render::ps_data>_ptr;
typedef vostok::intrusive_ptr<vostok::render::res_xs<vostok::render::vs_data>,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_xs<vostok::render::vs_data>_ptr;
class vostok::render::res_input_layout;
class vostok::render::res_state;
class vostok::render::res_xs<vostok::render::gs_data>;
class vostok::render::res_xs<vostok::render::ps_data>;
class vostok::render::res_xs<vostok::render::vs_data>;

/* FORWARD REFS */
class vostok::render::res_declaration;

namespace vostok {
namespace render {

class res_pass : public resource_intrusive_base {
public:
	inline							res_pass		(
										res_xs< vs_data >_ptr const&	vs,
										res_xs< gs_data >_ptr const&	gs,
										res_xs< ps_data >_ptr const&	ps,
										res_state_ptr const&			state
									) { /* no source */ }

	inline	void					mark_registered	( ) { /* no source */ }
	inline	bool					is_registered	( ) const { /* no source */ }

									~res_pass		( );

			void					destroy_impl	( ) const;

	inline	void					init_layout		( res_declaration const& arg_0 ) { /* no source */ }

	inline	bool					equal			( res_pass const& arg_0 ) const { /* no source */ }
	inline	bool					equal			( res_pass const* arg_0 ) const { /* no source */ }

			void					apply			( ) const;

	inline	u64						get_priority	( ) const { /* no source */ }
	inline	res_xs< vs_data >*		get_vs			( ) const { /* no source */ }
	inline	res_xs< ps_data >*		get_ps			( ) const { /* no source */ }
	inline	res_xs< gs_data >*		get_gs			( ) const { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	res_state_ptr				m_state;
public:
	/* 0x0008 */	res_xs< vs_data >_ptr		m_vs;
	/* 0x000c */	res_xs< gs_data >_ptr		m_gs;
	/* 0x0010 */	res_xs< ps_data >_ptr		m_ps;
private:
	/* 0x0014 */	res_input_layout_ptr		m_input_layout;
	/* 0x0018 */	bool						m_registered;
}; // class res_pass

STATIC_SIZE_ASSERT(res_pass, 0x1C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RES_PASS_H_INCLUDED
