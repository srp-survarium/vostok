////////////////////////////////////////////////////////////////////////////
//	Created		: 22.11.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_ANIMATION_CONTROLLER_H_INCLUDED
#define BASE_ANIMATION_CONTROLLER_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {
	class expression;
} // namespace mixing
} // namespace animation
} // namespace vostok

namespace survarium {

struct animation_controller_parameters;

struct VOSTOK_NOVTABLE base_animation_controller : private boost::noncopyable
{
public:
	virtual	void								initialize			( )	= 0;
// 	virtual	vostok::animation::mixing::expression	finalize			( base_animation_controller& next_controller )	= 0;
// 	virtual vostok::animation::mixing::expression	selected_animations	( )	= 0;
	virtual	void								set_target			( animation_controller_parameters const& target ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR							( base_animation_controller );
}; // struct base_animation_controller

} // namespace survarium

#endif // #ifndef BASE_ANIMATION_CONTROLLER_H_INCLUDED