// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

struct pch_payload
{
    int value;
};

__declspec(dllexport) inline int pch_inline_function(int value)
{
    return value + 4;
}
