#ifndef VOSTOK_RENDER_ENGINE_MODEL_FORMAT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MODEL_FORMAT_H_INCLUDED

namespace vostok {
namespace render {

enum mesh_type_enum {
	mt_static_submesh			= 0x00,
	mt_static_mesh				= 0x01,
	mt_static_submesh_colored	= 0x02,
	mt_skinned_mesh				= 0x28,
	mt_skinned_submesh_1w		= 0x2c,
	mt_skinned_submesh_2w		= 0x2d,
	mt_skinned_submesh_3w		= 0x2e,
	mt_skinned_submesh_4w		= 0x2f,
	mt_user_mesh_editable		= 0x64,
	mt_user_mesh_wire			= 0x65,
	mt_grass_mesh				= 0xc8
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MODEL_FORMAT_H_INCLUDED
