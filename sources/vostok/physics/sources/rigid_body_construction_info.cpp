////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/static_rigid_body.h>

namespace vostok {
namespace physics {

// claude@NOTE: STRUCTURE MATCH (init-list only, 0 stmts). Residual is non-steerable:
// the target is an optimized-COMDAT (this-in-eax, frameless) that inlines the
// m_collisionShape( NULL ) resource_ptr ctor to a plain zero store, where our /Od base
// emits a resource_ptr::resource_ptr call + uses a framed esi convention.
bt_rigid_body_construction_info::bt_rigid_body_construction_info( ) :
	m_mass						( 0.0f ),
	m_collisionShape			( NULL ),
	m_linearDamping				( 0.0f ),
	m_angularDamping			( 0.0f ),
	m_restitution				( 0.0f ),
	m_linearSleepingThreshold	( 0.8f ),
	m_angularSleepingThreshold	( 1.0f ),
	m_additionalDampingFactor	( 0.005f ),
	m_friction					( 0.5f ),
	m_additionalDamping			( false ),

	m_additionalLinearDampingThresholdSqr	( 0.01f ),
	m_additionalAngularDampingThresholdSqr  ( 0.01f ),
	m_additionalAngularDampingFactor		( 0.01f )
{
}

} // namespace physics
} // namespace vostok