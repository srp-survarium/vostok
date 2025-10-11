////////////////////////////////////////////////////////////////////////////
//	Created		: 18.04.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_MODEL_H_INCLUDED
#define VOSTOK_PHYSICS_MODEL_H_INCLUDED

#include <vostok/animation/skeleton.h>

namespace vostok {

namespace physics {


struct animated_model_instance : resources::unmanaged_resource {
	/* offset 0x0000 */ /* fields for resources::unmanaged_resource */
	/* offset 0x0108 */ animation::skeleton_ptr m_skeleton;
}; // struct animated_model_instance

namespace {
	typedef char size_assert[
		sizeof(animated_model_instance) == 0x110 ? 1 : -1
	];
}




typedef	resources::resource_ptr<
			animated_model_instance,
			resources::unmanaged_intrusive_base >
		animated_model_instance_ptr;


} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_MODEL_H_INCLUDED