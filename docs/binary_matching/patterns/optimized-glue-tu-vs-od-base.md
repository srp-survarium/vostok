# A whole glue TU was compiled OPTIMIZED in gold while our base builds /Od (wall)
tags: cpp:inline cpp:member cpp:wrapper | asm:mov asm:ret asm:call | topic:inline-vs-call topic:wall topic:convention
symptoms: target helper "0 stmts / 1 byte", getter is `mov eax,[eax+8]; ret`, base has +1 QUANTITY stmt per gfx()/wrapper call, frameless target vs framed /Od
confidence: 8/10
variants: optimized-comdat-in-od-unit.md, od-helper-inline-depth-wall.md, inline-vs-call-trivial-getter.md

Distinct from optimized-comdat-in-od-unit (one folded COMDAT) and from od-helper-inline-depth
(a missing inline keyword on ONE helper): here the ENTIRE non-template TU was compiled with
optimization in the shipped binary, while our base module builds `/Od`. Every thin wrapper in
the TU - the addressed `gfx()`-style bridge helper, the SDK forwarders (`gfx(this)->SetUInt(v)`),
the ctors/dtors - is byte-capped no matter how faithful the source.

Decisive tells (run `pdb_fetch --view structure` / `structure-diff`):
- a trivial bridge helper records **"0 stmts / 1 byte"** in the target (the identity cast got
  inlined into every caller AND emitted standalone as a bare `ret`); our /Od copy is a real
  8-byte `call` with a statement.
- trivial getters are `mov eax,[eax+8]; ret` (3 bytes, frameless) in the target; the /Od base
  is a full frame + `call helper` + slots, so objdiff cannot even PAIR them (UNPAIRED).
- per wrapper, the `structure-diff` shows a single `b_only` statement = the un-inlined helper
  call (`match_db` files it as **QUANTITY**, b_stmts = t_stmts + 1). This is NOT a steerable
  statement-count bug: the extra statement IS the helper call the optimizer inlined away.
- the non-trivial members still pair clean (e.g. a 5-7 statement `Call`/`Callback` pairs
  all statements SIZE-only) - that is the proof the SOURCE STRUCTURE is right; only the bytes
  are optimization residual.

Action: write the normal wrapper source (`gfx(this)->Method(args)`, `m_movie->Method(...)`,
`impl = SF_NEW impl_type(*this)`, `new (body) Value()`, `gfx(this)->~Value()`), confirm the
statement structure pairs (SIZE/QUANTITY-by-one), then PARK the byte residual as the
/Od-vs-optimized-TU wall. Do NOT collapse the wrapper to dodge the extra statement, and do NOT
keep the do-nothing stub just because tiny-vs-tiny scored higher - the stub is wrong structure.

Evidence: vostok/scaleform/sources/value.cpp (the survarium::flash_value / flash_movie /
flash_function_handler / flash_external_handler glue over GFx::Value/Movie). `gfx(flash_value
const*)` target 0x5bab70 = 1 byte / 0 stmts; GetUInt/GetInt/GetBool = `mov eax,[eax+8]; ret`;
every setter b_stmts = t_stmts+1 (the inlined gfx() call); Call pairs 7/7 statements
SIZE-only. Earlier wall note: the match/scaleform-flash-text branch.

Sibling evidence: vostok/scaleform/sources/movie.cpp flash_text/flash_text_manager glue over
GFx::DrawText/DrawTextManager. get_width/get_height = `text_impl->GetRect().Width()/Height()`
(target reads RectF.x2-x1 / y2-y1 inline, frameless); set_text/set_position/set_font_size /
create_text(_w) build a `flash_text` and call GetTextExtent / GetRect / SetRect / CreateText.
Faithful structure pairs in order (`size += 5.f`, the RectF SetRect, owner->need_capture=true);
residual is /Od artifacts NOT source-steerable: (a) the NRVO return-object ctor folds in target
(create_text 6 t-stmts vs 8 b-stmts), (b) the optimizer SPLITS one SetRect call across two
statements while /Od keeps it one (multi-line arg formatting does NOT make /Od split it), (c)
target folds GetRect into the padding line. TWO REACHABILITY TRAPS unique here: the WRONG-STRUCTURE
do-nothing stub scored HIGHER (set_position stub 41% vs faithful 4/4-stmt body unpaired; set_text
stub 11.5% vs 0.9%) - take the faithful body anyway. And the game module's OWN flash_text.h /
flash_text_manager.h inline-stub get_width/get_height/set_font_size/create_text_w, so those four
scaleform out-of-line bodies are /OPT:REF-stripped (UNPAIRED on reachability, not matching); they
need a scaleform-module anchor chain that does not yet exist - park as cross-module reachability.
