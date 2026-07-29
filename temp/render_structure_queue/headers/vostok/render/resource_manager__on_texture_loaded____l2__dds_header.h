////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RESOURCE_MANAGER_ON_TEXTURE_LOADED___L2_DDS_HEADER_H_INCLUDED
#define RENDER_RESOURCE_MANAGER_ON_TEXTURE_LOADED___L2_DDS_HEADER_H_INCLUDED

/* INCLUDES */
struct vostok::render::_DDSURFACEDESC2;

namespace vostok {
namespace render {

struct resource_manager::on_texture_loaded::__l2::dds_header {

	/* 0x0000 */	u32					signature0;
	/* 0x0004 */	_DDSURFACEDESC2		header0;
}; // struct resource_manager::on_texture_loaded::__l2::dds_header

STATIC_SIZE_ASSERT(resource_manager::on_texture_loaded::__l2::dds_header, 0x80);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RESOURCE_MANAGER_ON_TEXTURE_LOADED___L2_DDS_HEADER_H_INCLUDED
