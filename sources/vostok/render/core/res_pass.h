// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_RES_PASS_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_PASS_H_INCLUDED
#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {
namespace render {

class res_declaration;
class res_input_layout;
class res_state;
template < typename shader_data > class res_xs;

typedef intrusive_ptr<
	res_input_layout,
	resource_intrusive_base,
	threading::single_threading_policy
> res_input_layout_ptr;
typedef intrusive_ptr<
	res_state,
	resource_intrusive_base,
	threading::single_threading_policy
> res_state_ptr;
typedef intrusive_ptr<
	res_xs<vs_data>,
	resource_intrusive_base,
	threading::single_threading_policy
> res_vs_ptr;
typedef intrusive_ptr<
	res_xs<gs_data>,
	resource_intrusive_base,
	threading::single_threading_policy
> res_gs_ptr;
typedef intrusive_ptr<
	res_xs<ps_data>,
	resource_intrusive_base,
	threading::single_threading_policy
> res_ps_ptr;

class res_pass : public resource_intrusive_base {
	friend s32 compare( res_pass const& left, res_pass const& right );

public:
	res_pass(
		res_vs_ptr const& vs,
		res_gs_ptr const& gs,
		res_ps_ptr const& ps,
		res_state_ptr const& state
	);

	void mark_registered( ) { m_registered = true; }
	bool is_registered( ) const { return m_registered; }

	~res_pass( );
	void destroy_impl( ) const;

	void init_layout( res_declaration const& ) { }
	bool equal( res_pass const& other ) const
	{
		return m_state == other.m_state
			&& m_vs == other.m_vs
			&& m_gs == other.m_gs
			&& m_ps == other.m_ps;
	}
	bool equal( res_pass const* other ) const { return equal( *other ); }
	void apply( ) const;
	u64 get_priority( ) const { return 0; }
	res_xs<vs_data>* get_vs( ) const { return m_vs.c_ptr( ); }
	res_xs<ps_data>* get_ps( ) const { return m_ps.c_ptr( ); }
	res_xs<gs_data>* get_gs( ) const { return m_gs.c_ptr( ); }

private:
	friend class effect_manager;

	res_state_ptr m_state;

public:
	res_vs_ptr m_vs;
	res_gs_ptr m_gs;
	res_ps_ptr m_ps;

private:
	res_input_layout_ptr m_input_layout;
	bool m_registered;
};

typedef intrusive_ptr<
	res_pass,
	resource_intrusive_base,
	threading::single_threading_policy
> res_pass_ptr;

STATIC_SIZE_ASSERT( res_pass, 0x1C );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_PASS_H_INCLUDED
