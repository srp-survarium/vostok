// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_COOK_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_TEXTURE_COOK_H_INCLUDED
#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {

struct texture_data_resource {
	explicit texture_data_resource( u32 buffer_storage_size );
	const_buffer buffer( ) const;

private:
	u32 m_buffer_size;
};

struct texture_cook : public resources::inplace_managed_cook {
	texture_cook( );

	virtual u32 calculate_resource_size(
		u32 file_size,
		u32& offset_to_file,
		bool file_exist
	);
	virtual void create_resource(
		resources::query_result_for_cook& query,
		resources::managed_resource_ptr resource,
		u32 raw_file_size,
		u32& final_resource_size
	);
	virtual void destroy_resource( resources::managed_resource* resource );
};

STATIC_SIZE_ASSERT( texture_data_resource, 0x4 );
STATIC_SIZE_ASSERT( texture_cook, 0x20 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_TEXTURE_COOK_H_INCLUDED
