////////////////////////////////////////////////////////////////////////////
//	Created		: 29.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef INTERPOLATOR_VISITOR_H_INCLUDED
#define INTERPOLATOR_VISITOR_H_INCLUDED

namespace vostok {
namespace animation {

class instant_interpolator;
class linear_interpolator;
class fermi_interpolator;

struct VOSTOK_ANIMATION_API VOSTOK_NOVTABLE interpolator_visitor {
	virtual	void	visit	( instant_interpolator const& interpolator ) = 0;
	virtual	void	visit	( linear_interpolator const& interpolator ) = 0;
	virtual	void	visit	( fermi_interpolator const& interpolator ) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( interpolator_visitor )
}; // class interpolator_visitor

} // namespace animation
} // namespace vostok

#endif // #ifndef INTERPOLATOR_VISITOR_H_INCLUDED