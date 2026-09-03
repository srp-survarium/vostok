// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// checksum content marker: baseline
struct checksum_content_value
{
	int member;
};

__declspec(noinline) int checksum_header_value( checksum_content_value value )
{
	return value.member + 1;
}
