# Assembly patterns (MSVC 8.0 / VS2008, x86, /Od + LTCG)

How recovered-assembly shapes map back to Vostok C++ source. The knowledge base
lives in [`patterns/`](patterns/) - ONE FILE PER PATTERN, indexed in
[`patterns/INDEX.md`](patterns/INDEX.md). There is no privileged "core list";
curation is sushi's.

## Searching (agents: do this when staring at a diff row)

1. Skim/grep [`patterns/INDEX.md`](patterns/INDEX.md) - one line per pattern
   (`- [title](file.md) — cN — tags — symptoms` (cN = confidence/10)):
   - by construct: `grep 'cpp:if' patterns/INDEX.md` (also `cpp:switch`, `cpp:ctor`, ...)
   - by mnemonic: `grep 'asm:sbb' patterns/INDEX.md` (also `asm:call`, `asm:movss`, ...)
   - by category: `grep 'topic:mangling' patterns/INDEX.md`
   - by symptom token: `grep 'TRGT_ONLY 0xc' patterns/INDEX.md`, `grep '@@ABE' ...`
2. Read ONLY the hit files; follow their `variants:` links for siblings.
3. Full-text fallback when the index line misses:
   `grep -rli '<token>' docs/binary_matching/patterns/`

## Per-file schema (authoring: new pattern = new file + one INDEX.md line, SAME commit)

```markdown
# <title - tightened, symptom-first>
tags: cpp:<construct> | asm:<mnemonic> | topic:<category>   (1+ of each kind where applicable)
symptoms: <raw grep tokens: asm fragments, b.diff tags, mangling letters, score behaviors>
confidence: N/10
variants: <sibling-file.md, ...>   (only when true variants of the same construct exist)

<DESCRIPTION first: 1-3 lean sentences - what this is and when you see it.>

```cpp
<minimal C++ spelling - what to WRITE>      (omit block if no C++ side)
```
```asm
<minimal target-side asm signature - what you SEE>   (omit block if no asm side)
```
<at most 1-2 lean trailing lines: steerable-or-wall + evidence (functions, %s, PRs).>
```

## Bootstrapping new patterns by compiling probes

You do not have to wait for a diff to surface a shape. Write a minimal snippet, compile it
with the real toolchain, and read the listing — the method that produced the foundational
`construct -> /Od asm` set in [`patterns/manual/`](patterns/manual/INDEX.md) (if/else,
loops, `&&`/`||`, comma, braces, calls, virtual dispatch, `_alloca`, the `VOSTOK_NEW`/
`DELETE` family, construct/destruct-in-place). The reproduction recipe — a standalone
`cl.exe` invocation with the game_core codegen flags minus `/GL` `/Zi` and the PCH, plus
`/FAcs` for a machine-code listing — is at the top of that INDEX. Reach for it whenever a
construct's lowering is unclear; promote anything reusable to a `patterns/` file as usual.

