// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

struct beta_header_type
{
	int member;
};

__declspec(noinline) int beta_header_value( beta_header_type value )
{
	return value.member + 2;
}
