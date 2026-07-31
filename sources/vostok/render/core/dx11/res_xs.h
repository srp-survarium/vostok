#ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_RES_XS_H_INCLUDED

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
	> hardware_shader_ptr;
	typedef intrusive_ptr<
		res_texture_list,
		resource_intrusive_base,
		threading::single_threading_policy
	> texture_list_ptr;

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

	s32 compare( xs_descriptor<shader_data> const& ) const
	{
		// claude@NOTE: legacy body diverged - legacy is a bool equal(binder) &&-chain, not an s32 ordering (see the legacy res_xs.cpp remainder note); matcher-phase work.
		// STATE[STUB]
		// Observed vs_data instantiation [0x12f690].
		return 0;
	}

	bool less( xs_descriptor<shader_data> const& descriptor ) const
	{
		return compare( descriptor ) < 0;
	}

	s32 compare( res_xs<shader_data> const& ) const
	{
		// claude@NOTE: no legacy ancestor - the res_xs-vs-res_xs overload has no legacy counterpart (only the descriptor-keyed equal); matcher-phase work.
		// STATE[STUB]
		// needed by compare_shader_predicate (canonical header evidence)
		return 0;
	}

	void apply( ) const;

	bool is_registered( ) const { return m_is_registered; }
	void mark_registered( ) { m_is_registered = true; }

	void rebind_samplers( )
	{
		if ( m_samplers )
			m_samplers->rebind( );
	}

	res_xs_hw<shader_data> const* hardware_shader( ) const
	{
		return m_hardware_shader.c_ptr( );
	}

	~res_xs( ) { }

private:
	hardware_shader_ptr m_hardware_shader;
	shader_constant_table_ptr m_constants;

public:
	texture_list_ptr m_textures;

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

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_RES_XS_H_INCLUDED
