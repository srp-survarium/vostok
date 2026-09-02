// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/physics/collision_shapes.h>

#include "bullet_include.h"
#include <vostok/physics/bullet_utils.h>

namespace vostok {
namespace physics {

class btBvhTriangleMeshShapeResource : public btBvhTriangleMeshShape {
public:
			btBvhTriangleMeshShapeResource	(
				btStridingMeshInterface*			meshInterface,
				u16*								face_data,
				geometry_resource_ptr const&		vertices_resource,
				geometry_resource_ptr const&		indices_resource
			);

	/* 0x0000 */	/* btBvhTriangleMeshShape */
	/* 0x0060 */	u16*					m_face_data;
	/* 0x0064 */	geometry_resource_ptr	m_raw_vertices;
	/* 0x0068 */	geometry_resource_ptr	m_raw_indices;
}; // class btBvhTriangleMeshShapeResource

STATIC_SIZE_ASSERT(btBvhTriangleMeshShapeResource, 0x70);


btBvhTriangleMeshShapeResource::btBvhTriangleMeshShapeResource(
	btStridingMeshInterface*			meshInterface,
	u16*								face_data,
	geometry_resource_ptr const&		vertices_resource,
	geometry_resource_ptr const&		indices_resource
) :
	btBvhTriangleMeshShape	( meshInterface, true, false ),
	m_face_data				( face_data ),
	m_raw_vertices			( vertices_resource ),
	m_raw_indices			( indices_resource )
{
}

bt_collision_shape::bt_collision_shape( btCollisionShape* sh ):
	m_tri_face_data		( NULL ),
	m_shapes_face_data	( NULL ),
	m_bt_shape			( sh )
{
	sh->setUserPointer( this );
}

bt_collision_shape::~bt_collision_shape( )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_shapes_face_data );
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_tri_face_data );
	destroy_bt_shape( m_bt_shape );
}

u16 bt_collision_shape::get_triangle_material( const s32 triangle_id, const bool is_shape_index )
{
	if ( is_shape_index )
	{
		return m_shapes_face_data[triangle_id];
	}
	else
	{
		return m_tri_face_data[triangle_id];
	}
}

void destroy_bt_shape( btCollisionShape* sh )
{
	s32 shape_type = sh->getShapeType();
	if ( shape_type == COMPOUND_SHAPE_PROXYTYPE )
	{
		btCompoundShape* shape = (btCompoundShape*)sh;
		while ( shape->getNumChildShapes( ) )
		{
			btCollisionShape* child = shape->getChildList( )->m_childShape;
			shape->removeChildShapeByIndex( 0 );
			destroy_bt_shape( child );
		}
	}
	else if ( shape_type == TRIANGLE_MESH_SHAPE_PROXYTYPE )
	{
		btTriangleMeshShape* shape = (btTriangleMeshShape*)sh;
		btStridingMeshInterface* mesh = shape->getMeshInterface( );
		VOSTOK_DELETE_IMPL( g_ph_allocator, mesh );
	}

	VOSTOK_DELETE_IMPL( g_ph_allocator, sh );
}

void destroy_shape( bt_collision_shape* shape )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, shape );
}

btCollisionShape* create_bt_primitive( collision::primitive_type type, float3 const& dim, float3 const& __formal )
{
	btCollisionShape*	result = NULL;
	switch ( type )
	{
	case collision::primitive_sphere :
		{
			btScalar radius = dim.x;
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btSphereShape )( radius );
		}break;
	case collision::primitive_box :
		{
			btVector3 half_extents( dim.x, dim.y, dim.z );
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btBoxShape )( half_extents );
		}break;
	case collision::primitive_cylinder :
		{
			btVector3 half_extents( dim.x, dim.y, dim.z );
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btCylinderShape )( half_extents );
		}break;
	case collision::primitive_capsule :
		{
			btScalar radius = dim.x;
			btScalar height = dim.y;
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btCapsuleShape )( radius, height );
		}break;
	}
	return result;
}

bt_collision_shape* create_primitive_shape( collision::primitive_type type, float3 const& dim, float3 const& local_scale )
{
	btCollisionShape* bt_shape = create_bt_primitive( type, dim, local_scale );

	bt_shape->setLocalScaling( from_vostok_pos( local_scale ) );

	bt_collision_shape* result = VOSTOK_NEW_IMPL( g_ph_allocator, bt_collision_shape )( bt_shape );

	result->m_shapes_face_data = VOSTOK_NEW_IMPL( g_ph_allocator, u16 );
	*result->m_shapes_face_data = 0;
	return result;
}

bt_collision_shape* create_compound_shape( configs::binary_config_value const& shapes_root, float3 const& local_scale, pcstr model_path )
{
	configs::binary_config_value::const_iterator it		= shapes_root.begin();
	configs::binary_config_value::const_iterator it_e	= shapes_root.end();

	btCompoundShape* bt_shape							= VOSTOK_NEW_IMPL( g_ph_allocator, btCompoundShape )( );
	u32				 bt_shapes_len						= shapes_root.size( ) * sizeof( u16 );

	u16*			 face_data							= (u16*)VOSTOK_MALLOC_IMPL( g_ph_allocator, bt_shapes_len, "face_data" );
	memset( face_data, 0, bt_shapes_len );


	for ( u16* face_data_it = face_data ; it != it_e ; ++it )
	{
		collision::primitive_type type		= (collision::primitive_type)(int)(*it)["type"];
		float3 position						= (*it)["position"];
		float3 rotation						= (*it)["rotation"];
		float3 dim							= (*it)["scale"];

		*face_data_it = it->value_exists("mtl") ? (u16)(*it)["mtl"] : 0;
		btCollisionShape* child_shape = create_bt_primitive( type, dim, dim );

		float4x4 child_transform			= create_rotation(rotation) * create_translation(position);

		btTransform child_local_transform	= from_vostok( child_transform );
		bt_shape->addChildShape				( child_local_transform, child_shape );
		++face_data_it;
	}

	bt_shape->setLocalScaling( from_vostok_pos( local_scale ) );
	bt_collision_shape* result		= VOSTOK_NEW_IMPL( g_ph_allocator, bt_collision_shape )( bt_shape );

	result->m_shapes_face_data = face_data;

	return result;
}

btBvhTriangleMeshShape* create_btBvhTriangleMeshShape(
	float3*                            vertices,
	u32*                               indices,
	u32                                num_vertices,
	u32                                num_indices,
	u16*                               face_data,
	float3 const&                      local_scale,
	geometry_resource_ptr const&       vertices_resource,
	geometry_resource_ptr const&       indices_resource)
{
	btIndexedMesh mesh;
	mesh.m_triangleIndexBase		= (u8*)indices;
	mesh.m_numTriangles				= num_indices/3;
	mesh.m_numVertices				= num_vertices;
	mesh.m_triangleIndexStride		= sizeof(int)*3;
	mesh.m_vertexStride				= sizeof(float3);
	mesh.m_vertexBase				= (u8*)vertices;
	mesh.m_vertexType				= PHY_FLOAT;
	mesh.m_indexType				= PHY_INTEGER;

	btTriangleIndexVertexArray* mesh_interface	= VOSTOK_NEW_IMPL( g_ph_allocator, btTriangleIndexVertexArray );
	mesh_interface->setScaling					( from_vostok( local_scale ) );
	mesh_interface->addIndexedMesh				( mesh );
	btBvhTriangleMeshShape* result				= VOSTOK_NEW_IMPL( g_ph_allocator, btBvhTriangleMeshShapeResource )( mesh_interface, face_data, vertices_resource, indices_resource );

	if ( !result->getOptimizedBvh( ) )
		result->buildOptimizedBvh( );

	return result;
}

bt_collision_shape* create_static_triangle_mesh_shape(
	float3*                            vertices,
	u32*                               indices,
	u32                                num_vertices,
	u32                                num_indices,
	u16*                               face_data,
	float3 const&                      local_scale,
	geometry_resource_ptr const&       vertices_resource,
	geometry_resource_ptr const&       indices_resource)
{
	btCollisionShape* bt_shape	= create_btBvhTriangleMeshShape( vertices, indices, num_vertices, num_indices, face_data, local_scale, vertices_resource, indices_resource );

	bt_collision_shape* shape			= VOSTOK_NEW_IMPL( g_ph_allocator, bt_collision_shape )( bt_shape );
	return shape;
}

} // namespace physics
} // namespace vostok
