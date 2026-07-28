# A commented-out body block is live in the target; reconstruct it from a vtable-index chain + CRT `_*_l` locale macros

## Symptom

A short function (our base ~4 statements, target ~10) where the target structure-diff
shows several `TRGT_ONLY` statements with no base counterpart, and the source has a large
`//`-commented block that was never reconstructed. The shipped binary kept that block LIVE.

Concretely (`vostok::ui::insert_char_action::execute`): base 0x40 bytes / 4 stmts,
target 0x124 bytes / 10 stmts. The dropped block was the keyboard-translate path that
used a member (`m_input_world`) only recovered by a recent layout fix.

## Decoding the target asm

1. **Virtual call by vtable offset → source method chain.** Read offsets against the
   interface declaration order:
   - `mov ecx,[this+M]; mov edx,[ecx]; mov eax,[edx+0x24]; call eax` = call the
     `0x24/4 = 9`th virtual of the member at offset M. Count the pure virtuals in the
     interface header (`input::world`: tick=0 … get_keyboard=9) → `m_input_world->get_keyboard()`.
   - The returned pointer is then `mov edx,[eax]; mov edx,[edx+4]; ... call edx` =
     the `1`st virtual of `keyboard` (`is_key_down=0, get_dik_name=1`) → `->get_dik_name(...)`.
   - The whole expression: `m_input_world->get_keyboard()->get_dik_name( m_key, buff, sizeof(buff) )`.
   - A raw-ptr smart wrapper (`raw<keyboard const>::ptr`) is a thin pointer; eax IS the
     pointer, so `->` chains directly with no extra deref instructions.

2. **CRT `_isalpha_l` is a MACRO, not a function.** `<ctype.h>` defines
   `_isalpha_l(c,loc) → _ischartype_l(c,_ALPHA,loc)`, whose inline expands to
   `loc && loc->locinfo->mb_cur_max>1 ? _isctype_l(c,_ALPHA,loc) : _chvalidchk_l(...)` —
   exactly a `cmp [loc+0ACh],1; jle` (mb_cur_max), `__pctype_func()` lookup, and
   `and eax, 103h` (`_ALPHA == 0x0103`). If the build errors `'_isalpha_l': identifier
   not found`, the macro just isn't visible — add `#include <ctype.h>`.

3. **Secure `_strlwr_s_l(buf, size, loc)`** (3 args: buffer, size, locale) — the asm
   `push loc; lea ecx,&buf; push 0x80; push ecx; call _strlwr_s_l`. The old non-secure
   `_strlwr_l(buf, loc)` in the comment maps to this `_s` variant under VS2008.
   `static _locale_t current_locale = _create_locale(LC_ALL/*=0*/, "")` shows as a `$S3`
   one-time-init guard around `_create_locale`.

## Fix

Uncomment and rewrite the block with the real method chain and CRT macros; add the
type-completing includes (`<vostok/input/world.h>`, `<locale.h>`, `<ctype.h>`).
24.6% → 100% (exact byte match, STRUCTURE MATCH).

## Lesson

When a layout fix recovers a member that a function's commented-out block referenced, the
block was almost certainly LIVE in the target — reconstruct it. PDB-recorded locals
(`char c`, `char[128] buff`) confirm the disabled `string128 buff;` was real. Source line
numbers do NOT affect bytes; match the CODE, let the structure-diff confirm the shape.
