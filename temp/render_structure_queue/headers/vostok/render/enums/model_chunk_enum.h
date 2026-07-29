////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MODEL_CHUNK_ENUM_H_INCLUDED
#define RENDER_MODEL_CHUNK_ENUM_H_INCLUDED

namespace vostok {
namespace render {

enum model_chunk_enum
{
	model_chunk_header						= 0x01,
	model_chunk_vertices					= 0x03,
	model_chunk_indices						= 0x04,
	model_chunk_collision_v					= 0x19,
	model_chunk_collision_i					= 0x1a,
	model_chunk_collision_face_data_hdr		= 0x1b,
	model_chunk_collision_face_data			= 0x1c,
	model_chunk_terrain_data				= 0x66,
	model_chunk_terrain_data_compressed1	= 0x67,
	model_anim_bones						= 0x65,
	model_texture							= 0x02,
	model_children							= 0x09,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MODEL_CHUNK_ENUM_H_INCLUDED
