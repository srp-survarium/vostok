////////////////////////////////////////////////////////////////////////////
//	Created		: 25.11.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SIMPLE_ANIMATION_CONTROLLER_H_INCLUDED
#define SIMPLE_ANIMATION_CONTROLLER_H_INCLUDED

#include "base_animation_controller.h"
#include "animation_controller_parameters.h"

namespace survarium {

class simple_animation_controller : public base_animation_controller
{
public:
								simple_animation_controller			( );
								~simple_animation_controller		( );

	virtual	void								initialize			( );
// 	virtual	vostok::animation::mixing::expression	finalize			( base_animation_controller& next_controller );
// 	virtual vostok::animation::mixing::expression	selected_animations	( );
	virtual	void								set_target			( animation_controller_parameters const& target );

private:
	simple_animation_controller_parameters		m_current_parameters;
	simple_animation_controller_parameters		m_target_parameters;
}; // class simple_animation_controller

} // namespace survarium

#endif // #ifndef SIMPLE_ANIMATION_CONTROLLER_H_INCLUDED