////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
#define ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED

namespace survarium {

struct animation_controller_parameters {
	virtual	void	reset							( ) = 0;

	inline			animation_controller_parameters	( ) { /* no source */ }
}; // struct animation_controller_parameters

STATIC_SIZE_ASSERT(animation_controller_parameters, 0x4);

} // namespace survarium

#endif // #ifndef ANIMATION_CONTROLLER_PARAMETERS_H_INCLUDED
