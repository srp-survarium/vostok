////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_DATA_RESOURCE_H_INCLUDED
#define RENDER_TEXTURE_DATA_RESOURCE_H_INCLUDED

/* INCLUDES */
class vostok::const_buffer;

namespace vostok {
namespace render {

struct texture_data_resource {
	explicit		texture_data_resource	( u32 buffer_storage_size );

	const_buffer	buffer					( ) const;

private:
	/* 0x0000 */	u32		m_buffer_size;
}; // struct texture_data_resource

STATIC_SIZE_ASSERT(texture_data_resource, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_DATA_RESOURCE_H_INCLUDED
