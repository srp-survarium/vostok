////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_RAW_FILE_DATA_H_INCLUDED
#define RENDER_TEXTURE_RAW_FILE_DATA_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct texture_raw_file_data : public resources::unmanaged_resource {
	inline	explicit					texture_raw_file_data	( mutable_buffer arg_0 ) { /* no source */ }
	virtual								~texture_raw_file_data	( ) { /* no source */ }

	inline	mutable_buffer const&		buffer					( ) const { /* no source */ }

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	mutable_buffer		m_buffer;
}; // struct texture_raw_file_data

STATIC_SIZE_ASSERT(texture_raw_file_data, 0x110);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_RAW_FILE_DATA_H_INCLUDED
