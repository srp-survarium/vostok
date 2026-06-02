////////////////////////////////////////////////////////////////////////////
//	Created		: 02.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef FERMI_INTERPOLATOR_H_INCLUDED
#define FERMI_INTERPOLATOR_H_INCLUDED

#include <vostok/animation/base_interpolator.h>

namespace vostok {
namespace animation {

class VOSTOK_ANIMATION_API fermi_interpolator : public base_interpolator {
public:
	explicit		fermi_interpolator	( float const transition_time, float const epsilon = .005f );
	virtual	float	interpolated_value	( float current_transition_time ) const;
	virtual	float	transition_time		( ) const;
	inline	float	epsilon				( ) const { return m_epsilon; }

	virtual	fermi_interpolator*	clone	( mixing::n_ary_tree_transition_tree_constructor& constructor ) const;
	virtual	fermi_interpolator* clone	( mutable_buffer& buffer ) const;

	virtual void	accept				( interpolator_visitor& visitor ) const;

	virtual void	accept				( interpolator_comparer& dispatcher, base_interpolator const& interpolator ) const;
	virtual void	visit				( interpolator_comparer& dispatcher, instant_interpolator const& interpolator ) const;
	virtual void	visit				( interpolator_comparer& dispatcher, linear_interpolator const& interpolator ) const;
	virtual void	visit				( interpolator_comparer& dispatcher, fermi_interpolator const& interpolator ) const;

	// claude@MATCH: members are non-const and operator= is the compiler-implicit
	// memberwise one (per PDB ground-truth structure header) - legs_ik_processor::
	// set_{heel,toe}_on_ground assigns `m_*_interpolator = fermi_interpolator(...)`,
	// emitting a 2-float copy (+4/+8) that skips the vtable. `float const` members
	// or a private operator= (the prior reconstruction) make that assignment illegal.
private:
	float			m_total_transition_time;
	float			m_epsilon;
}; // class fermi_interpolator

STATIC_SIZE_ASSERT(fermi_interpolator, 0xC);

} // namespace animation
} // namespace vostok

#endif // #ifndef FERMI_INTERPOLATOR_H_INCLUDED