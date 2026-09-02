// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

enum declaration_kind
{
	declaration_alpha,
	declaration_beta,
};

struct declaration_value
{
	int member;
	declaration_kind kind;
};
