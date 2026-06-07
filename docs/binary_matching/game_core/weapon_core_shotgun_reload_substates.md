# weapon_core_shotgun_reload_{start,one_round,finish}_substate - matching log

Unit: the three STUB shotgun-reload substate classes. Worktree
/home/sheep/Projects/surv/vostok_6 ; branch off origin/int/game_core.

## Target RVAs (binaries/rich/target/index.jsonl)
start:     ctor 0x58e620  initialize 0x58e4e0  finalize 0x58e4a0  is_ready 0x58e420  on_animation_end 0x58e440
one_round: ctor 0x58e270  initialize 0x58e340  finalize 0x58e230  is_ready 0x58e220  on_animation_end 0x58e2d0
finish:    ctor 0x58e1b0  initialize 0x58e0d0  finalize 0x58e030  is_ready 0x58dfb0  on_animation_end 0x58dfc0

## Access chars (mangled)
all three: initialize/finalize MAE (protected virtual), is_ready_for_transition EBE
(private const virtual), on_animation_end AAE (private), ctor QAE (public).
Headers changed to protected: initialize/finalize ; private: is_ready/on_animation_end.

## Ctor literals (decoded from pushed args, base ctor order)
playback_enum: play_cyclically=0, freeze=1, remove=2.
- start:     freeze(1), sync_group=3, "shotgun-start_reload","reload_start(stand/crouch/jump)"
- one_round: cyclic(0), sync_group=4, "shotgun-reload_one","reload_cycle(stand/crouch/jump)"
- finish:    freeze(1), sync_group=5, "shotgun-finish_reload","reload_finish(stand/crouch/jump)"
finish ctor also inits m_owner_ready_for_transition(0) in init list (mov [+168],0 after vtable).
start/one_round ctors: empty body.

## Bodies (decoded)
- is_ready: start -> return m_animation_ended (mov al,[+168]); one_round/finish -> return true (mov al,1).
- finalize (all): ASSERT(empty_stub) + m_animation_playback_state->reset() + remove_animation_callback(channel_id_on_animation_end,this).
  finish.finalize additionally: if(!deserializing() && chamber_a_round_on_reload() && ammo_in_magazine()) instant_chamber_a_round().
- initialize: set_animation_callback(channel_id_on_animation_end, this, boost::bind(&this::on_animation_end,this,_1)).
  start.initialize prepends m_animation_ended=false and appends
  if(!deserializing() && chamber_a_round_on_reload() && round_is_chambered()) unload_chambered_round().
- on_animation_end (all): params.interrupt_animation_player_tick=false;
  if(params.animated_object==&m_weapon){ ASSERT; if(m_animation_to_wait_for==params.animation){ <action>; interrupt=true; } } return call_me_again.
  action: start -> m_animation_ended=true ; one_round -> m_weapon.reload_one_round() ;
          finish -> *m_owner_ready_for_transition=true.

## Anchoring
temp_include_all.cpp: template use_game_core_shotgun_reload_substate_impl<T> constructs
the substate (emits vtable -> virtuals; initialize's bind keeps on_animation_end).
Three wrappers + dispatcher calls added.

## Access fix in base header
Base substate members (m_weapon/m_animation_to_wait_for/m_animation_playback_state) were
`private` -> subclasses couldn't access them (C2248). asm proves subclasses read them directly,
so they must be `protected`. Changed base header `private:` member block to `protected:`. This
is accessibility-only; it does not change the already-matched base substate's layout/codegen.

## Results (report.json fuzzy_match_percent)
ctor:                    start 100   one_round 100   finish 100   DONE
is_ready_for_transition: start 100   one_round 100   finish 100   DONE
initialize:              start 97.45 one_round 100   finish 100
finalize:                start 83.42 one_round 83.42 finish 89.40
on_animation_end:        start 83.0  one_round 83.55 finish 83.55

## Residual analysis (all unsteerable, outside this unit's scope)
- finalize (all): animation_playback_state::reset() has a REAL body (xorps; mov [eax],0;
  movss [eax+4],xmm0 - zeroes the two fields of m_animation_playback_state @ this+0x148) and
  is kept OUT-OF-LINE in the target (standalone symbol @ target rva 0x087f60). In our /GL base
  it is ABSENT from the rich index (pdb_rich_query base ::reset -> "no function matched"): the
  whole-program build elides the call entirely (the playback_state member is not observed after
  finalize, so LTCG DSEs the two stores). This is the documented per-call-site inline/elide-vs-call
  LTCG wall (assembly_patterns.md "animation_playback_state::reset() in
  weapon_core_aimed_state_base::finalize"), NOT a no-op stub - corrected from the earlier claim.
  Same wall as weapon_core_shotgun_reload_state::finalize (78%). The elided reset() also shifts the
  downstream register numbers (the +0x148 load is gone), explaining the chamber-refill if churn in
  finish.finalize. Plus the benign dummy::nonnull vs finalize_impl ICF fold of the compiled-out
  ASSERT (both @0x3f210, byte-identical at link).
- on_animation_end (all): dummy::nonnull/finalize_impl ICF fold + the LTCG operand scheduling
  of intrusive_ptr::operator== (target computes &m_animation_to_wait_for before loading the
  params.animation arg; ours pushes the arg first - call-boundary temp/arg residual). Structure
  matches the carcass statement-for-statement.
- start::initialize: only divergence is weapon_core::round_is_chambered() inline-vs-call (header
  STUB inlines m_is_round_chambered, target calls out-of-line). weapon_core.h is owned by another
  worker, out of scope. Same wall as weapon_core_reload_state_base::initialize (92%).
- finish::finalize: the missing reset() call also shifts downstream register numbers in the
  chamber-refill if; structure is correct.

## Anchoring (final)
temp_include_all.cpp: template use_game_core_shotgun_reload_substate_impl<T> constructs the
substate (emits vtable -> virtuals initialize/finalize/is_ready_for_transition; initialize's
boost::bind keeps the non-virtual on_animation_end). Three wrappers + dispatcher calls.
