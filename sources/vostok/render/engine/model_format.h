#ifndef VOSTOK_RENDER_ENGINE_MODEL_FORMAT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MODEL_FORMAT_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_sphere.h>

namespace vostok {
namespace render {

enum platform_id_enum {
	platform_uni			= 0x0,
	platform_windows_dx11	= 0x1,
	platform_xbox360		= 0x2,
	platform_ps3			= 0x3
};

enum model_chunk_enum {
	model_chunk_header					= 0x01,
	model_chunk_vertices				= 0x03,
	model_chunk_indices					= 0x04,
	model_chunk_collision_v				= 0x19,
	model_chunk_collision_i				= 0x1a,
	model_chunk_collision_face_data_hdr	= 0x1b,
	model_chunk_collision_face_data		= 0x1c,
	model_chunk_terrain_data				= 0x66,
	model_chunk_terrain_data_compressed1	= 0x67,
	model_anim_bones						= 0x65,
	model_texture						= 0x02,
	model_children						= 0x09
};

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

class base_basis {
public:
	u8 x;
	u8 y;
	u8 z;

	void set( float3 n )
	{
		n.normalize_safe( n );
		n += 1.0f;
		n *= 0.5f * 255.f;
		s32 nx = math::floor( n.x ); math::clamp( nx, 0, 255 ); x = u8( nx );
		s32 ny = math::floor( n.y ); math::clamp( ny, 0, 255 ); y = u8( ny );
		s32 nz = math::floor( n.z ); math::clamp( nz, 0, 255 ); z = u8( nz );
	}
};

STATIC_SIZE_ASSERT( base_basis, 0x3 );

#pragma pack( push, 1 )

struct model_header {
	model_header( ) { }

	u8			platform_id;
	u8			type;
	u16		version;
	math::aabb	bb;
	math::sphere	bs;
};

struct vert_boned_1w {
	void invalidate( )
	{
		P.set( 0.0f, 0.0f, 0.0f );
		N.set( 0.0f, 1.0f, 0.0f );
		T.set( 1.0f, 0.0f, 0.0f );
		B.set( 1.0f, 0.0f, 1.0f );
		uv.set( 0.0f, 0.0f );
		m = 0;
	}

	float3	P;
	float3	N;
	float3	T;
	float3	B;
	float2	uv;
	u16		m;
};

struct vert_boned_2w {
	void invalidate( )
	{
		P.set( 0.0f, 0.0f, 0.0f );
		N.set( 0.0f, 1.0f, 0.0f );
		T.set( 1.0f, 0.0f, 0.0f );
		B.set( 1.0f, 0.0f, 1.0f );
		uv.set( 0.0f, 0.0f );
		m[0] = 0;
		m[1] = 0;
		w = 0.0f;
	}

	u16		m[2];
	float3	P;
	float3	N;
	float3	T;
	float3	B;
	float	w;
	float2	uv;
};

struct vert_boned_3w {
	void invalidate( )
	{
		P.set( 0.0f, 0.0f, 0.0f );
		N.set( 0.0f, 1.0f, 0.0f );
		T.set( 1.0f, 0.0f, 0.0f );
		B.set( 1.0f, 0.0f, 1.0f );
		uv.set( 0.0f, 0.0f );
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		w[0] = 0.0f;
		w[1] = 0.0f;
	}

	u16		m[3];
	float3	P;
	float3	N;
	float3	T;
	float3	B;
	float	w[2];
	float2	uv;
};

struct vert_boned_4w {
	void invalidate( )
	{
		P.set( 0.0f, 0.0f, 0.0f );
		N.set( 0.0f, 1.0f, 0.0f );
		T.set( 1.0f, 0.0f, 0.0f );
		B.set( 1.0f, 0.0f, 1.0f );
		uv.set( 0.0f, 0.0f );
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
		w[0] = 0.0f;
		w[1] = 0.0f;
		w[2] = 0.0f;
	}

	u16		m[4];
	float3	P;
	float3	N;
	float3	T;
	float3	B;
	float	w[3];
	float2	uv;
};

struct hardware_1weights_skinning_vertex {
	float3	P;
	u8		indices[4];
	u32		N;
	u32		T;
	u32		B;
	float2	uv;
};

struct hardware_2weights_skinning_vertex {
	float3	P;
	u8		indices[4];
	float	weights;
	u32		N;
	u32		T;
	u32		B;
	float2	uv;
};

struct hardware_3weights_skinning_vertex {
	float3	P;
	u8		indices[4];
	float	weights[2];
	u32		N;
	u32		T;
	u32		B;
	float2	uv;
};

struct hardware_4weights_skinning_vertex {
	float3	P;
	u8		indices[4];
	float	weights[3];
	u32		N;
	u32		T;
	u32		B;
	float2	uv;
};

#pragma pack( pop )

STATIC_SIZE_ASSERT( model_header, 0x2C );
STATIC_SIZE_ASSERT( vert_boned_1w, 0x3A );
STATIC_SIZE_ASSERT( vert_boned_2w, 0x40 );
STATIC_SIZE_ASSERT( vert_boned_3w, 0x46 );
STATIC_SIZE_ASSERT( vert_boned_4w, 0x4C );
STATIC_SIZE_ASSERT( hardware_1weights_skinning_vertex, 0x24 );
STATIC_SIZE_ASSERT( hardware_2weights_skinning_vertex, 0x28 );
STATIC_SIZE_ASSERT( hardware_3weights_skinning_vertex, 0x2C );
STATIC_SIZE_ASSERT( hardware_4weights_skinning_vertex, 0x30 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MODEL_FORMAT_H_INCLUDED
