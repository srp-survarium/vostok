////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SIMPLE_ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
#define SIMPLE_ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED

/* INCLUDES */
struct survarium::animation_controller_parameters;
typedef vostok::resources::resource_ptr<vostok::animation::animation_expression_emitter,vostok::resources::unmanaged_intrusive_base>
	vostok::animation::animation_expression_emitter_ptr;
class vostok::animation::animation_expression_emitter;

namespace survarium {

struct simple_animation_controller_parameters : public animation_controller_parameters {
	virtual	void	reset	( ) override;

	inline			simple_animation_controller_parameters( ) { /* no source */ }
	inline			~simple_animation_controller_parameters( ) { /* no source */ }

public:
	/* 0x0000 */	/* animation_controller_parameters */
	/* 0x0004 */	animation::animation_expression_emitter_ptr	emitter;
}; // struct simple_animation_controller_parameters

STATIC_SIZE_ASSERT(simple_animation_controller_parameters, 0x8);

} // namespace survarium

#endif // #ifndef SIMPLE_ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
