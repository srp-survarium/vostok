# /Od inlining-depth wall: target CALLS a small helper, base INLINES it (structure already matches)
tags: cpp:inline cpp:call cpp:helper | asm:call asm:sbb asm:strstr | topic:inline-vs-call topic:wall
symptoms: STRUCTURE MATCH + locals match, one statement is SIZE+/- because base emits the helper's inlined body (strstr/branchless sbb-neg-and-add/c_str+strcmp_s) where target emits a single `call`
confidence: 7/10

In a /Od TU (logging is /Od, 0% frameless) a small `inline`/template helper that the TARGET
keeps OUT-OF-LINE (`call buffer_string::find`, `call vostok::math::min`, `call vostok::operator<`)
the BASE INLINES one level deeper. The statement structure and named-local set already MATCH; the
only diff is the inlining cut, which carries a cluster of unnamed compiler temporaries on the
inlined side (base grows [ebp-N] slots; target stays lean, often keeping a callee-saved reg).

Worked examples (all logging, all STRUCTURE MATCH, PARKED):
- `filter_tree::filter_is_overwritten` (30.9%): target `call buffer_string::find` + `test eax,eax`;
  base inlines `find(s,0)` down to `strstr` + `res - m_begin` offset math (5 temp slots).
- `log_file::process_next_line` (76%): `math::min(u32,u32)` -> target `call vostok::math::min`;
  base inlines `min_integral`'s branchless `sbb/neg/neg/and/add`. The u32 args select the by-VALUE
  `min(u32,u32)` overload (math_functions_inline.h), NOT the by-const-ref template `min<T>`.
- `compare_nodes::operator()(node_base,node_base)` (70%): `left.name < right.name` -> target inlines
  `buffer_string::operator<` (c_str + strcmp_s + `==-1`), base keeps `call vostok::operator<`.
  (Here the polarity is reversed — target inlines, base calls — same wall, opposite cut.)
- `game_world::add_decal` (54%, game): `float4x4 transform = create_rotation( direction, normal );`
  -> the TARGET inlines the whole `inline`-marked two-vector `create_rotation` (math_float4x4_inline.h:328
  — the `(normal ^ direction).normalize()` cross-product + the i/j/k/c row stores, 0x124 of math) while
  base OUT-LINES it to a single 0x13 `call vostok::math::create_rotation`. One SIZE -0x111 row, ~33% of
  the function's bytes, over an otherwise perfect 7/7 statement structure. A big-inline reversed cut. The
  `(normal^direction).normalize()`/axis+angle re-spellings score WORSE (they pull the math inline but
  break the surrounding statements that pair under the plain two-vector call): the faithful source is
  `create_rotation(direction, normal)`, parked at the inline cut.

The source spelling (`x.find(y) == 0`, `math::min(a,b)`, `a < b`) is already correct and faithful.
You CANNOT steer the inline cut from the consuming TU at /Od without distorting the natural code
(rewriting `<` as an explicit `strcmp_s(...)==-1`, or adding `__forceinline`/declaration-only views
of a SHARED header — both off-limits: don't change another unit's header to win this match). The
boost-intrusive `find`/iterator-`operator*`/member-hook-destructor expansion depth (node::set,
node::get_verbosity, node::~node) is the same wall. CONFIRM structure+locals, leave a claude@NOTE,
PARK with cause "inlining-depth, not source-steerable".
