// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"

int pch_preinclude_anchor( preincluded_type value )
{
	return value.member + value.kind;
}
