////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MOVEMENT_ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
#define MOVEMENT_ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED

/* INCLUDES */
struct survarium::animation_controller_parameters;
typedef vostok::resources::resource_ptr<vostok::animation::animation_expression_emitter,vostok::resources::unmanaged_intrusive_base>
	vostok::animation::animation_expression_emitter_ptr;
class vostok::animation::animation_expression_emitter;

namespace survarium {

struct movement_animation_controller_parameters : public animation_controller_parameters {
	virtual	void	reset	( ) override;

	inline			movement_animation_controller_parameters( ) { /* no source */ }
	inline			~movement_animation_controller_parameters( ) { /* no source */ }

public:
	/* 0x0000 */	/* animation_controller_parameters */
	/* 0x0004 */	float3		position;
	/* 0x0010 */	float3		eyes_direction;
	/* 0x001c */	float3		velocity;
	/* 0x0028 */	animation::animation_expression_emitter_ptr	animation;
}; // struct movement_animation_controller_parameters

STATIC_SIZE_ASSERT(movement_animation_controller_parameters, 0x2C);

} // namespace survarium

#endif // #ifndef MOVEMENT_ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
