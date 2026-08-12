#include "pch.h"
#include <vostok/render/core/texture_cook.h>

namespace vostok {
namespace render {

struct texture_raw_file_data : public resources::unmanaged_resource {
	explicit texture_raw_file_data( mutable_buffer outer_buffer )
		: m_buffer( outer_buffer )
	{
	}

	virtual ~texture_raw_file_data( )
	{
	}

	mutable_buffer const& buffer( ) const
	{
		return m_buffer;
	}

	mutable_buffer m_buffer;
};

texture_data_resource::texture_data_resource( u32 const buffer_storage_size ) :
	m_buffer_size		( buffer_storage_size )
{
	// FUNCTION BODY[0x559460]
}

const_buffer texture_data_resource::buffer( ) const
{
	// FUNCTION BODY[0x559490]
	return				const_buffer( (pbyte)this + sizeof(texture_data_resource), m_buffer_size );
}

texture_cook::texture_cook( )
	: resources::inplace_managed_cook(
		resources::texture_class,
		reuse_false,
		use_resource_manager_thread_id
	)
{
	// FUNCTION BODY[0x5594b0]
}

u32 texture_cook::calculate_resource_size( u32 file_size, u32& out_offset_to_file, bool file_exist )
{
	// FUNCTION BODY[0x559470]
	R_ASSERT_U								( file_exist );
	u32 const class_size					= sizeof( texture_data_resource );
	out_offset_to_file						= class_size;
	u32 const full_size						= class_size + file_size;
	return									full_size;
}

void texture_cook::create_resource(
	resources::query_result_for_cook& in_out_query,
	resources::managed_resource_ptr in_out_resource,
	u32 raw_file_size,
	u32& out_final_resource_size
)
{
	// FUNCTION BODY[0x559520]
	resources::pinned_ptr_mutable< texture_data_resource > managed_typed_ptr	= pin_for_write< texture_data_resource >( in_out_resource );
	new ( managed_typed_ptr.c_ptr() ) texture_data_resource ( raw_file_size );
	out_final_resource_size					= sizeof( texture_data_resource ) + raw_file_size;
	in_out_query.finish_query				( result_success );
}

void texture_cook::destroy_resource( resources::managed_resource* const dying_resource )
{
	// FUNCTION BODY[0x5594f0]
	resources::pinned_ptr_mutable< texture_data_resource > managed_typed_ptr	= pin_for_write< texture_data_resource >( dying_resource );
	managed_typed_ptr.c_ptr()->~texture_data_resource	( );
}

} // namespace render
} // namespace vostok
