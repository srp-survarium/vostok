# vostok-pdb.nvim

In-repo Neovim frontend for `vostok-pdb`: navigate from the C++ being edited to
the assembly, statement structure, or two-sided diff for the function or
statement under the cursor.

The plugin is presentation only - every view is a `vostok-pdb` invocation
(async, rendered into scratch buffers). No plugin-side state, nothing to go
stale; target lookups key on the VA, the one thing that never moves.

## Interface

One command, tab-completable. The two-argument form opens a view; the
single-argument form toggles a setting or closes the views:

    :Vostok {base|target|diff} {stmt|asm|structure}
    :Vostok {hints|autobuild|diffhl|close}

|            | `stmt` (one statement)            | `asm` (function, rich)  | `structure`     |
|------------|-----------------------------------|-------------------------|-----------------|
| **base**   | asm of the statement at cursor    | rich asm                | statement table |
| **target** | the PAIRED statement's asm        | rich asm                | statement table |
| **diff**   | asm diff for the statement        | asm diff                | structure diff  |

Buffer-local keymaps (no visual mode here):

    vbs / vts / vds   base / target structure, structure diff
    vbf / vtf / vdf   base / target function asm, asm diff
    vba / vta / vda   the same (a = asm)
    vo                target | base asm side by side, scrollbound
                      (the objdiff look)
    vDa / vDs         target | base in TWO windows as a native diff
                      (asm / structure). Each pane is NORMALIZED so only real
                      divergences show: asm drops the per-instruction offset and
                      the source-carrying `[0x..]:` markers (keeping mnemonics +
                      labels); structure keeps just the `size` skeleton. So the
                      diff no longer lights up every line from shifted addresses
                      / base-only source. `:Vostok diffhl` toggles the coloring
                      off (keeping the alignment) for a clean side-by-side read.
                      (Addresses are stripped here, so use the single-side views
                      for `<CR>` navigation.) `vo` is the raw scrollbound look.
    vB                rebuild (:VostokRebuild) and refresh the views
    V                 statement asm peek - base in source buffers; inside
                      plugin tables the side comes from the column under
                      the cursor (t.addr vs b.addr) or the diff line prefix

`:Vostok diff stmt` (the diff opened at the statement) has no binding.

- Function/statement at cursor resolves through the project's base evidence DB
  (PDB line tables); a `0x<addr>` under the cursor wins over position.
- In every plugin view, addresses are links: `<CR>` jumps to that statement's
  asm on the address's own side; `q` closes; a view stack gives back-nav.
- `stmt` views float; `asm`/`structure`/`diff` open a reusable scratch split.

The project root is resolved from the EDITED BUFFER's path (walk up to
`binaries/pdb/base/evidence.sqlite`), so sibling worktrees query their own build.

## Inline match metrics

Like coc's inlay hints: every function in a C/C++ source buffer is tagged with
its match state as end-of-line virtual text, anchored on its opening line and
colored by how close it is (green ≥100%, yellow ≥50%, red below, grey for an
unpaired function):

    cur%   ↑best%   s<m>/<t>   ℓ<m>/<t>   N⟳

- **cur%** - current fuzzy match % (`✓100%` when byte-exact).
- **↑best%** - best-ever fuzzy % (shown only when above current; the
  ICF-churn-immune ceiling).
- **s m/t** - structure: body-statement skeleton agreement, **count-based** and
  brace-trimmed (the synthetic `{`/`}` are dropped, matching `vostok-pdb`'s
  structure view): `m/t = min/max` of the two sides' body-statement counts, so
  any count gap counts against you in **either** direction (everything is
  steerable). Equal counts read as a full match - byte-size differences are what
  the `%` above captures, so a clearly-structured function no longer reads
  `4/14`. The one exception: an **empty body** is hidden (no source statements
  exist to structure-match; the target's come purely from compiler-generated
  code such as a member-dtor walk).
- **ℓ m/t** - locals: PDB-recorded local counts agreeing across the two sides
  (a separate field; shown only when either side declares locals).
- **N⟳** - how many times the matcher has been dispatched on this function.

Current, historical-best, and retry counts come from the committed
`config/match_state.tsv` ledger, keyed by decorated name. Structure and locals
come from the base/target PDB evidence databases. The view headers and build
popup show the same metrics. They refresh on enter/save and after a build.
Toggle with `:Vostok hints`, or default off with `hints = false`.

## Rebuilding without leaving the editor

`:VostokRebuild [args]` (or `vB`) runs `python3 -m vostok build --foreground` in
a terminal split at the project root, refreshing everything the views read.
On success the inline metrics
refresh, any open `asm`/`structure`/`diff` views re-render in place (cursor and
scroll preserved), and a corner popup reports the overall % change and the
functions that moved. Extra args pass through to `rebuild.py` (e.g.
`:VostokRebuild logging` builds one project first).

### Build on save (`:Vostok autobuild`)

Off by default. When on, **saving any source** kicks a *quiet* incremental
rebuild - no terminal, just a small `rebuilding …` corner note that closes into
the result popup - so the inline %s update right after you save and any open
view re-renders. Rapid saves supersede: a new build cancels the in-flight one
(latest wins), so saving several times quickly doesn't queue builds or race
the build on `binaries/`. It is a full incremental build,
so keep it off unless you want the tight edit→`:w`→watch-the-%-move loop.

The `hints` and `autobuild` toggles are remembered per checkout in
`binaries/vostok_pdb-nvim.json` (gitignored), so each worktree keeps its own
setting across restarts; a remembered toggle wins over the `setup` default.

## Requirements

`vostok-pdb` on PATH, generated
`binaries/pdb/{base,target}/evidence.sqlite` databases, and
`config/match_state.tsv` for metrics. The Vostok dev shell provides the binary.

## Install

The project dev shell wraps `nvim` so it auto-loads this checked-in plugin,
without changing the user's configuration. Plain `nvim` outside the shell is
unchanged.

The first use in a buffer queries the base evidence DB for that file's functions
and caches the result against the database mtime.

Optional config (defaults shown):

    require("vostok_pdb").setup({
      keymaps = true,            -- set false to bind your own
      hints = true,              -- inline match metrics per function
      build_on_save = false,     -- quiet rebuild.py on every source save
      diff_highlight = true,     -- vDa/vDs coloring (off = aligned, no color)
      split = "botright vsplit", -- where views open
    })

## Line numbers go stale - addresses don't

Everything cursor-based maps through the LAST BUILD's PDB line tables: edit a
file and every resolution below the edit shifts until the next rebuild. The
plugin detects this (unsaved buffer edits, or source newer than the index)
and prefixes views with a `; STALE LINES:` warning instead of resolving
silently wrong. When in doubt, navigate by address - a `0x...` under the
cursor bypasses line mapping entirely, and target VAs never move.

## Pairing notes

- Cross-side function selection uses the qualified path derived from the
  mangled name: full demangled signatures do NOT pair (the target PDB spells
  top-level parameter const, the base does not).
- `target stmt` pairs statements by line DELTA from each side's first
  statement (absolute line numbers differ between the two sources) - the
  same key the structure-diff's `t.ln/b.ln` columns use.
