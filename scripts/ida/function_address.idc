// SPDX-License-Identifier: GPL-3.0-or-later

#include <idc.idc>

//
// Don't run this script multiple times!
// We do not check for whether slot was already inserted or not.
// And if it is, it will be inserted one more time, which will confuse IDA.
//

static mark_breakpoints(fn_start_addr)
{
  // sushi@TODO: Shouldn't be hardcoded, but there is no way to access environment.
  // I tried `call_system` with echo, which is slow and also doesn't work.
  auto root_path = "E:\\Projects";
  auto temp_result = ".tmp.res1";

  call_system("python " + root_path + "\\vostok\\scripts\\find_breakpoints.py"
    + " " + form("0x%x", fn_start_addr)
    + " > " + temp_result
  );

  auto handle = fopen(temp_result, "r");

  auto breakpoints_len = long(readstr(handle));
  msg("Found %d breakpoints\n", breakpoints_len);

  auto slot;
  for ( slot = 0; slot != 1024 && breakpoints_len != 0; slot++ ) {
    // Should be '!= -1', but I by mistake filled my database with zeros
    if ( get_bookmark(slot) > 0 ) {
      continue;
    }

    auto breakpoint_ea = xtol(readstr(handle));
    msg("Breakpoint '0x%x'\n", breakpoint_ea);
    put_bookmark(breakpoint_ea, -1, -1, -1, slot, "Auto-generated <" + form("0x%x", breakpoint_ea) + ">");
    breakpoints_len--;
  }
  fclose(handle);

  if ( slot == 1024 ) {
    msg("Out of bookmark slots!");
    throw "Out of bookmark slots!";
  }
}

static main()
{
  auto ea = get_screen_ea();
  auto fn_start_addr = get_func_attr(ea, FUNCATTR_START);

  msg("Function starts at: '0x%x'\n", fn_start_addr);
  mark_breakpoints(fn_start_addr);
}
