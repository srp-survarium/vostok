////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_ANIMATION_CONTROLLER_H_INCLUDED
#define BASE_ANIMATION_CONTROLLER_H_INCLUDED

/* INCLUDES */
class vostok::animation::mixing::expression;
class vostok::render::base_scene;

/* FORWARD REFS */
class vostok::render::game::renderer;
class survarium::animation_controller_parameters;

namespace survarium {

struct base_animation_controller : public boost::noncopyable {
	virtual	void								initialize					( ) = 0;

	virtual	animation::mixing::expression		try_finalize				( base_animation_controller& arg_0, mutable_buffer& arg_1 ) = 0;

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& arg_0 ) = 0;

	virtual	void								set_target					( animation_controller_parameters const& arg_0 ) = 0;

	virtual	void								query_new_target_if_needed	( ) = 0;

	virtual	void								debug_draw					( render::game::renderer& arg_0, render::base_scene_ptr const& arg_1 ) const = 0;

	virtual										~base_animation_controller	( ) = 0;
	inline										base_animation_controller	( ) { /* no source */ }
}; // struct base_animation_controller

STATIC_SIZE_ASSERT(base_animation_controller, 0x4);

} // namespace survarium

#endif // #ifndef BASE_ANIMATION_CONTROLLER_H_INCLUDED
