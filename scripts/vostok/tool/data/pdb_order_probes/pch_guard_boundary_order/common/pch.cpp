// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"

int guard_boundary_anchor( guard_boundary_shared_type value )
{
	return value.member + 1;
}
