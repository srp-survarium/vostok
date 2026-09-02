// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

enum preincluded_kind
{
	preincluded_alpha,
	preincluded_beta,
};

struct preincluded_type
{
	int member;
	preincluded_kind kind;
};
