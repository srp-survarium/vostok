// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BASE_ANIMATION_CONTROLLER_H_INCLUDED
#define BASE_ANIMATION_CONTROLLER_H_INCLUDED

// PDB spells the debug_draw param vostok::render::base_scene_ptr - the same
// resource_ptr type our render tree typedefs as scene_ptr
#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace animation {
namespace mixing {
	class expression;
} // namespace mixing
} // namespace animation
namespace render {
namespace game {
	class renderer;
} // namespace game
} // namespace render
} // namespace vostok

namespace survarium {

struct animation_controller_parameters;

struct base_animation_controller : private boost::noncopyable {
	virtual	void								initialize					( ) = 0;

	virtual	animation::mixing::expression		try_finalize				( base_animation_controller& arg_0, mutable_buffer& arg_1 ) = 0;

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& arg_0 ) = 0;

	virtual	void								set_target					( animation_controller_parameters const& arg_0 ) = 0;

	virtual	void								query_new_target_if_needed	( ) = 0;

	virtual	void								debug_draw					( render::game::renderer& arg_0, render::scene_ptr const& arg_1 ) const = 0;

protected:
	// PDB: pure virtual dtor; the macro carries the inline body derived dtors chain to
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR		( base_animation_controller );

public:
}; // struct base_animation_controller

STATIC_SIZE_ASSERT(base_animation_controller, 0x4);

} // namespace survarium

#endif // #ifndef BASE_ANIMATION_CONTROLLER_H_INCLUDED
