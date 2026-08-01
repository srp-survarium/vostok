#ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_HANDLERS_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_BACKEND_HANDLERS_H_INCLUDED

#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/res_sampler_list.h>
#include <vostok/render/core/res_texture_list.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/shader_constant_table.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {
namespace render {

class backend;
class res_texture;
class shader_constant_host;

template < typename shader_data >
class res_xs_hw;

typedef intrusive_ptr<
	shader_constant_table const,
	resource_intrusive_base const,
	threading::single_threading_policy
> shader_constant_table_const_ptr;

typedef intrusive_ptr<
	res_texture_list const,
	resource_intrusive_base const,
	threading::single_threading_policy
> res_texture_list_const_ptr;

typedef intrusive_ptr<
	res_sampler_list const,
	resource_intrusive_base const,
	threading::single_threading_policy
> res_sampler_list_const_ptr;

template < enum_shader_type shader_type >
class constants_handler {
private:
	friend class backend;

	constants_handler( ) :
		m_diff_range_start( 0 ),
		m_diff_range_end( 0 )
	{
	}

	template < typename T >
	void set_constant( shader_constant_host const& constant, T const& value );

	template < typename T >
	void set_constant_array(
		shader_constant_host const& constant,
		T const* values,
		u32 array_size
	);

	void assign( shader_constant_table const* table );
	void gather_data( );
	void update_buffers( );
	void apply( );
	void fill_changes_buffer( ID3D11Buffer** buffer, u32& out_num_constants );
	void check_for_unset_constants( );

	struct change_ragne {
		change_ragne( ) : start( 0 ), end( 0 ) { }
		u32 start;
		u32 end;
	};

public:
	~constants_handler( );

private:
	u32 m_diff_range_start;
	u32 m_diff_range_end;
	shader_constant_table_const_ptr m_current;
};

template < enum_shader_type shader_type >
class textures_handler : private boost::noncopyable {
private:
	friend class backend;
	typedef typename shader_type_traits< shader_type >::shader_data shader_data;

	explicit textures_handler( res_xs_hw< shader_data > const* const& shader ) :
		m_diff_range_start( 0 ),
		m_diff_range_end( 0 ),
		m_shader( shader )
	{
		vostok::memory::zero( m_tmp_buffer );
	}

	void assign( res_texture_list const* list );
	bool set_overwrite( pcstr name, res_texture* texture );
	void apply( );
	void fill_changes_buffer( ID3D11ShaderResourceView** buffer, s32& out_num_textures );
	void check_for_unset_textures( );

public:
	~textures_handler( );

private:
	res_texture_list_const_ptr m_current;
	u32 m_diff_range_start;
	u32 m_diff_range_end;

public:
	ID3D11ShaderResourceView* m_tmp_buffer[128];

private:
	res_xs_hw< shader_data > const* const& m_shader;
	res_texture_list m_custom_list;
};

template < enum_shader_type shader_type >
class samplers_handler : private boost::noncopyable {
private:
	friend class backend;
	typedef typename shader_type_traits< shader_type >::shader_data shader_data;

	explicit samplers_handler( res_xs_hw< shader_data > const* const& shader ) :
		m_diff_range_start( 0 ),
		m_diff_range_end( 0 ),
		m_shader( shader )
	{
	}

	void assign( res_sampler_list const* list );
	void apply( );
	void fill_changes_buffer( ID3D11SamplerState** buffer, u32& out_num_samplers );
	void check_for_unset_samplers( );

public:
	~samplers_handler( );

private:
	u32 m_diff_range_start;
	u32 m_diff_range_end;
	ID3D11SamplerState* m_tmp_buffer[16];
	res_sampler_list_const_ptr m_current;
	res_xs_hw< shader_data > const* const& m_shader;
};

STATIC_SIZE_ASSERT( constants_handler< enum_shader_type_vertex >, 0xC );
STATIC_SIZE_ASSERT( textures_handler< enum_shader_type_vertex >, 0x224 );
STATIC_SIZE_ASSERT( samplers_handler< enum_shader_type_vertex >, 0x50 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_HANDLERS_H_INCLUDED
