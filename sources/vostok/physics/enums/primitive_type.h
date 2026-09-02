// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.08.2026
////////////////////////////////////////////////////////////////////////////
#ifndef PHYSICS_PRIMITIVE_TYPE_H_INCLUDED
#define PHYSICS_PRIMITIVE_TYPE_H_INCLUDED

namespace vostok {
namespace physics {

enum primitive_type
{
	primitive_sphere				= 0x00,
	primitive_box					= 0x01,
	primitive_cylinder				= 0x02,
	primitive_capsule				= 0x03,
	primitive_truncated_sphere		= 0x04,
	primitive_unknown				= 0x63,
};

} // namespace physics
} // namespace vostok

#endif // #ifndef PHYSICS_PRIMITIVE_TYPE_H_INCLUDED
