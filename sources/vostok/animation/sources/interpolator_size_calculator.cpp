////////////////////////////////////////////////////////////////////////////
//	Created		: 29.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "interpolator_size_calculator.h"
#include "mixing_n_ary_tree_comparer.h"
#include <vostok/animation/instant_interpolator.h>
#include <vostok/animation/linear_interpolator.h>
#include <vostok/animation/fermi_interpolator.h>

using vostok::animation::interpolator_size_calculator;
using vostok::animation::instant_interpolator;
using vostok::animation::linear_interpolator;
using vostok::animation::fermi_interpolator;

void interpolator_size_calculator::visit	( instant_interpolator const& interpolator )
{
	VOSTOK_UNREFERENCED_PARAMETER	( interpolator );

	if ( m_comparer )		m_comparer->advance_buffer		( sizeof( instant_interpolator ) );
	else
		m_size							+= sizeof( instant_interpolator );
}

void interpolator_size_calculator::visit	( linear_interpolator const& interpolator )
{
	VOSTOK_UNREFERENCED_PARAMETER	( interpolator );

	if ( m_comparer )		m_comparer->advance_buffer		( sizeof( linear_interpolator ) );
	else
		m_size							+= sizeof( linear_interpolator );
}

void interpolator_size_calculator::visit	( fermi_interpolator const& interpolator )
{
	VOSTOK_UNREFERENCED_PARAMETER	( interpolator );

	if ( m_comparer )		m_comparer->advance_buffer		( sizeof( fermi_interpolator ) );
	else
		m_size							+= sizeof( fermi_interpolator );
}
