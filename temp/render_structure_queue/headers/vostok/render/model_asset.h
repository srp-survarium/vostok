////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODEL_ASSET_H_INCLUDED
#define RENDER_MODEL_ASSET_H_INCLUDED

/* INCLUDES */
class vostok::fs_new::virtual_path_string;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::configs::binary_config;
class vostok::resources::managed_resource;
class vostok::resources::unmanaged_resource;

namespace vostok {
namespace render {

struct model_asset {
	inline		~model_asset( ) { /* no source */ }
	inline		model_asset	( ) { /* no source */ }

	/* 0x0000 */	resources::managed_resource_ptr			converted_model_buffer;
	/* 0x0004 */	resources::unmanaged_resource_ptr		material;
	/* 0x0008 */	configs::binary_config_ptr				export_properties_config;
	/* 0x000c */	fs_new::virtual_path_string				m_surface_name;
}; // struct model_asset

STATIC_SIZE_ASSERT(model_asset, 0x120);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MODEL_ASSET_H_INCLUDED
