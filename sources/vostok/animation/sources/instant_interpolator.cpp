////////////////////////////////////////////////////////////////////////////
//	Created		: 19.02.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/instant_interpolator.h>
#include "interpolator_comparer.h"
#include "mixing_n_ary_tree_transition_tree_constructor.h"
#include <vostok/animation/interpolator_visitor.h>

using vostok::animation::instant_interpolator;
using vostok::animation::interpolator_comparer;
using vostok::animation::interpolator_visitor;
using vostok::animation::linear_interpolator;
using vostok::animation::fermi_interpolator;

float instant_interpolator::interpolated_value		( float current_transition_time ) const
{
	VOSTOK_UNREFERENCED_PARAMETER	( current_transition_time );
	return				1.f;
}

float instant_interpolator::transition_time			( ) const
{
	return				0.f;
}

instant_interpolator* instant_interpolator::clone	( vostok::mutable_buffer& buffer ) const
{
	return				clone_impl< instant_interpolator >( buffer );
}
// STATE[STUB]
instant_interpolator* instant_interpolator::clone( vostok::animation::mixing::n_ary_tree_transition_tree_constructor& constructor ) const
{
	return NULL;

	// FUNCTION BODY
	// <0x56f8c0>|0x000|+0x020:'37'
	// <0x56f8e0>|0x020|-0x003:'37'
	// <0x56f8dd>|0x01d|+0x00f:'38'
	// ******
}

void instant_interpolator::accept					( interpolator_visitor& visitor ) const
{
	visitor.visit		( *this );
}

void instant_interpolator::accept					( interpolator_comparer& dispatcher, base_interpolator const& interpolator ) const
{
	interpolator.visit	( dispatcher, *this );
}

void instant_interpolator::visit					( interpolator_comparer& dispatcher, instant_interpolator const& interpolator ) const
{
	dispatcher.dispatch	( interpolator, *this );
}

void instant_interpolator::visit					( interpolator_comparer& dispatcher, linear_interpolator const& interpolator ) const
{
	dispatcher.dispatch	( interpolator, *this );
}

void instant_interpolator::visit					( interpolator_comparer& dispatcher, fermi_interpolator const& interpolator ) const
{
	dispatcher.dispatch	( interpolator, *this );
}