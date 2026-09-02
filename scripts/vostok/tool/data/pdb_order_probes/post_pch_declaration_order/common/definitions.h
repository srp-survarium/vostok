// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include "macros.h"

__declspec(noinline) int definition_value( int value )
{
	return value + PROBE_BIAS;
}
