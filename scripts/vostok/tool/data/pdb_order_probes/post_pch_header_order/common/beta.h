// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

struct beta_post_pch_type
{
	int member;
};

__declspec(noinline) int beta_post_pch_value( beta_post_pch_type value )
{
	return value.member + 2;
}
