// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

struct alpha_post_pch_type
{
	int member;
};

__declspec(noinline) int alpha_post_pch_value( alpha_post_pch_type value )
{
	return value.member + 1;
}
