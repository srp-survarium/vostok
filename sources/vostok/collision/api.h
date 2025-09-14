////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_COLLISION_API_H_INCLUDED
#define VOSTOK_COLLISION_API_H_INCLUDED

#ifndef VOSTOK_COLLISION_API
#	ifdef VOSTOK_STATIC_LIBRARIES
#		define VOSTOK_COLLISION_API
#	else // #ifdef VOSTOK_STATIC_LIBRARIES
#		ifdef VOSTOK_COLLISION_BUILDING
#			define VOSTOK_COLLISION_API				VOSTOK_DLL_EXPORT
#		else // #ifdef VOSTOK_COLLISION_BUILDING
#			ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_COLLISION_API			VOSTOK_DLL_IMPORT
#			else // #ifndef VOSTOK_ENGINE_BUILDING
#				define VOSTOK_COLLISION_API			VOSTOK_DLL_EXPORT
#			endif // #ifndef VOSTOK_ENGINE_BUILDING
#		endif // #ifdef VOSTOK_COLLISION_BUILDING
#	endif // #ifdef VOSTOK_STATIC_LIBRARIES
#endif // #ifndef VOSTOK_COLLISION_API

#include <vostok/collision/common_types.h>			// for object_type
#include <vostok/animation/skeleton.h>

namespace vostok {
namespace collision {

struct space_partitioning_tree;
class geometry_instance;
class geometry;
class object;
class bone_collision_data;
class animated_object;
class triangle_mesh_geometry;

VOSTOK_COLLISION_API	void initialize				( );

VOSTOK_COLLISION_API	non_null<space_partitioning_tree>::ptr new_space_partitioning_tree (
												memory::base_allocator* allocator,
												float min_aabb_radius,
												u32 reserve_node_count
											);
VOSTOK_COLLISION_API	void					delete_space_partitioning_tree	( space_partitioning_tree* tree );


VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr new_box_geometry_instance		(
												memory::base_allocator* allocator,
												float4x4 const& matrix
											);

VOSTOK_COLLISION_API	u32						get_box_geometry_instance_size			( );

VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr new_sphere_geometry_instance	(
												memory::base_allocator* allocator,
												float4x4 const& matrix,
												float radius
											);

VOSTOK_COLLISION_API	u32						get_sphere_geometry_instance_size		( );


VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr new_cylinder_geometry_instance	(
												memory::base_allocator* allocator,
												float4x4 const& matrix,
												float radius, 
												float half_length
											);

VOSTOK_COLLISION_API	u32						get_cylinder_geometry_instance_size		( );

VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr	new_capsule_geometry_instance	(
															memory::base_allocator* allocator,
															float4x4 const& matrix,
															float radius,
															float half_length
														);
VOSTOK_COLLISION_API	u32						get_capsule_geometry_instance_size		( );

VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr	new_truncated_sphere_geometry_instance(
															memory::base_allocator* allocator,
															float4x4 const& matrix,
															float radius,
															float4* planes,
															u32 planes_count
														);

VOSTOK_COLLISION_API	u32						get_truncated_sphere_geometry_instance_size( );


VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr new_terrain_geometry_instance	( 
												memory::base_allocator* allocator,
												float4x4 const& matrix,
												float physical_size,
												u32 vertex_row_size,
												const float* heightfield,
												bool delete_by_collision_object = true
											);

VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr	new_composite_geometry_instance	(
															memory::base_allocator* allocator,
															float4x4 const& matrix,
															non_null<geometry const>::ptr comp_geom
														);

VOSTOK_COLLISION_API	non_null<geometry_instance>::ptr	new_triangle_mesh_geometry_instance	(
															memory::base_allocator* allocator,
															float4x4 const& matrix,
															non_null<geometry const>::ptr geometry
														);

VOSTOK_COLLISION_API	u32						get_geometry_instance_size	( );

VOSTOK_COLLISION_API	void					delete_geometry_instance	( 
												memory::base_allocator* allocator,	
												geometry_instance* object
											);

VOSTOK_COLLISION_API	non_null<geometry>::ptr		new_composite_geometry				(
													memory::base_allocator* allocator,
													buffer_vector<geometry_instance*> const& instances
												);


VOSTOK_COLLISION_API	non_null<geometry>::ptr		new_triangle_mesh_geometry		(
													memory::base_allocator* allocator,
													math::float3 const* vertices,
													u32 vertex_count,
													u32 const* indices,
													u32 index_count,
													u32 const* triangle_data,
													u32 triangle_count
												);

VOSTOK_COLLISION_API	non_null<geometry>::ptr		new_triangle_mesh_geometry		(
													memory::base_allocator* allocator, 
													math::float3 const* vertices,
													u32 vertex_count,
													u32 const* indices,
													u32 index_count
												);



VOSTOK_COLLISION_API	non_null<geometry>::ptr		new_composite_geometry		(
													memory::base_allocator* allocator,
													resources::managed_resource_ptr resource
												);

VOSTOK_COLLISION_API	u32						get_composite_geometry_size	( );



VOSTOK_COLLISION_API	void					delete_geometry				(
												memory::base_allocator* allocator,
												geometry* geometry
											);

VOSTOK_COLLISION_API	non_null<object>::ptr	new_collision_object		(
												memory::base_allocator* allocator,
												object_type object_type,
												non_null<geometry_instance>::ptr geometry,
												pvoid user_data = 0
											);

VOSTOK_COLLISION_API	non_null<object>::ptr	new_aabb_object				(
												memory::base_allocator* allocator,
												collision::object_type object_type,
												math::float3 const& position,
												math::float3 const& epsilon = math::float3( math::epsilon_5, math::epsilon_5, math::epsilon_5 ),
												pvoid user_data = 0
											);

VOSTOK_COLLISION_API	void					delete_object				(
												memory::base_allocator* allocator,
												object* object
											);


VOSTOK_COLLISION_API	animated_object*		new_animated_object	(
												configs::binary_config_value const& config,
												animation::skeleton_ptr const& model_skeleton,
												memory::base_allocator* allocator
											);
VOSTOK_COLLISION_API	void					delete_animated_object	(
												animated_object* object_to_be_deleted,
												memory::base_allocator* allocator
											);
#ifdef MASTER_GOLD
VOSTOK_COLLISION_API	bool					contains	(
												geometry_instance const&	bounding_volume,
												geometry_instance const&	testee
											);

#else
VOSTOK_COLLISION_API	bool					contains	(
												geometry_instance const&	bounding_volume,
												geometry_instance const&	testee,
												render::debug::renderer*	m_renderer,
												render::scene_ptr const*	m_scene
											);
#endif //#ifndef MASTER_GOLD

#ifdef MASTER_GOLD
VOSTOK_COLLISION_API	bool					intersects	(
												geometry_instance const&	bounding_volume,
												geometry_instance const&	testee
											);

#else
VOSTOK_COLLISION_API	bool					intersects	(
												geometry_instance const&	bounding_volume,
												geometry_instance const&	testee,
												render::debug::renderer*	m_renderer,
												render::scene_ptr const*	m_scene
											);
#endif //#ifndef MASTER_GOLD

} // namespace collision
} // namespace vostok

#endif // #ifndef VOSTOK_COLLISION_API_H_INCLUDED
