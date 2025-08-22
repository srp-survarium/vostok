////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.02.2008
//	Author		: Konstantin Slipchenko
//	Description : collision element interface
////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "element.h"

#include <vostok/collision/geometry_instance.h>
#include <vostok/collision/contact_info.h>
#include <vostok/collision/primitives.h>

#include "generate_contact.h"
#include "body.h"


element::element( body& b, vostok::collision::geometry_instance& cf ):
m_body			( b ),
collide_element	( cf )
{
	float4x4 m;
	m_pose_in_body =  cf.get_matrix() * math::invert4x3( b.get_pose(m) );
}

element::~element( )
{
}

void element::interpolate_world_transform( float factor, float4x4 &transform ) const
{  
	float4x4 body_pose;
	m_body.interpolate_pose( factor, body_pose );
	transform = m_pose_in_body * body_pose;
}

bool	element::collide( island& i, const collide_element_interface& o, bool reverted_order )const
{
	VOSTOK_UNREFERENCED_PARAMETERS( reverted_order );
	return o.collide( i, m_geometry_instance, m_body );
}

bool	element::collide( island& i, const vostok::collision::geometry_instance& og, body& body )const
{
	//contact_info_buffer	buffer;

	on_two_body_contact gc(  m_body, body,  i );

	generate_contacts( gc, og );//normal og->m_geometry_instance

	return gc.number != 0;
}

bool	element::collide( island& i, const vostok::collision::geometry_instance& og, bool reverted_order )const
{
	VOSTOK_UNREFERENCED_PARAMETERS( reverted_order );

	on_one_body_contact gc(  m_body, i );

	generate_contacts( gc, og );

	return gc.number != 0;
}

bool	element::integrate( const step_counter_ref& sr, float time_delta )
{
	bool is_awake = m_body.integrate( sr, time_delta );
	float4x4 m;
	m_geometry_instance.set_matrix( m_pose_in_body * m_body.get_pose( m ) );
	return is_awake;
}

void		element::on_step_start	( island	&i, float time_delta )
{
	m_body.on_step_start( i ,time_delta );
}

void	element::fill_island( island &i )
{
	m_body.fill_island( i );
}

struct primitive_sum_volume: public vostok::collision::enumerate_primitives_callback
{
	primitive_sum_volume() : result( 0 ){}

	virtual void enumerate( vostok::math::float4x4 const&, vostok::collision::primitive const & p )
	{
		switch( p.type )
		{
			
			case vostok::collision::primitive_sphere: {
					float r = p.sphere().radius;
					result += vostok::math::pi * r * r * r * 4.f / 3.f;
				} break;

			case vostok::collision::primitive_box: {

					float3 sides( p.box().half_side * 2.f );	
					result += sides.x * sides.y * sides.z;
				} break;

			case vostok::collision::primitive_cylinder: {
					float r = p.cylinder().radius;
					float l = p.cylinder().half_length * 2.f;
					result += math::pi * r * r * l;
				} break;

			default:
				NODEFAULT();
		}
	}
	 
	float result;
};

void element::set_mass( float mass )
{
	
	VOSTOK_UNREFERENCED_PARAMETER(mass);

	#pragma message ( VOSTOK_TODO( "move volume definition to collision" ) )
	#pragma message ( VOSTOK_TODO( "take into account primitives intersections (may be random points approach)" ) )
	primitive_sum_volume v;
	m_geometry_instance.enumerate_primitives( m_pose_in_body, v );

	if( v.result < vostok::math::epsilon_7 )
		return;

	float density = mass/v.result;
	m_body.set_mass( density, m_geometry_instance, m_pose_in_body );
}

void	element::render(  render::scene_ptr const& scene, render::debug::renderer& renderer  ) const
{
	ASSERT ( get_body() );
	collide_element::render( scene, renderer );
	get_body()->render( scene, renderer );
}