////////////////////////////////////////////////////////////////////////////
//	Created		: 02.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/fermi_interpolator.h>
#include "interpolator_comparer.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"
#include <vostok/animation/interpolator_visitor.h>

using vostok::animation::fermi_interpolator;
using vostok::animation::interpolator_comparer;
using vostok::animation::interpolator_visitor;
using vostok::animation::instant_interpolator;
using vostok::animation::linear_interpolator;

// claude@NOTE: cross-module LTCG wall. Target ctor is an optimized COMDAT with the
// eax-this frameless convention (this in eax, transition_time in xmm0, epsilon on
// stack, ret 4) reached only via game::legs_ik_processor::set_*_on_ground. Our /Od
// base never emits a standalone ctor (inlined at every site) so report pairs it with
// an ICF fold-rep (spurious call log_format). Structure/init-list already faithful.
fermi_interpolator::fermi_interpolator			( float const transition_time, float const epsilon ) :
	m_total_transition_time	( transition_time ),
	m_epsilon				( epsilon )
{
}

// Target /GL removes the derived locals from PDB and schedules this upstream body
// through direct SSE libm calls; the unoptimized base retains their stack lifetimes.
float fermi_interpolator::interpolated_value			( float current_transition_time ) const
{
	R_ASSERT				( current_transition_time < m_total_transition_time );

	float const t0			= m_total_transition_time/2.f;
	float const length		= 1.f + 2.f*m_epsilon;
	float const d			= t0 / math::log(1.f/m_epsilon + 1.f);
	float const value		= length /( 1.f + math::exp( -(current_transition_time - t0)/d ) );

	return					math::clamp_r( value - m_epsilon, 0.f, 1.f );
}

float fermi_interpolator::transition_time				( ) const
{
	return					m_total_transition_time;
}

fermi_interpolator* fermi_interpolator::clone	( vostok::mutable_buffer& buffer ) const
{
	return					clone_impl< fermi_interpolator >( buffer );
}

fermi_interpolator* fermi_interpolator::clone( vostok::animation::mixing::n_ary_tree_transition_tree_constructor& constructor ) const
{
	return					clone_impl< fermi_interpolator >( constructor.m_buffer );
}

void fermi_interpolator::accept						( interpolator_visitor& visitor ) const
{
	visitor.visit			( *this );
}

void fermi_interpolator::accept						( interpolator_comparer& dispatcher, base_interpolator const& interpolator ) const
{
	interpolator.visit		( dispatcher, *this );
}

void fermi_interpolator::visit						( interpolator_comparer& dispatcher, instant_interpolator const& interpolator ) const
{
	dispatcher.dispatch		( interpolator, *this );
}

void fermi_interpolator::visit						( interpolator_comparer& dispatcher, linear_interpolator const& interpolator ) const
{
	dispatcher.dispatch		( interpolator, *this );
}

void fermi_interpolator::visit						( interpolator_comparer& dispatcher, fermi_interpolator const& interpolator ) const
{
	dispatcher.dispatch		( interpolator, *this );
}
