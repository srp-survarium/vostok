// SPDX-License-Identifier: GPL-3.0-or-later

int consumer_function();

extern "C" void entry()
{
    volatile int value = consumer_function();
    (void)value;
}
