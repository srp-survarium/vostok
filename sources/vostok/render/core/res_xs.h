// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_RES_XS_H_INCLUDED
#define VOSTOK_RENDER_CORE_RES_XS_H_INCLUDED
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/shader_constant_table.h>
#include <vostok/render/core/dx11/res_xs_hw.h>
#include <vostok/render/core/dx11/xs_descriptor.h>

namespace vostok {
namespace render {

class resource_manager;
class res_effect;

template < typename shader_data >
class res_xs : public resource_intrusive_base {
	typedef intrusive_ptr<
		res_xs_hw<shader_data>,
		resource_intrusive_base,
		threading::single_threading_policy
	> ref_xs_hw;

	friend class resource_intrusive_base;
	friend class resource_manager;
	friend class res_effect;

	explicit res_xs( xs_descriptor<shader_data> const& binder );

	void destroy_impl( ) const;

public:
	bool equal( xs_descriptor<shader_data> const& descriptor ) const
	{
		return compare( descriptor ) == 0;
	}

	s32 compare( xs_descriptor<shader_data> const& descriptor ) const;

	bool less( xs_descriptor<shader_data> const& descriptor ) const
	{
		return compare( descriptor ) < 0;
	}

	friend s32 compare( res_xs<shader_data> const& left, res_xs<shader_data> const& right )
	{
		s32 result = left.m_hardware_shader < right.m_hardware_shader ? -1
			: right.m_hardware_shader < left.m_hardware_shader ? 1 : 0;
		if ( result )
			return result;

		result = left.m_constants->compare( *right.m_constants );
		if ( result )
			return result;

		result = left.m_textures->compare( *right.m_textures );
		if ( result )
			return result;

		return left.m_samplers->compare( *right.m_samplers );
	}

	void apply( ) const;

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }

	void rebind_samplers( )
	{
		if ( m_samplers )
			m_samplers->rebind( );
	}

	res_xs_hw<shader_data> const* hardware_shader( )
	{
		return m_hardware_shader.c_ptr( );
	}

private:
	ref_xs_hw m_hardware_shader;
	shader_constant_table_ptr m_constants;

public:
	res_texture_list_ptr m_textures;

private:
	res_sampler_list_ptr m_samplers;
	bool m_is_registered;
};

typedef res_xs<vs_data> res_vs;
typedef res_xs<gs_data> res_gs;
typedef res_xs<ps_data> res_ps;

STATIC_SIZE_ASSERT( res_xs<vs_data>, 0x18 );
STATIC_SIZE_ASSERT( res_xs<gs_data>, 0x18 );
STATIC_SIZE_ASSERT( res_xs<ps_data>, 0x18 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_RES_XS_H_INCLUDED
