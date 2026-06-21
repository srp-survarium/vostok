# A reachability anchor must CONSTRUCT + CALL with volatile-sourced args - `keep()` keeps the symbol but does NOT pair the body
tags: cpp:member cpp:ctor | topic:anchoring topic:reachability topic:fold-icf
symptoms: carcass TU's functions report `unpaired` (no `fuzzy_match_percent`) even after an `anchor_*` references them; base obj is tiny/missing or shows an ICF-folded empty body; `use_*` anchor absent from the base index (count 0); the anchor's call vanished from anchor_game's instruction list
confidence: 9/10
variants: anchor-sole-caller-convention.md, dced-static-helper-anchor.md, carcass-anchor-must-call-not-declare (CLAUDE.md)

A carcass TU whose functions sit at `unpaired` in report.json is a REACHABILITY gap,
not a matching gap: the function compiled into the `.lib` but `/OPT:REF` stripped it from
the EXE, so the delinker (which reads the EXE, not the lib) never produced a base body to
pair. The fix is an anchor - but the FORM of the anchor decides whether the symbol is
merely KEPT or actually PAIRED, and these are different outcomes:

- **`keep( &C::method )` (address-take to a `volatile` sink) only KEEPS the symbol.** The
  function survives `/OPT:REF`, but the delinked body often pairs as an ICF-folded empty
  thunk (or stays `unpaired`). Evidence: `booby_trap::play_fired_effects`,
  `victory_item::*` - all `keep()`-anchored, all `unpaired`. The address-take is not a
  strong-enough, body-preserving reference.

- **CONSTRUCT the object and CALL its methods - and source EVERY argument through a
  `volatile` pointer - to PAIR it.** The volatile reads are observable side effects the
  optimiser cannot prove away, so it can neither DCE the calls nor fold the bodies, and
  the delinker recovers a real body to pair. This is the pattern that works:

  ```cpp
  void use_game_small_utils( ) {
      static volatile bool s_run = false;
      if ( !s_run ) return;                 // volatile guard: unreachable at runtime,
                                            // not provably-dead at compile time
      static player const* volatile s_player = 0;   // EVERY arg via a volatile ptr
      static game_world*  volatile  s_world  = 0;
      step_manager step;                    // construct -> pins ctor (+ vtable for virtuals)
      step.on_step( *s_player, *s_player_vec, *s_player_vec, *s_world );  // call -> pins body
  }
  ```

THE TRAP that silently fails: passing a LITERAL argument (`swf.get_bind( enum_keyboard(0) )`,
`hands.update_bones_matrices( *s_skel, s_mat, 0, 0, ... )`). The optimiser proves the call
on a fresh local has no escaping effect, DCEs it, then DCEs the now-empty `use_*` function,
then removes anchor_game's call to it - so `use_*` disappears from the base index entirely
(grep count 0) and nothing pairs. The cure is mechanical: route ALL args (including the
`0`/`false`/enum constants) through `static T volatile s_x = 0;` placeholders.

Befriend the anchor where the ctor/methods are protected/private
(`friend void ::vostok::use_*( );`). A class reached for real by another path (e.g.
a registered `*_cook` that placement-news it) does NOT need the anchor for that path - but
it DOES need the `friend` if you made the ctor private to match a `private:` demangled
access (the cook's `new (p) C(...)` breaks otherwise).

Evidence (game small-utils cluster): the `keep()` form left every symbol `unpaired`; the
construct+call+all-volatile form paired them in one rebuild - step_manager ctor 100%,
text_translator ctor/dtor 100%, empty_hands activate 100%, swf process_mouse_move 100%,
register_ctl_bind 99%, initialize 87%.
