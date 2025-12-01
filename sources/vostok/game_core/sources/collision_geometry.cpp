////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/collision_geometry.h>

#include <vostok/physics/ghost_object.h>

namespace survarium {

// STATE[83.12%|DONE]
collision_geometry::collision_geometry( ) :
	m_physics_world		( NULL ),
	m_ghost_object		( NULL ),
	m_group				( 0 ),
	m_mask				( 0 )
{
}

// STATE[99.74%|DONE]
collision_geometry::~collision_geometry( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	destroy_ghost_object( );

	// FUNCTION BODY
	// <0x6ff192>|0x000|0x000:'25'
	// <0x6ff19e>|0x00c|0x00c:'26'
	// <0x6ff1aa>|0x018|0x00c:'27'
	// ******
}

// STATE[100%|DONE]
void collision_geometry::destroy_ghost_object( )
{
	vostok::physics::destroy_ghost_object( m_ghost_object );

	// FUNCTION BODY
	// <0x6ff167>|0x000|0x000:'32'
	// ******
}

// STATE[82.66%|DONE]
void collision_geometry::load( vostok::configs::binary_config_value const& cfg_val )
{
	// LOCALS
	// vostok::physics::bt_collision_shape_ptr shape
	// ******

	m_name					= cfg_val["full_name"];
	float3 const& scale		= cfg_val["scale"];			// sushi@NOTE: Can reuse `read_transform` in base_project.cpp
	float3 const& rotation	= cfg_val["rotation"];
	float3 const& position	= cfg_val["position"];
	float4x4 transform = create_scale( scale ) * create_rotation( rotation ) * create_translation( position );

	if ( !cfg_val.value_exists( "meshes" ) )
	{
		LOG_WARNING( "invalid collision_geometry" );
		return;
	}
	

	m_mode											= (collision_geometry::collision_geometry_mode)(u32)cfg_val["mode"];
	vostok::configs::binary_config_value meshes		= cfg_val["meshes"];
	vostok::physics::bt_collision_shape_ptr shape	= vostok::physics::create_compound_shape( meshes, float3( 1.0f, 1.0f, 1.0f ), m_name.c_str( ) );
	m_ghost_object									= vostok::physics::create_ghost_object( shape, transform );
	shape->set_no_delete( );	// sushi@NOTE: I don't know why this is used

	m_group											= cfg_val["filter_group"];
	m_mask											= cfg_val["filter_mask"];

	// FUNCTION BODY
	// <0x6feefb>|0x000|0x000:'37'
	// <0x6fef16>|0x01b|0x01b:'38'
	// <0x6fef2b>|0x030|0x015:'39'
	// <0x6fef40>|0x045|0x015:'40'
	// <0x6fef55>|0x05a|0x015:'41'
	// <1>
	// <0x6fefa8>|0x0ad|0x053:'43'	if ( cfg_val.value_exists( "meshes" )
	// <1>
	// <0x6fefc0>|0x0c5|0x018:'45'
	// <0x6ff049>|0x14e|0x089:'46'
	// <1>
	// <2>
	// <0x6ff04e>|0x153|0x005:'49'	m_mode
	// <0x6ff06c>|0x171|0x01e:'50'	vostok::configs::binary_config_value meshes		= cfg_val["meshes"]
	// <0x6ff09c>|0x1a1|0x030:'51'	vostok::physics::bt_collision_shape_ptr shape	= 
	// <0x6ff0dc>|0x1e1|0x040:'52'	m_ghost_object									= vostok::physics::create_ghost_object( shape, transform );
	// <0x6ff100>|0x205|0x024:'53'
	// <1>
	// <0x6ff10f>|0x214|0x00f:'55'
	// <0x6ff12e>|0x233|0x01f:'56'
	// ******
}

// STATE[100%|DONE]
u32 collision_geometry::get_overlapping_objects_count( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	return m_ghost_object->get_overlapping_objects_count( );

	// FUNCTION BODY
	// <0x6fece9>|0x000|0x000:'61'
	// <0x6fecf5>|0x00c|0x00c:'62'
	// ******
}

// STATE[74.69%|DONE]
void collision_geometry::get_overlapping_objects(vostok::physics::base_physics_objects_type& result ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->get_overlapping_objects( result );

	// FUNCTION BODY
	// <0x6fecb9>|0x000|0x000:'67'
	// <0x6fecc5>|0x00c|0x00c:'68'
	// ******
}

// STATE[100%|DONE]
void collision_geometry::contact_test( vostok::physics::base_physics_object* object, vostok::physics::contact_test_predicate& predicate )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->contact_test( m_physics_world, object, predicate );

	// FUNCTION BODY
	// <0x6fee19>|0x000|0x000:'73'
	// <0x6fee25>|0x00c|0x00c:'74'
	// ******
}

// STATE[100%|DONE]
bool collision_geometry::contact_test( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	return m_ghost_object->contact_test( m_physics_world );

	// FUNCTION BODY
	// <0x6fec89>|0x000|0x000:'79'
	// <0x6fec95>|0x00c|0x00c:'80'
	// ******
}

// STATE[93.33%|DONE]
void collision_geometry::get_shapes_centers( vostok::vectora<float3>& centers_results ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->non_compound_shapes_centers( centers_results );

	// FUNCTION BODY
	// <0x6fee89>|0x000|0x000:'91'
	// <0x6fee95>|0x00c|0x00c:'92'
	// ******
}

// STATE[100%|DONE]
void collision_geometry::insert( vostok::physics::world* world )
{
	m_physics_world = world;
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );
	m_ghost_object->user_data = this;
	m_ghost_object->insert( m_physics_world, m_group, m_mask );

	// FUNCTION BODY
	// <0x6feb89>|0x000|0x000:'97'
	// <0x6feb92>|0x009|0x009:'98'
	// <0x6feb9e>|0x015|0x00c:'99'
	// <0x6febad>|0x024|0x00f:'100'
	// ******
}

// STATE[100%|DONE]
void collision_geometry::remove( )
{
	ASSERT( UNKNOWN_EXPRESSION_T( m_ghost_object ) );	// sushi@NOTE: Might also be m_physics_world check
	m_ghost_object->remove( m_physics_world );			// sushi@NOTE: user_data is not unset
	m_physics_world = NULL;

	// FUNCTION BODY
	// <0x6feb49>|0x000|0x000:'105'
	// <0x6feb55>|0x00c|0x00c:'106'
	// <0x6feb6a>|0x021|0x015:'107'
	// ******
}

// STATE[100%|DONE]
void collision_geometry::subscribe( vostok::physics::world* world, collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	
	if ( m_subscribers.empty( ) )
		insert( world );

	m_subscribers.push_back( subscriber );

	// FUNCTION BODY
	// <0x6fedb9>|0x000|0x000:'112'
	// <1>
	// <0x6fedc5>|0x00c|0x00c:'114'
	// <0x6fede5>|0x02c|0x020:'115'
	// <1>
	// <0x6fedf1>|0x038|0x00c:'117'
	// ******
}

// STATE[91.57%|DONE] m_end is wrapped into an empty call
void collision_geometry::unsubscribe( collision_geometry_subscriber* subscriber )
{
	ASSERT( UNKNOWN_EXPRESSION );
	m_subscribers.erase( std::find( m_subscribers.begin( ), m_subscribers.end( ), subscriber ) );

	if ( m_subscribers.empty( ) )
		remove( );

	// FUNCTION BODY
	// <0x6febe9>|0x000|0x000:'122'
	// <0x6febf5>|0x00c|0x00c:'123'
	// <1>
	// <0x6fec4a>|0x061|0x055:'125'
	// <0x6fec6a>|0x081|0x020:'126'
	// ******
}

// STATE[100%|DONE]
void collision_geometry::set_transform( float4x4 const& transform )
{
	m_ghost_object->set_transform( transform );

	// FUNCTION BODY
	// <0x6feeb8>|0x000|0x000:'132'
	// ******
}

// STATE[100%|DONE]
float4x4 collision_geometry::get_transform( )
{
	return m_ghost_object->get_transform( );

	// FUNCTION BODY
	// <0x6fee57>|0x000|0x000:'136'
	// ******
}

} // namespace survarium
