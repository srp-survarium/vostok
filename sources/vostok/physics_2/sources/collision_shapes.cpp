////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics_2/collision_shapes.h>

#include "bullet_include.h"
#include <vostok/physics_2/bullet_utils.h>

namespace vostok {
namespace physics_2 {
/*
// STATE[STUB]
// vostok::physics::btBvhTriangleMeshShapeResource::btBvhTriangleMeshShapeResource(btStridingMeshInterface*, unsigned short*, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&)
btBvhTriangleMeshShapeResource::btBvhTriangleMeshShapeResource(
	btStridingMeshInterface*           meshInterface,
	u16*                               face_data,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& vertices_resource,
	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const& indices_resource)
{
}
*/
// STATE[STUB]
bt_collision_shape::bt_collision_shape( btCollisionShape* sh ):
	m_tri_face_data		( NULL ),
	m_shapes_face_data	( NULL ),
	m_bt_shape			( sh )
{
	sh->setUserPointer( this ); // <0x72c4e3>|0x000|0x000:'48'
}

// STATE[STUB]
bt_collision_shape::~bt_collision_shape( )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_shapes_face_data );	// <0x72c459>|0x000|0x000:'53'
	VOSTOK_DELETE_IMPL( g_ph_allocator, m_tri_face_data );		// <0x72c47b>|0x022|0x022:'54'
	destroy_bt_shape( m_bt_shape );								// <0x72c49d>|0x044|0x022:'55'
}

// STATE[STUB]
u16 bt_collision_shape::get_triangle_material( s32 triangle_id, bool is_shape_index )
{
	if ( is_shape_index )						// <0x72c230>|0x000|0x000:'60'
	{
		// sushi@NOTE: ASSERT?
		return m_shapes_face_data[triangle_id];	// <0x72c237>|0x007|0x007:'63'
	}
	else
	{
		return m_tri_face_data[triangle_id];	// <0x72c248>|0x018|0x011:'67'
	}
}

// STATE[STUB]: sushi@NOTE: There are allocations behind bt_shape. Not sure whether done by survarium or bt. Needs to be investigated first.
void destroy_bt_shape( btCollisionShape* sh )
{
	s32 shape_type = sh->getShapeType();

	if ( shape_type == 31 ) // sushi@TODO: What is 31?
	{
		
		// sh->

	}
	else if ( shape_type == 21 )
	{

	}


	// FUNCTION BODY
	// <0x72c3a9>|0x000|0x000:'92'



	// <0x72c3b5>|0x00c|0x00c:'96'

	// <0x72c3c0>|0x017|0x00b:'98'
	// <0x72c3c6>|0x01d|0x006:'99'
	// <0x72c3cd>|0x024|0x007:'100'

	// <0x72c3dc>|0x033|0x00f:'102'
	// <0x72c3de>|0x035|0x002:'103'


	// <0x72c3e3>|0x03a|0x005:'106'
	// <0x72c3e6>|0x03d|0x003:'107'


	// <0x72c415>|0x06c|0x02f:'110'
	// ******
}

// STATE[STUB]
void destroy_shape( bt_collision_shape* shape )
{
	VOSTOK_DELETE_IMPL( g_ph_allocator, shape ); // <0x72c201>|0x000|0x000:'115'
}

// STATE[STUB]
btCollisionShape* create_bt_primitive( collision::primitive_type type, float3 const& dim, float3 const& __formal )
{
	btCollisionShape*	result = NULL;	// <0x72c269>|0x000|0x000:'120'
	switch ( type )						// <0x72c26c>|0x003|0x003:'121'
	{
	case collision::primitive_sphere :
		{
			btScalar radius = dim.x; 	// <0x72c27c>|0x013|0x010:'125'

			result = VOSTOK_NEW_IMPL( g_ph_allocator, btSphereShape )( radius ); // <0x72c280>|0x017|0x004:'127'
			// ???
			// ???
			// ???
			// ???
		}break;
	case collision::primitive_box :
		{
			btVector3 half_extents( dim.x, dim.y, dim.z );							// <0x72c2af>|0x046|0x02f:'135'
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btBoxShape )( half_extents );	// <0x72c2c9>|0x060|0x01a:'136'
			// ???
			// ???
			// ???
			// ???
		}break;
	case collision::primitive_cylinder :
		{
			btVector3 half_extents( dim.x, dim.y, dim.z );									// <0x72c2fd>|0x094|0x034:'144'
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btCylinderShape )( half_extents );	// <0x72c317>|0x0ae|0x01a:'145'
			// ???
			// ???
			// ???
			// ???
		}break;
	case collision::primitive_capsule :
		{
			btScalar radius = dim.x;														// <0x72c347>|0x0de|0x030:'153'
			btScalar height = dim.y;														// <0x72c351>|0x0e8|0x00a:'154'
			result = VOSTOK_NEW_IMPL( g_ph_allocator, btCapsuleShape )( radius, height );	// <0x72c356>|0x0ed|0x005:'155'
			// ???
			// ???
			// ???
			// ???
		}break;
	}
	return result;
}

// STATE[STUB]
bt_collision_shape* create_primitive_shape( collision::primitive_type type, float3 const& dim, float3 const& local_scale )
{
	btCollisionShape* bt_shape = create_bt_primitive( type, dim, local_scale );							// <0x72ca3e>|0x000|0x000:'167'

	bt_shape->setLocalScaling( btVector3( local_scale.x, local_scale.y, local_scale.z ) );				// <0x72ca43>|0x005|0x005:'169' // sushi@NOTE: from_vostok, but without `local_scale.z` reversed, maybe from_vostok was changed?
	
	bt_collision_shape* result = VOSTOK_NEW_IMPL( g_ph_allocator, bt_collision_shape )( bt_shape );		// <0x72ca7c>|0x03e|0x039:'171'

	// ASSERT? How exactly `m_shapes_face_data` is set?
	result->m_shapes_face_data = VOSTOK_NEW_IMPL( g_ph_allocator, u16 );								// <0x72cac4>|0x086|0x048:'174'
	*result->m_shapes_face_data = 0;																	// <0x72cad9>|0x09b|0x015:'175'
	return result;																						
}

// STATE[STUB]
bt_collision_shape* create_compound_shape( configs::binary_config_value const& shapes_root, float3 const& local_scale, pcstr model_path )
{
	configs::binary_config_value::const_iterator it		= shapes_root.begin();
	configs::binary_config_value::const_iterator it_e	= shapes_root.end();										// <0x72c501>|0x000|0x000:'182'

	btCompoundShape* bt_shape							= VOSTOK_NEW_IMPL( g_ph_allocator, btCompoundShape )( );	// <0x72c511>|0x010|0x010:'184'
	u32				 bt_shapes_len						= sizeof(u16) * 24 * shapes_root.count / 24;				// <0x72c539>|0x038|0x028:'185': sushi@NOTE: No idea why length is calculated like this. Happens in many places.

	u16*			 face_data							= (u16*)VOSTOK_MALLOC_IMPL( g_ph_allocator, bt_shapes_len, "face_data" );	// <0x72c54d>|0x04c|0x014:'187'
	memset( face_data, 0, bt_shapes_len );																							// <0x72c568>|0x067|0x01b:'188'


	for ( u16* face_data_it = face_data ; it != it_e ; ++it ) // <0x72c57a>|0x079|0x012:'191'
	{
		collision::primitive_type type		= (collision::primitive_type)(int)(*it)["type"];
		float3 position						= (*it)["position"];
		float3 rotation						= (*it)["rotation"];
		float3 dim							= (*it)["scale"];

		*face_data_it = it->value_exists("mtl") ? (*it)["mtl"] : 0;				// <0x72c62f>|0x12e|0x0b5:'198'
		btCollisionShape* child_shape = create_bt_primitive( type, dim, dim );	// <0x72c638>|0x137|0x009:'199': sushi@NOTE: Note sure the last argument is needed.
	


		float4x4 child_transform			= create_rotation(rotation) * create_translation(position);	// <0x72c646>|0x145|0x00e:'201'


















		btTransform child_local_transform	= from_vostok( child_transform );		// <0x72c584>|0x083|-0x0c2:'220'
		bt_shape->addChildShape				( child_local_transform, child_shape );	// <0x72c7f3>|0x2f2|0x26f:'221'
		++face_data_it;																// <0x72c949>|0x448|0x156:'222'
	}

	bt_shape->setLocalScaling( from_vostok( local_scale ) );																// <0x72c95f>|0x45e|0x016:'225'
	bt_collision_shape* result		= VOSTOK_NEW_IMPL( g_ph_allocator, bt_collision_shape )( bt_shape );	// <0x72c99d>|0x49c|0x03e:'226'

	result->m_shapes_face_data = face_data;																	// <0x72ca0f>|0x50e|0x072:'228'

	return result;																							// <0x72ca15>|0x514|0x006:'230'
}

// STATE[STUB]
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
	return NULL;

	// LOCALS
	// btIndexedMesh                   mesh
	// ******

	// FUNCTION BODY

	// <0x72cb5d>|0x000|0x000:'243'
	// <0x72cb65>|0x008|0x008:'244'
	// <0x72cb6e>|0x011|0x009:'245'

	// <0x72cb77>|0x01a|0x009:'247'



	// <0x72cb7e>|0x021|0x007:'251'
	// <0x72cbe7>|0x08a|0x069:'252'
	// <0x72cc19>|0x0bc|0x032:'253'
	// <0x72cc36>|0x0d9|0x01d:'254'

	// <0x72cc62>|0x105|0x02c:'256'
	// <0x72cc67>|0x10a|0x005:'257'


	// ******
}

// STATE[STUB]
// vostok::physics::bt_collision_shape* vostok::physics::create_static_triangle_mesh_shape(vostok::math::float3*, unsigned int*, unsigned int, unsigned int, unsigned short*, vostok::math::float3 const&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&)
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
	return NULL;

	// FUNCTION BODY







	// <0x72cc81>|0x000|0x000:'278'

	// <0x72cca6>|0x025|0x025:'280'
	// <0x72ccef>|0x06e|0x049:'281'
	// ******
}

} // namespace physics
} // namespace vostok