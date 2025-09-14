////////////////////////////////////////////////////////////////////////////
//	Created		: 29.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "interpolator_size_calculator.h"
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
	m_size						+= sizeof( instant_interpolator );
}

void interpolator_size_calculator::visit	( linear_interpolator const& interpolator )
{
	VOSTOK_UNREFERENCED_PARAMETER	( interpolator );
	m_size						+= sizeof( linear_interpolator );
}

void interpolator_size_calculator::visit	( fermi_interpolator const& interpolator )
{
	VOSTOK_UNREFERENCED_PARAMETER	( interpolator );
	m_size						+= sizeof( fermi_interpolator );
}