// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

struct alpha_header_type
{
	int member;
};

__declspec(noinline) int alpha_header_value( alpha_header_type value )
{
	return value.member + 1;
}
