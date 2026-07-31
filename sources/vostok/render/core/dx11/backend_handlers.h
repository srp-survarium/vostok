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

template < enum_shader_type shader_type >
template < typename T >
inline void constants_handler< shader_type >::set_constant(
	shader_constant_host const& c,
	T const& arg
)
{
	// FUNCTION BODY[0x672e0] for vertex shader
	// FUNCTION BODY[0x67330] for pixel shader
	// FUNCTION BODY[0x8cad0] for geometry shader
	COMPILE_ASSERT(	constant_type_traits<T>::value == true, Only_types_described_in_constant_h_are_supported );

	u32 buffer_index = c.shader_slot(shader_type).buffer_index();
	if( c.m_update_markers[shader_type] == backend::ref().constant_update_counter()
		&& ( buffer_index = c.shader_slot(shader_type).buffer_index()) != slot_dest_buffer_null)
	{
		ASSERT( c.shader_slot(shader_type).array_size() == 1, "Single shader constant can't be set into an array.");

		ASSERT( (constant_class_cast_mask & c.shader_slot(shader_type).class_id()) == (constant_type_traits<T>::class_id&constant_class_cast_mask)
			,"Wrong type! The given object cant be cast to the corresponding shader constant slot.");
		ASSERT( (c.shader_slot(shader_type).class_id() & constant_class_size_mask) <= constant_type_traits<T>::size, "The given object has smaller dimension than the corresponding shader constant slot!");

		ASSERT( c.type() == constant_type_traits<T>::type, "The given object dosen't have the same type as the corresponding shader constant slot!");

		m_current->m_const_buffers[buffer_index]->set_typed( c.shader_slot(shader_type), arg);

#ifdef DEBUG
		render::vector<shader_constant_host*>::iterator it = std::find( m_unset_constants.begin(), m_unset_constants.end(), &c);
		if( it != m_unset_constants.end())
			m_unset_constants.erase( it);
#endif //DEBUG

	}
}

template < enum_shader_type shader_type >
template < typename T >
inline void constants_handler< shader_type >::set_constant_array(
	shader_constant_host const& c,
	T const* arg,
	u32 array_size
)
{
	// FUNCTION BODY[0x6f680] for vertex shader
	// FUNCTION BODY[0x7ce90] for pixel shader
	COMPILE_ASSERT(	constant_type_traits<T>::value == true, Only_types_described_in_constant_h_are_supported );

	u32 buffer_index;
	if( c.m_update_markers[shader_type] == backend::ref().constant_update_counter()
		&& ( buffer_index = c.shader_slot(shader_type).buffer_index()) != slot_dest_buffer_null)
	{
		// == may be changed to <
		//ASSERT( c.shader_slot(shader_type).array_size() == array_size);
		ASSERT( c.shader_slot(shader_type).array_size() >= array_size);

		ASSERT( c.shader_slot(shader_type).class_id() == constant_type_traits<T>::class_id,"Wrong type! The given object can't be cast to the corresponding shader constant slot.");
		ASSERT( (c.shader_slot(shader_type).class_id() & constant_class_size_mask) == constant_type_traits<T>::size, "The given object has smaller dimension than the corresponding shader constant slot!");

		ASSERT( c.type() == constant_type_traits<T>::type, "The given object dosen't have the same type as the corresponding shader constant slot!");

		m_current->m_const_buffers[buffer_index]->set_typed( c.shader_slot(shader_type), arg, array_size);

#ifdef DEBUG
		render::vector<shader_constant_host*>::iterator it = std::find( m_unset_constants.begin(), m_unset_constants.end(), &c);
		if( it != m_unset_constants.end())
			m_unset_constants.erase( it);
#endif //DEBUG

	}
}

STATIC_SIZE_ASSERT( constants_handler< enum_shader_type_vertex >, 0xC );
STATIC_SIZE_ASSERT( textures_handler< enum_shader_type_vertex >, 0x224 );
STATIC_SIZE_ASSERT( samplers_handler< enum_shader_type_vertex >, 0x50 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_BACKEND_HANDLERS_H_INCLUDED
