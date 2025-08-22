////////////////////////////////////////////////////////////////////////////
//	Created		: 13.10.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/animation/base_interpolator.h>
#include "interpolator_comparer.h"

using vostok::animation::base_interpolator;

vostok::animation::comparison_result_enum vostok::animation::compare	(
		base_interpolator const& left,
		base_interpolator const& right
	)
{
	vostok::animation::interpolator_comparer	comparer;
	left.accept								( comparer, right );
	return									comparer.result;
}