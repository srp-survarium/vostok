// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/physics/static_rigid_body.h>

namespace vostok {
namespace physics {

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
