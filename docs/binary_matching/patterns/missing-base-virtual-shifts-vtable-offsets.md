# A missing virtual in the BASE class shifts EVERY later vtable-offset by 4

## Symptom
Many functions across the module (and its CONSUMERS) sit at 99.x%, each off by a
single mis-resolved vtable call: base emits `mov eax,[edx+0x24]; call eax` where the
target emits `mov eax,[edx+0x28]; call eax` (a constant `+4` / one-slot delta on the
SAME virtual method). A whole-engine sweep shows dozens of `engine_world::*`,
`get_exit_code`, `initialize_core`, etc. all at 99.9x for the same reason. The
offending call is almost always to a method that, in your header, is declared EARLIER
than it sits in the target's vtable.

## Cause
Your base-class declaration is missing a virtual method that the original had. Vtable
slots are assigned in declaration order, so dropping one virtual pulls every later
slot DOWN by one (4 bytes on Win32). Every `[vptr+offset]` call past the gap is wrong
by `4 * (number of missing virtuals before it)`.

In `vostok::debug::engine` the header lacked `virtual bool output_to_debugger() const`
between `terminate_on_error` and `on_terminate`. So `on_terminate` (and everything
after) was one slot early: `terminate_impl` called it at `[edx+8]` instead of
`[edx+0Ch]`, `current_directory` at `[edx+24h]` instead of `[edx+28h]`, and so on.

## Fix
1. Pull the target's class layout from `binaries/structure/target/headers/...` and
   diff the virtual list against your header (the structure header lists the FULL
   ordered set, including methods with `/* no source */` bodies).
2. Insert the missing pure virtual at the right position in the base class.
3. Add the `override` to EVERY concrete subclass (grep `: public Base`), even ones in
   OTHER modules — otherwise the class stays abstract and `new Subclass` won't compile.
   The override's POSITION in the derived class is irrelevant (slot is fixed by the
   base); give it the body the target shows (`core_debug_engine::output_to_debugger`
   was `mov al,1; ret` = `return true;`).

One header edit recovers the whole cascade at once: in the debug run this lifted
`debug::initialize` 37.5->100, `terminate_impl`/`terminate` to 100, `save_minidump`'s
`current_directory` call, and ~10 engine-module callers from 99.9x to 100.

## Tell-tale
The delta is a CONSTANT `+4*n` on vtable calls and NOTHING else moves (frame, regs,
statement order all identical). If only ONE function is off, suspect a wrong slot
INDEX in that call; if MANY are off by the same constant, suspect a missing/extra
virtual in the shared base header.
