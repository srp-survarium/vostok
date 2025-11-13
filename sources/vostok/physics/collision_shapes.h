////////////////////////////////////////////////////////////////////////////
//	Created		: 09.09.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_COLLISION_SHAPES_H_INCLUDED
#define VOSTOK_PHYSICS_COLLISION_SHAPES_H_INCLUDED

#include <vostok/physics/api.h>
#include <vostok/collision/primitives.h>

// #include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

class btCollisionShape;
class btStridingMeshInterface;
class btBvhTriangleMeshShape;

namespace vostok {
namespace physics {

class VOSTOK_PHYSICS_API bt_collision_shape : public resources::unmanaged_resource
{
public:
								bt_collision_shape		( btCollisionShape* sh );
	virtual						~bt_collision_shape		( );

	virtual	u16					get_triangle_material	( s32 triangle_id, bool is_shape_index );

	inline btCollisionShape*	get_bt_shape			( ) const { return m_bt_shape; }
	inline void					get_bounding_sphere		( float3&, float& ) const { ; } // sushi@TODO

private:
	btCollisionShape*			m_bt_shape;
public:
	u16*						m_tri_face_data;
	u16*						m_shapes_face_data;
}; // class collision_shape

STATIC_SIZE_ASSERT(bt_collision_shape, 0x118);


typedef resources::resource_ptr<
	resources::managed_resource,
	resources::managed_intrusive_base
> geometry_resource_ptr;

VOSTOK_PHYSICS_API void						destroy_bt_shape						( btCollisionShape* sh );
VOSTOK_PHYSICS_API void						destroy_shape							( bt_collision_shape* shape );
VOSTOK_PHYSICS_API btCollisionShape*		create_bt_primitive						( collision::primitive_type type, float3 const& dim, float3 const& __formal );
VOSTOK_PHYSICS_API bt_collision_shape*		create_primitive_shape					( collision::primitive_type type, float3 const& dim, float3 const& local_scale );
VOSTOK_PHYSICS_API bt_collision_shape*		create_compound_shape					( configs::binary_config_value const& shapes_root, float3 const& local_scale, pcstr model_path );

VOSTOK_PHYSICS_API btBvhTriangleMeshShape*	create_btBvhTriangleMeshShape			(
																							float3*                            vertices,
																							u32*                               indices,
																							u32                                num_vertices,
																							u32                                num_indices,
																							u16*                               face_data,
																							float3 const&                      local_scale,
																							geometry_resource_ptr const&       vertices_resource,
																							geometry_resource_ptr const&       indices_resource);

VOSTOK_PHYSICS_API bt_collision_shape*		create_static_triangle_mesh_shape		(
																							float3*                            vertices,
																							u32*                               indices,
																							u32                                num_vertices,
																							u32                                num_indices,
																							u16*                               face_data,
																							float3 const&                      local_scale,
																							geometry_resource_ptr const&       vertices_resource,
																							geometry_resource_ptr const&       indices_resource);

typedef	resources::resource_ptr<
	bt_collision_shape,
	resources::unmanaged_intrusive_base
> bt_collision_shape_ptr;

#if 0
// sushi@TODO: Moved to cpp
class btBvhTriangleMeshShapeResource : public btBvhTriangleMeshShape {
public:
	btBvhTriangleMeshShapeResource(
		btStridingMeshInterface*           meshInterface,
		u16*                               face_data,
		geometry_resource_ptr const&       vertices_resource,
		geometry_resource_ptr const&       indices_resource);

private:
	/* offset 0x0000 */ /* fields for btBvhTriangleMeshShape */
	/* offset 0x0060 */ u16*                   m_face_data;
	/* offset 0x0064 */ geometry_resource_ptr  m_raw_vertices;
	/* offset 0x0068 */ geometry_resource_ptr  m_raw_indices;
}; // class btBvhTriangleMeshShapeResource

namespace {
	typedef char size_assert[
		sizeof(btBvhTriangleMeshShapeResource) == 0x70 ? 1 : -1
	];
}
#endif


/*
VOSTOK_PHYSICS_API bt_collision_shape* create_primitive_shape				( memory::base_allocator& allocator, vostok::collision::primitive_type type, float3 const& dim );
VOSTOK_PHYSICS_API bt_collision_shape* create_compound_shape				( memory::base_allocator& allocator, configs::binary_config_value const& config );
VOSTOK_PHYSICS_API bt_collision_shape* create_static_triangle_mesh_shape	( memory::base_allocator& allocator, float3* vertices, u32* indices, u32 num_vertices, u32 num_indices );
VOSTOK_PHYSICS_API bt_collision_shape* create_dynamic_triangle_mesh_shape	( memory::base_allocator& allocator, float3* vertices, u32* indices, u32 num_vertices, u32 num_indices );
VOSTOK_PHYSICS_API bt_collision_shape* create_terrain_shape				( memory::base_allocator& allocator, float* heighfield, u32 rowcol, float min_height, float max_height );
VOSTOK_PHYSICS_API void				 destroy_shape						( memory::base_allocator& allocator, bt_collision_shape* shape );
*/

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_COLLISION_SHAPES_H_INCLUDED