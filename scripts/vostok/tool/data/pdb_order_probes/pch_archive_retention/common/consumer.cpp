// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"

int consumer_function()
{
    pch_payload payload = {5};
    return pch_inline_function(payload.value);
}
