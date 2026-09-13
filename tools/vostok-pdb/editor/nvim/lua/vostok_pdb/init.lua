-- vostok_pdb.nvim - presentation layer over the vostok_pdb CLI.
-- Every view IS a vostok_pdb invocation; the plugin only resolves what is
-- under the cursor (function/statement/address) and renders the output.

local M = {}
local uv = vim.uv or vim.loop

M.config = {
  keymaps = true, -- vbs/vts (structure), vbf/vtf (function asm), V (stmt peek)
  hints = true, -- inline match metrics (cur/best/structure/retries) per function
  build_on_save = false, -- run rebuild.py quietly whenever a source is saved
  diff_highlight = true, -- vDa/vDs color the diff; off = aligned, no coloring
  split = "botright vsplit", -- where view windows open
}

local INDEX = { base = "binaries/pdb/base/evidence.sqlite",
                target = "binaries/pdb/target/evidence.sqlite" }

-- ---------------------------------------------------------------- project --

--- Walk up from the buffer's file to the checkout containing the evidence DBs,
--- so sibling worktrees each query their own build.
local function project_root(bufnr)
  local file = vim.api.nvim_buf_get_name(bufnr or 0)
  if file == "" then return nil end
  for dir in vim.fs.parents(file) do
    if uv.fs_stat(dir .. "/" .. INDEX.base) then return dir end
  end
end

--- Index `file` fields are relative to <root>/sources/.
local function rel_source(file, root)
  local prefix = root .. "/sources/"
  if file:sub(1, #prefix) == prefix then return file:sub(#prefix + 1) end
end

-- ----------------------------------------------------- function at cursor --

-- {index_path -> {mtime=, files={relfile -> entries}}}
local cache = {}

local function entries_for(root, relfile, side)
  local idx = root .. "/" .. INDEX[side or "base"]
  local st = uv.fs_stat(idx)
  if not st then return nil end
  local c = cache[idx]
  if not c or c.mtime ~= st.mtime.sec then
    c = { mtime = st.mtime.sec, files = {} }
    cache[idx] = c
  end
  if c.files[relfile] then return c.files[relfile] end

  local entries = {}
  local output = vim.fn.system({ "vostok-pdb", "inspect", "--database", idx,
                                 "--file", relfile, "--list", "--json" })
  local ok, decoded = pcall(vim.json.decode, output)
  if ok and type(decoded) == "table" then entries = decoded end
  c.files[relfile] = entries
  return entries
end

local function line_span(e)
  local lo, hi = math.huge, 0
  for _, s in ipairs(e.statements or {}) do
    if s.line and s.line > 0 then
      lo, hi = math.min(lo, s.line), math.max(hi, s.line)
    end
  end
  return lo, hi
end

--- The function at `lnum`. The cursor inside a function's statement span wins
--- (tightest span - inlined-into neighbors can overlap). The first statement is
--- the opening `{`, so the SIGNATURE line(s) above it aren't in any span; when
--- nothing covers the cursor, fall back to the function whose body starts just
--- below (the cursor is on its signature / leading comment).
local function function_at(root, relfile, lnum)
  local best, best_width
  local below, below_lo
  for _, e in ipairs(entries_for(root, relfile) or {}) do
    local lo, hi = line_span(e)
    if lo <= hi then
      if lnum >= lo and lnum <= hi then
        local width = hi - lo
        if not best or width < best_width then best, best_width = e, width end
      elseif lo >= lnum and (not below_lo or lo < below_lo) then
        below, below_lo = e, lo
      end
    end
  end
  return best or below
end

--- The statement starting exactly on `lnum` (first if several).
local function stmt_at(entry, lnum)
  for _, s in ipairs(entry.statements or {}) do
    if s.line == lnum then return s end
  end
end

local function va_of(entry, off)
  return string.format("0x%x", entry.image_base + entry.rva + off)
end

--- Cross-side `--function` selector. Full demangled signatures do NOT pair
--- across the two PDBs (the target spells top-level parameter const, the
--- base does not), but the qualified path matches both. Derive it from the
--- mangled name - identical on both sides; `?fn@inner@outer@@...` is
--- innermost-first. Templates/operators mangle with `?$`/`??` and fall back
--- to the demangled signature (same-side views still work).
local function qualified_name(entry)
  local path = (entry.mangled or ""):match("^%?([%w_@]-)@@")
  if path and path ~= "" then
    local parts = vim.split(path, "@", { plain = true })
    local rev = {}
    for i = #parts, 1, -1 do rev[#rev + 1] = parts[i] end
    return table.concat(rev, "::")
  end
  return entry.name
end

-- ---------------------------------------------------------- match metrics --
-- Per-function match state is the committed config/match_state.tsv ledger.
-- Statement and local counts come from the two evidence DBs below, because the
-- ledger intentionally stores only the derived structure class.

local MATCH_STATE = "config/match_state.tsv"

local metrics_cache = {} -- root -> { mtime=, by={ mangled -> rec } }
local function number_or_nil(value)
  if value == nil or value == "" then return nil end
  return tonumber(value)
end

--- { mangled -> {cur,best,tries,cls,module,size} } for `root`, cached against
--- match_state.tsv's mtime. nil if the ledger is absent.
local function load_metrics(root)
  local ledger = root .. "/" .. MATCH_STATE
  local st = uv.fs_stat(ledger)
  if not st then return nil end
  local c = metrics_cache[root]
  if c and c.mtime == st.mtime.sec then return c.by end
  local fd = io.open(ledger, "r")
  if not fd then return c and c.by or nil end
  local by = {}
  local columns
  for line in fd:lines() do
    if line:sub(1, 1) ~= "#" and line ~= "" then
      local cells = vim.split(line, "\t", { plain = true })
      if not columns then
        columns = {}
        for i, name in ipairs(cells) do columns[name] = i end
      elseif columns.mangled then
        local mangled = cells[columns.mangled]
        by[mangled] = {
          cur = number_or_nil(cells[columns.cur]),
          best = number_or_nil(cells[columns.hist]),
          tries = number_or_nil(cells[columns.tries]) or 0,
          cls = cells[columns.cls], module = cells[columns.module],
          size = number_or_nil(cells[columns.size]) or 0,
        }
      end
    end
  end
  fd:close()
  metrics_cache[root] = { mtime = st.mtime.sec, by = by }
  return by
end

local function invalidate_metrics(root) metrics_cache[root] = nil end

-- ---- formatting ----

local function hint_hl(cur)
  if cur == nil then return "Comment" end
  if cur >= 99.995 then return "DiagnosticOk" end
  if cur >= 50 then return "DiagnosticWarn" end
  return "DiagnosticError"
end

--- Locals agreement for a function: the count of PDB-recorded locals on each
--- side, as min/max. { l_m, l_t } or
--- nil when neither side declares any. base entry required; target optional.
local function locals_counts(base_e, target_e)
  local b = base_e and #(base_e.locals or {}) or 0
  if not target_e then
    return b > 0 and { l_m = 0, l_t = b } or nil
  end
  local t = #(target_e.locals or {})
  if b == 0 and t == 0 then return nil end
  return { l_m = math.min(b, t), l_t = math.max(b, t) }
end

local function structure_counts(base_e, target_e)
  local b = base_e and math.max(#(base_e.statements or {}) - 2, 0) or 0
  if b == 0 then return nil end
  local t = target_e and math.max(#(target_e.statements or {}) - 2, 0) or 0
  return { st_m = math.min(b, t), st_t = math.max(b, t) }
end

--- Evidence counts for `mangled` in `relfile`, pairing base and target entries
--- by decorated identity. Returns locals, structure; either may be nil.
local function evidence_pair(root, relfile, mangled)
  if not (relfile and mangled) then return nil end
  local be, te
  for _, e in ipairs(entries_for(root, relfile, "base") or {}) do
    if e.mangled == mangled then be = e break end
  end
  if not be then return nil end
  for _, e in ipairs(entries_for(root, relfile, "target") or {}) do
    if e.mangled == mangled then te = e break end
  end
  return locals_counts(be, te), structure_counts(be, te)
end

--- eol virtual-text chunks: cur% (colored) · ↑best · s m/t (structure) · ℓ m/t
--- (locals) · N⟳ retries. An unpaired function (no record / no cur) is "— n/a".
local function metric_chunks(rec, loc, structure)
  if not rec or rec.cur == nil then
    return { { "  — n/a", "Comment" } }
  end
  local cur = rec.cur >= 99.995 and "✓100%" or string.format("%.0f%%", rec.cur)
  local chunks = { { "  " .. cur, hint_hl(rec.cur) } }
  if rec.best and rec.best > (rec.cur + 0.5) then
    chunks[#chunks + 1] = { ("  ↑%.0f%%"):format(rec.best), "Comment" }
  end
  if structure and structure.st_t > 0 then
    chunks[#chunks + 1] = {
      ("  s%d/%d"):format(structure.st_m, structure.st_t), "Comment"
    }
  end
  if loc and loc.l_t and loc.l_t > 0 then
    chunks[#chunks + 1] = { ("  ℓ%d/%d"):format(loc.l_m, loc.l_t), "Comment" }
  end
  if rec.tries and rec.tries > 0 then
    chunks[#chunks + 1] = { ("  %d⟳"):format(rec.tries), "Comment" }
  end
  return chunks
end

--- A `;`-prefixed header line summarising a function's metrics, for view tops.
local function metric_header(rec, name, loc, structure)
  if not rec then return nil end
  local parts = {}
  parts[#parts + 1] = rec.cur ~= nil
    and (rec.cur >= 99.995 and "100% (exact)" or ("%.1f%%"):format(rec.cur))
    or "unpaired"
  if rec.best ~= nil then parts[#parts + 1] = ("best %.1f%%"):format(rec.best) end
  if structure and structure.st_t > 0 then
    parts[#parts + 1] = ("structure %d/%d"):format(
      structure.st_m, structure.st_t)
  end
  if loc and loc.l_t and loc.l_t > 0 then
    parts[#parts + 1] = ("locals %d/%d"):format(loc.l_m, loc.l_t)
  end
  if rec.cls then parts[#parts + 1] = rec.cls end
  if rec.tries and rec.tries > 0 then parts[#parts + 1] = ("%d tries"):format(rec.tries) end
  return "; " .. (name and (name .. "  ") or "") .. table.concat(parts, "  ·  ")
end

-- -------------------------------------------------------------- execution --

local function side_args(root, side)
  local args = { "inspect" }
  if side == "base" or side == "diff" then
    vim.list_extend(args, { "--base", INDEX.base })
  end
  if side == "target" or side == "diff" then
    vim.list_extend(args, { "--target", INDEX.target })
  end
  return args
end

-- every invocation is logged (:VostokLog) with the resolved binary path -
-- a stale dev shell shipping an old vostok_pdb is the classic silent killer
-- (an old --address selector re-based VAs as RVAs: wrong function entirely)
M._log = {}
local function log(line)
  table.insert(M._log, os.date("%H:%M:%S ") .. line)
  if #M._log > 200 then table.remove(M._log, 1) end
end

function M.show_log()
  local lines = #M._log > 0 and vim.deepcopy(M._log) or { "(no queries yet)" }
  table.insert(lines, 1, "vostok-pdb binary: " ..
    (vim.fn.exepath("vostok-pdb") ~= "" and vim.fn.exepath("vostok-pdb")
     or "NOT ON PATH"))
  vim.cmd(M.config.split)
  local buf = vim.api.nvim_create_buf(false, true)
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines)
  vim.bo[buf].buftype = "nofile"
  vim.bo[buf].bufhidden = "wipe"
  vim.api.nvim_win_set_buf(0, buf)
  vim.keymap.set("n", "q", "<cmd>close<cr>", { buffer = buf, nowait = true })
end

local function run(root, args, cb)
  if vim.fn.executable("vostok-pdb") == 0 then
    return vim.notify("vostok-pdb: not on PATH - launch nvim from the " ..
      "project dev shell (nix develop)", vim.log.levels.ERROR)
  end
  local cmd = vim.list_extend({ "vostok-pdb" }, args)
  log(table.concat(cmd, " ") .. "  [" .. root .. "]")
  vim.system(cmd, { cwd = root, text = true }, function(res)
    vim.schedule(function()
      local out = (res.stdout or "")
      if res.code ~= 0 and (res.stderr or "") ~= "" then
        out = out .. res.stderr
      end
      local lines = vim.split(out, "\n", { trimempty = true })
      if #lines == 0 then lines = { "(vostok_pdb produced no output)" } end
      cb(lines)
    end)
  end)
end

-- -------------------------------------------------------------- rendering --

-- syntax per view: rich asm gets asm highlighting, the asm diff keeps
-- diff's +/- coloring; tables stay plain
local VIEW_FT = { base = "asm", target = "asm", diff = "diff" }

local function fill(buf, lines, ctx, transient)
  vim.bo[buf].modifiable = true
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines)
  vim.bo[buf].modifiable = false
  vim.bo[buf].buftype = "nofile"
  vim.bo[buf].swapfile = false
  vim.bo[buf].filetype = VIEW_FT[ctx.view] or ""
  -- splits/peeks: float buffers die with their window, view buffers persist
  -- (survive :split and window close, refresh on the next same query)
  vim.bo[buf].bufhidden = transient and "wipe" or "hide"
  vim.b[buf].vostok_pdb = ctx
  local opts = { buffer = buf, nowait = true, silent = true }
  vim.keymap.set("n", "q", "<cmd>close<cr>", opts)
  vim.keymap.set("n", "<CR>", function() M.follow() end, opts)
  vim.keymap.set("n", "ga", function() M.follow() end, opts)
  -- V peeks here too: side inferred from the table column / line prefix
  vim.keymap.set("n", "V", function() M.follow() end, opts)
end

--- Peeks show JUST the asm: drop vostok_pdb's `;` meta and signature lines,
--- keep the [0xNN] statement marker and instruction lines.
local function asm_only(lines)
  local asm = {}
  for _, l in ipairs(lines) do
    if l:match("^%[?0[xX]%x") then asm[#asm + 1] = l end
  end
  if #asm > 0 then return asm end
  return lines -- errors/odd output stay verbatim
end

--- Strip the per-side NOISE that makes a native two-window diff (vDa/vDs)
--- light up every line, leaving only what is meant to line up across sides:
---   * asm: drop the per-instruction `0xNN:` offset prefix and the `[0xNN]:`
---     statement markers (base appends the source line there, target doesn't);
---     keep `mnemonic operands` + jump labels - so only diverging instructions
---     highlight (the objdiff intent), not every shifted address.
---   * structure: keep just the `size` column (the statement skeleton); drop the
---     absolute address, function-relative offset, source line, and the base-
---     only `code` column - all of which differ between the two PDBs by design.
--- Absolute addresses are gone, so `<CR>` follow doesn't work in these panes;
--- use the single-side views (vbf/vbs/...) for navigation.
local function diff_normalize(lines, kind)
  local out = {}
  for _, l in ipairs(lines) do
    if kind == "structure" then
      local size = l:match("^%s*0[xX]%x+%s*|[^|]*|%s*([^|]+)")
      if size then
        out[#out + 1] = vim.trim(size)
      elseif l:match("|offst|") or l:match("^%s*%-+%+") or l:match("^%s*;") then
        -- drop the column header, the `---+---` separator, and `; ...` meta
      else
        out[#out + 1] = l -- signature, `{`, `}`: identical on both sides
      end
    else -- asm
      if not l:match("^%s*%[0[xX]%x+%]:") then -- drop [0xNN]: statement markers
        out[#out + 1] = (l:gsub("^%s*0[xX]%x+:%s*", "")) -- strip instr offset
      end
    end
  end
  return out
end

-- vDa/vDs use Neovim's diff for ALIGNMENT (filler lines line matching code up
-- across the two panes). When diff_highlight is off we blank the Diff* groups in
-- those windows via winhighlight, so you get the aligned side-by-side with NO
-- coloring at all - scoped to these windows, your other diffs keep their look.
local DIFF_NOHL =
  "DiffAdd:Normal,DiffChange:Normal,DiffText:Normal,DiffDelete:Normal"
local function apply_diff_hl(win)
  if vim.api.nvim_win_is_valid(win) then
    vim.wo[win].winhighlight = M.config.diff_highlight and "" or DIFF_NOHL
  end
end

--- The side lives in the float title (BASE/TARGET + address) so it is never
--- ambiguous; pre-composed content (ctx.raw) brings its own title.
local function show_float(lines, ctx)
  if not ctx.raw then
    lines = asm_only(lines)
    ctx.title = (ctx.side or "?"):upper() .. (ctx.addr and (" " .. ctx.addr) or "")
  end
  local width = 0
  for _, l in ipairs(lines) do width = math.max(width, #l) end
  width = math.min(width + 1, vim.o.columns - 4)
  local height = math.min(#lines, math.floor(vim.o.lines * 0.6))
  local buf = vim.api.nvim_create_buf(false, true)
  fill(buf, lines, ctx, true)
  local win = vim.api.nvim_open_win(buf, true, {
    relative = "cursor", row = 1, col = 0,
    width = width, height = height,
    style = "minimal", border = "single",
    title = ctx.title, title_pos = "left",
  })
  -- a peek dismisses itself: leaving the float closes it (q/<Esc> too)
  vim.keymap.set("n", "<Esc>", "<cmd>close<cr>",
    { buffer = buf, nowait = true, silent = true })
  vim.api.nvim_create_autocmd("WinLeave", {
    buffer = buf, once = true,
    callback = function()
      if vim.api.nvim_win_is_valid(win) then
        vim.api.nvim_win_close(win, true)
      end
    end,
  })
end

--- One buffer per (side, view, symbol): rerunning the same query refreshes
--- it in place, different queries coexist - open as many splits as you like.
local function show_split(lines, ctx)
  local sym = (ctx.name or "?"):gsub("[^%w_:~]+", "."):sub(1, 80)
  local name = ("pdbfetch://%s-%s/%s"):format(ctx.side, ctx.view, sym)
  local buf = vim.fn.bufnr("^" .. vim.fn.fnameescape(name) .. "$")
  if buf == -1 then
    buf = vim.api.nvim_create_buf(true, true)
    vim.api.nvim_buf_set_name(buf, name)
  end
  fill(buf, lines, ctx)
  local win
  for _, w in ipairs(vim.api.nvim_tabpage_list_wins(0)) do
    if vim.api.nvim_win_get_buf(w) == buf then win = w break end
  end
  if win then
    vim.api.nvim_set_current_win(win)
  else
    vim.cmd(M.config.split)
    vim.api.nvim_win_set_buf(0, buf)
  end
  if ctx.search then vim.fn.search(ctx.search) end
end

-- ---------------------------------------------------------- address links --

--- Header-table column at the cursor, for structure-diff side resolution:
--- returns the name of the `|`-separated field the cursor column falls in.
local function column_at(header, col)
  local start = 0
  for field in (header .. "|"):gmatch("([^|]*)|") do
    local stop = start + #field
    if col >= start and col <= stop then return vim.trim(field) end
    start = stop + 1
  end
end

--- Both statements of a structure-diff row, side by side in one float:
--- target asm left, base asm right, sizes from the row's t.sz/b.sz columns.
local function peek_pair(ctx, row, header)
  local fields = {}
  local start = 0
  for field in (header .. "|"):gmatch("([^|]*)|") do
    local value = vim.trim(row:sub(start + 1, start + #field))
    fields[vim.trim(field)] = value
    start = start + #field + 1
  end
  -- `t.va`/`b.va` since vostok_pdb labelled its address spaces; `t.addr`/
  -- `b.addr` before that. Accept both so the plugin and the tool can skew.
  local taddr = (fields["t.va"] or fields["t.addr"] or ""):match("^0x%x+$")
  local baddr = (fields["b.va"] or fields["b.addr"] or ""):match("^0x%x+$")
  if not (taddr and baddr) then return false end -- one-sided row: single peek

  local function fetch(side, addr, cb)
    local a = side_args(ctx.root, side)
    vim.list_extend(a, { "--address", addr,
                         "--view", side == "target" and "target" or "base" })
    run(ctx.root, a, cb)
  end
  fetch("target", taddr, function(tl)
    fetch("base", baddr, function(bl)
      tl, bl = asm_only(tl), asm_only(bl)
      local width = 0
      for _, l in ipairs(tl) do width = math.max(width, #l) end
      local out = {}
      for i = 1, math.max(#tl, #bl) do
        local l, r = tl[i] or "", bl[i] or ""
        out[#out + 1] = l .. string.rep(" ", width - #l + 2) .. "| " .. r
      end
      show_float(out, {
        root = ctx.root, side = "diff", view = "pair", raw = true,
        title = ("TARGET %s %s | BASE %s %s"):format(
          taddr, fields["t.sz"] or "", baddr, fields["b.sz"] or ""),
      })
    end)
  end)
  return true
end

--- Follow the address/offset under the cursor inside a plugin view.
function M.follow()
  local ctx = vim.b.vostok_pdb
  if not ctx then return end

  -- a single statement's asm alone is rarely what you want from a paired
  -- structure-diff row: peek BOTH sides side by side when the row has both
  -- addresses (works from anywhere on the row, no address under cursor needed)
  local sd_header
  if ctx.view == "structure-diff" then
    for _, l in ipairs(vim.api.nvim_buf_get_lines(0, 0, 20, false)) do
      if l:find("|t.va", 1, true) or l:find("|t.addr", 1, true) then
        sd_header = l break
      end
    end
    if sd_header
        and peek_pair(ctx, vim.api.nvim_get_current_line(), sd_header) then
      return
    end
  end

  local word = vim.fn.expand("<cword>")
  local hex = word:match("^0[xX]%x+$") and word
  if not hex then
    return vim.notify("vostok_pdb: no address under cursor", vim.log.levels.INFO)
  end

  local side, selector = ctx.side, nil
  if ctx.view == "structure-diff" then
    local field = sd_header and column_at(sd_header, vim.fn.col(".") - 1) or ""
    side = field:find("^t%.") and "target" or "base"
    selector = { "--address", hex }
  elseif ctx.view == "diff" then
    -- side comes from the -/+ line prefix; numbers are function offsets
    local prefix = vim.api.nvim_get_current_line():sub(1, 1)
    side = (prefix == "+") and "target" or "base"
    selector = { "--function", ctx.name, "--offset", hex }
  elseif ctx.view == "structure" then
    selector = { "--address", hex } -- the `va` column, view's own side
  else -- rich asm: [0xNN] statement heads / instruction offsets
    selector = { "--function", ctx.name, "--offset", hex }
  end

  local view = (side == "target") and "target" or "base"
  local args = side_args(ctx.root, side)
  vim.list_extend(args, selector)
  vim.list_extend(args, { "--view", view })
  run(ctx.root, args, function(lines)
    show_float(lines, { root = ctx.root, side = side, view = view,
                        name = ctx.name, addr = hex })
  end)
end

-- ------------------------------------------------------------ inline hints --

local HINT_NS = vim.api.nvim_create_namespace("vostok_pdb_pct")

--- Inline match metrics as end-of-line virtual text on each function's opening
--- line (its first body statement, from base PDB evidence): cur% · ↑best · m/t
--- · N⟳, colored by current %. Reads the mtime-cached TSV ledger
--- - refreshed on enter/save and after a build. Non-project buffers no-op.
function M.hints(buf)
  buf = buf or vim.api.nvim_get_current_buf()
  if not vim.api.nvim_buf_is_loaded(buf) then return end
  vim.api.nvim_buf_clear_namespace(buf, HINT_NS, 0, -1)
  if not M.config.hints then return end
  local root = project_root(buf)
  if not root then return end
  local relfile = rel_source(vim.api.nvim_buf_get_name(buf), root)
  if not relfile then return end
  local by = load_metrics(root)
  if not by then return end
  -- target entries (by mangled) for the locals field - the only metric not in
  -- ledger; one cached target evidence query supplies locals/structure counts.
  local tgt = {}
  for _, e in ipairs(entries_for(root, relfile, "target") or {}) do
    if e.mangled and not tgt[e.mangled] then tgt[e.mangled] = e end
  end
  -- one hint per function, on its first body statement line; when two functions
  -- share that line (inlining overlap), the enclosing (widest-span) one owns it.
  local at = {} -- lnum -> { entry=, width= }
  for _, e in ipairs(entries_for(root, relfile) or {}) do
    local lo, hi = line_span(e)
    if lo <= hi then
      local prev = at[lo]
      if not prev or (hi - lo) > prev.width then
        at[lo] = { entry = e, width = hi - lo }
      end
    end
  end
  local last = vim.api.nvim_buf_line_count(buf)
  for lnum, v in pairs(at) do
    if lnum >= 1 and lnum <= last then
      local e = v.entry
      vim.api.nvim_buf_set_extmark(buf, HINT_NS, lnum - 1, 0, {
        virt_text = metric_chunks(by[e.mangled], locals_counts(e, tgt[e.mangled]),
          structure_counts(e, tgt[e.mangled])),
        virt_text_pos = "eol", hl_mode = "combine",
      })
    end
  end
end

--- Re-render hints in every loaded buffer (a build moves the %s); non-project
--- buffers early-return inside M.hints.
local function refresh_all_hints()
  for _, b in ipairs(vim.api.nvim_list_bufs()) do M.hints(b) end
end

-- ------------------------------------------------------------ entry point --

local VIEW_FLAG = {
  base = { structure = "structure", asm = "base" },
  target = { structure = "structure", asm = "target" },
  diff = { structure = "structure-diff", asm = "diff" },
}

--- :Vostok {base|target|diff} {stmt|asm|structure} from a source buffer.
function M.view(side, kind)
  local root = project_root(0)
  if not root then
    return vim.notify("vostok_pdb: no PDB evidence databases above this file",
      vim.log.levels.ERROR)
  end
  local relfile = rel_source(vim.api.nvim_buf_get_name(0), root)
  local lnum = vim.api.nvim_win_get_cursor(0)[1]
  local cword = vim.fn.expand("<cword>")
  local addr = cword:match("^0[xX]%x+$") and cword

  -- All cursor->function/statement mapping keys on the LAST BUILD's line
  -- tables. Edits shift lines until the next rebuild - detect and SAY it
  -- rather than resolve silently wrong. (Addresses never go stale: with the
  -- cursor on a 0x... the index is bypassed entirely.)
  local stale
  if vim.bo.modified then
    stale = "unsaved buffer edits"
  else
    local src = uv.fs_stat(vim.api.nvim_buf_get_name(0))
    local idx = uv.fs_stat(root .. "/" .. INDEX.base)
    if src and idx and src.mtime.sec > idx.mtime.sec then
      stale = "source saved after the index was built"
    end
  end

  -- an address under the cursor (carcass comments carry target VAs) selects
  -- the function for target/diff views without needing the index
  local selector, entry
  if addr and side ~= "base" then
    selector = { "--va", addr }
  else
    if not relfile then
      return vim.notify("vostok_pdb: buffer is not under <root>/sources/",
        vim.log.levels.ERROR)
    end
    entry = function_at(root, relfile, lnum)
    if not entry then
      return vim.notify("vostok_pdb: no function at this line in the base index",
        vim.log.levels.WARN)
    end
    selector = { "--function", qualified_name(entry) }
  end
  local name = entry and entry.name or addr
  local rec = entry and (load_metrics(root) or {})[entry.mangled]
  local loc, structure = entry and evidence_pair(root, relfile, entry.mangled)

  local function show(args, view, opts)
    local a = side_args(root, side)
    vim.list_extend(a, args)
    run(root, a, function(lines)
      -- full views (not peeks/floats) head with the function's match metrics,
      -- then the stale-cursor warning; floats stay clean.
      if not (opts and opts.float) then
        local header = metric_header(rec, nil, loc, structure)
        if header then table.insert(lines, 1, header) end
        if stale and not addr then
          table.insert(lines, header and 2 or 1,
            "; STALE LINES: " .. stale .. " - cursor mapping may be off; rebuild,")
          table.insert(lines, header and 3 or 2,
            ";              or navigate by address (cursor on a 0x...).")
        end
      end
      local ctx = { root = root, side = side, view = view, name = name,
                    mangled = entry and entry.mangled, relfile = relfile,
                    title = side .. " " .. view, args = a,
                    addr = opts and opts.addr,
                    search = opts and opts.search }
      if opts and opts.float then show_float(lines, ctx)
      else show_split(lines, ctx) end
    end)
  end

  if kind ~= "stmt" then
    local view = VIEW_FLAG[side][kind]
    return show(vim.list_extend(vim.deepcopy(selector), { "--view", view }), view)
  end

  -- stmt: needs the statement at the cursor line, hence the index entry
  if not entry then
    return vim.notify("vostok_pdb: stmt views need the cursor inside a function",
      vim.log.levels.WARN)
  end
  local stmt = stmt_at(entry, lnum)
  if not stmt then
    return vim.notify("vostok_pdb: no statement starts on this line",
      vim.log.levels.WARN)
  end

  if side == "base" then
    local va = va_of(entry, stmt.off)
    show({ "--address", va, "--view", "base" }, "base",
      { float = true, addr = va })
  elseif side == "diff" then
    -- the diff view ignores statement selectors: open it whole and land on
    -- the statement's base offset
    show({ "--function", qualified_name(entry), "--view", "diff" }, "diff",
      { search = ("^[-+ ] ?0x0*%x:"):format(stmt.off) })
  else
    -- target: the two sources disagree on absolute line numbers (the
    -- target file predates ours), but matched functions agree on line
    -- DELTAS from each side's first statement - the same key the
    -- structure-diff's t.ln/b.ln columns use. Pair through that.
    local base_first
    for _, s in ipairs(entry.statements) do
      if s.line and s.line > 0 then base_first = s.line break end
    end
    local want = stmt.line - base_first
    local a = side_args(root, "target")
    vim.list_extend(a, { "--function", qualified_name(entry), "--view", "structure" })
    run(root, a, function(lines)
      local taddr, t_first
      for _, l in ipairs(lines) do
        local address, line = l:match("^(0x%x+)|[^|]*|[^|]*|(%d+)")
        if address then
          t_first = t_first or tonumber(line)
          if tonumber(line) - t_first == want then taddr = address break end
        end
      end
      if not taddr then
        return vim.notify(
          "vostok_pdb: no paired target statement for line " .. stmt.line,
          vim.log.levels.WARN)
      end
      local b = side_args(root, "target")
      vim.list_extend(b, { "--address", taddr, "--view", "target" })
      run(root, b, function(out)
        show_float(out, { root = root, side = "target", view = "target",
                          name = name, addr = taddr })
      end)
    end)
  end
end

--- Side-by-side pair view: TARGET left, BASE right. `kind` is "asm" (function
--- asm - the objdiff look) or "structure" (the statement table). opts.diff turns
--- on a native Neovim diff between the panes (vDa/vDs - changed lines
--- highlighted, folds off); otherwise the panes are just scrollbound (vo).
--- Buffers follow the per-view naming, so rerunning refreshes in place.
function M.view_pair(kind, opts)
  kind = kind or "asm"
  local diff = opts and opts.diff
  local tview = kind == "structure" and "structure" or "target"
  local bview = kind == "structure" and "structure" or "base"

  local root = project_root(0)
  if not root then
    return vim.notify("vostok_pdb: no PDB evidence databases above this file",
      vim.log.levels.ERROR)
  end
  local relfile = rel_source(vim.api.nvim_buf_get_name(0), root)
  if not relfile then
    return vim.notify("vostok_pdb: buffer is not under <root>/sources/",
      vim.log.levels.ERROR)
  end
  local lnum = vim.api.nvim_win_get_cursor(0)[1]
  local entry = function_at(root, relfile, lnum)
  if not entry then
    return vim.notify("vostok_pdb: no function at this line in the base index",
      vim.log.levels.WARN)
  end
  local qn = qualified_name(entry)

  local function fetch(side, view, cb)
    local a = side_args(root, side)
    vim.list_extend(a, { "--function", qn, "--view", view })
    run(root, a, function(lines) cb(lines, a) end)
  end

  -- find-or-make the refreshable named buffer for one pane. In diff mode the
  -- content is normalized (noise stripped) and ctx.normalize tells a post-build
  -- refresh to do the same; vo keeps raw text (addresses live for <CR>).
  local function pane(side, view, lines, args)
    local sym = (entry.name or "?"):gsub("[^%w_:~]+", "."):sub(1, 80)
    local name = ("pdbfetch://%s-%s/%s"):format(side, view, sym)
    local buf = vim.fn.bufnr("^" .. vim.fn.fnameescape(name) .. "$")
    if buf == -1 then
      buf = vim.api.nvim_create_buf(true, true)
      vim.api.nvim_buf_set_name(buf, name)
    end
    local norm = diff and kind or nil
    fill(buf, norm and diff_normalize(lines, norm) or lines,
      { root = root, side = side, view = view, name = entry.name,
        mangled = entry.mangled, relfile = relfile, args = args, normalize = norm })
    return buf
  end

  fetch("target", tview, function(tlines, targs)
    fetch("base", bview, function(blines, bargs)
      local tbuf = pane("target", tview, tlines, targs)
      local bbuf = pane("base", bview, blines, bargs)
      vim.cmd(M.config.split)
      vim.api.nvim_win_set_buf(0, tbuf)
      local twin = vim.api.nvim_get_current_win()
      vim.cmd("rightbelow vsplit")
      vim.api.nvim_win_set_buf(0, bbuf)
      local bwin = vim.api.nvim_get_current_win()
      if diff then
        for _, w in ipairs({ twin, bwin }) do
          vim.api.nvim_win_call(w, function()
            vim.cmd("diffthis")
            vim.wo.foldenable = false -- show the whole function, not just hunks
          end)
          apply_diff_hl(w) -- honor the diff_highlight toggle (color vs none)
        end
      else
        vim.wo[twin].scrollbind = true
        vim.wo[bwin].scrollbind = true
        vim.api.nvim_win_call(bwin, function() vim.cmd("syncbind") end)
      end
      vim.api.nvim_set_current_win(twin)
    end)
  end)
end

--- The V peek. Inside plugin views the side comes from the table column /
--- diff prefix (follow). In source buffers, carcass stubs annotate each
--- statement with its TARGET VA (`// <0xVA>|...`) - peek that statement's
--- target asm; lines without an annotation peek the base statement.
function M.peek()
  if vim.b.vostok_pdb then return M.follow() end
  local tva = vim.api.nvim_get_current_line():match("<(0[xX]%x+)>")
  if not tva then return M.view("base", "stmt") end
  local root = project_root(0)
  if not root then
    return vim.notify("vostok_pdb: no PDB evidence databases above this file",
      vim.log.levels.ERROR)
  end
  local a = side_args(root, "target")
  vim.list_extend(a, { "--address", tva, "--view", "target" })
  run(root, a, function(out)
    show_float(out, { root = root, side = "target", view = "target",
                      name = tva, addr = tva })
  end)
end

-- ------------------------------------------------------------------ build --
-- `python3 -m vostok build` is the canonical refresh: build, structure/COFF/PDB
-- evidence regeneration, report, and ledger. It is intentionally a full build.

--- Replace a view buffer's content in place, preserving every showing window's
--- cursor/scroll (winrestview clamps if the line count shrank) so a refresh
--- doesn't move you.
local function set_buf_lines(buf, lines)
  local views = {}
  for _, w in ipairs(vim.fn.win_findbuf(buf)) do
    views[w] = vim.api.nvim_win_call(w, vim.fn.winsaveview)
  end
  vim.bo[buf].modifiable = true
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines)
  vim.bo[buf].modifiable = false
  for w, v in pairs(views) do
    if vim.api.nvim_win_is_valid(w) then
      vim.api.nvim_win_call(w, function() vim.fn.winrestview(v) end)
    end
  end
end

--- Re-run the vostok_pdb query behind every open split view for `root` and
--- replace its content, so a build updates open asm/structure/diff views in
--- place. Keyed by the args stashed in each view's ctx; transient floats skip.
local function refresh_views(root)
  local by = load_metrics(root)
  for _, buf in ipairs(vim.api.nvim_list_bufs()) do
    local ctx = vim.api.nvim_buf_is_loaded(buf) and vim.b[buf].vostok_pdb or nil
    if ctx and ctx.root == root and ctx.args then
      local rec = ctx.mangled and by and by[ctx.mangled]
      local loc, structure = evidence_pair(root, ctx.relfile, ctx.mangled)
      run(root, ctx.args, function(lines)
        if ctx.normalize then -- a vDa/vDs diff pane: re-strip the noise, no header
          lines = diff_normalize(lines, ctx.normalize)
        else
          local header = metric_header(rec, nil, loc, structure)
          if header then table.insert(lines, 1, header) end
        end
        if vim.api.nvim_buf_is_valid(buf) then set_buf_lines(buf, lines) end
      end)
    end
  end
end

-- ---- the build-result corner popup ----

local function pct1(x) return x and string.format("%.1f%%", x) or "-" end

--- Code-weighted overall fuzzy % over all target functions (unpaired count 0),
--- mirroring match_score.py's headline so the popup's number is comparable.
local function overall_fuzzy(by)
  local num, den = 0, 0
  for _, r in pairs(by or {}) do
    den = den + (r.size or 0)
    num = num + (r.cur or 0) * (r.size or 0)
  end
  return den > 0 and num / den or 0
end

--- A readable Class::method from a demangled signature (params + return type
--- stripped); falls back to the mangled name.
local function fn_short(rec, mangled)
  local dem = rec and rec.dem or ""
  local head = dem:match("^(.-)%(") or dem               -- drop param list
  local short = head:match("([%w_~<>]+::[%w_~<>]+)%s*$")  -- Class::method
             or head:match("([%w_~<>]+)%s*$")             -- or bare name
  return (short or mangled or "?"):sub(1, 46)
end

--- A non-focusable corner popup summarising the build: overall % before->after
--- and the functions whose % moved (capped). Fades, never steals focus (it may
--- land while you are typing elsewhere). The full table lives in match_score.py.
local function build_popup(root, before, elapsed)
  local after = load_metrics(root)
  if not after then return vim.notify("rebuild: done (no ledger to compare)") end
  before = before or {}
  local lines = { ("overall  %s -> %s")
    :format(pct1(overall_fuzzy(before)), pct1(overall_fuzzy(after))) }
  local moved = {}
  for m, r in pairs(after) do
    local was = before[m] and before[m].cur
    if (was or 0) ~= (r.cur or 0) then
      moved[#moved + 1] = { m = m, rec = r, was = was, now = r.cur,
                            d = (r.cur or 0) - (was or 0) }
    end
  end
  table.sort(moved, function(a, b) return a.d > b.d end)
  if #moved == 0 then
    lines[#lines + 1] = "(no function % changes)"
  else
    for i = 1, math.min(#moved, 8) do
      local r = moved[i]
      local arr = r.was == nil and "+" or r.d > 0 and "^" or r.d < 0 and "v" or " "
      lines[#lines + 1] = ("  %s %-7s -> %-7s %s")
        :format(arr, pct1(r.was), pct1(r.now), fn_short(r.rec, r.m))
    end
    if #moved > 8 then
      lines[#lines + 1] = ("  +%d more (:VostokRebuild / match_score.py)")
        :format(#moved - 8)
    end
  end

  local width = 0
  local title = elapsed and ((" rebuild OK  %.1fs "):format(elapsed)) or " rebuild OK "
  for _, l in ipairs(lines) do width = math.max(width, #l, #title) end
  local buf = vim.api.nvim_create_buf(false, true)
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, lines)
  vim.bo[buf].bufhidden = "wipe"
  local win = vim.api.nvim_open_win(buf, false, {
    relative = "editor", anchor = "NE", row = 1, col = vim.o.columns - 2,
    width = math.min(width + 1, vim.o.columns - 4), height = #lines,
    style = "minimal", border = "single", focusable = false, noautocmd = true,
    title = title, title_pos = "left",
  })
  vim.defer_fn(function()
    if vim.api.nvim_win_is_valid(win) then vim.api.nvim_win_close(win, true) end
  end, 6000)
end

--- A small persistent "rebuilding ..." corner note; returns a closer.
local function show_note(text)
  local buf = vim.api.nvim_create_buf(false, true)
  vim.bo[buf].bufhidden = "wipe"
  vim.api.nvim_buf_set_lines(buf, 0, -1, false, { " " .. text .. " " })
  local win = vim.api.nvim_open_win(buf, false, {
    relative = "editor", anchor = "NE", row = 1, col = vim.o.columns - 2,
    width = #text + 2, height = 1, style = "minimal", border = "single",
    focusable = false, noautocmd = true,
  })
  return function()
    if vim.api.nvim_win_is_valid(win) then vim.api.nvim_win_close(win, true) end
  end
end

-- One build at a time, latest-wins: a new request kills the in-flight build and
-- starts fresh (rapid saves; also avoids two rebuild.py runs racing on binaries/).
-- build_gen tags each request so a superseded job's on_exit bails out.
local build_job, build_gen, build_note = nil, 0, nil

local function finish_build(root, before, code, elapsed, quiet)
  if build_note then pcall(build_note); build_note = nil end
  if code ~= 0 then
    return vim.notify("rebuild: FAILED (exit " .. code .. ")", vim.log.levels.ERROR)
  end
  for k in pairs(cache) do cache[k] = nil end -- line tables moved with the build
  invalidate_metrics(root)
  if quiet then
    build_popup(root, before, elapsed)
  else
    vim.notify(("rebuild: done%s - views are fresh")
      :format(elapsed and ((" (%.1fs)"):format(elapsed)) or ""))
  end
  refresh_all_hints() -- the inline %s now reflect the new build
  refresh_views(root) -- and any open asm/diff/structure views re-render in place
end

local function do_build(root, args, quiet)
  if build_job then pcall(vim.fn.jobstop, build_job); build_job = nil end
  if build_note then pcall(build_note); build_note = nil end
  build_gen = build_gen + 1
  local my_gen = build_gen
  local t0 = uv.hrtime()
  local before = load_metrics(root)
  local cmd = { "python3", "-m", "vostok", "build", "--foreground" }
  vim.list_extend(cmd, args or {})
  log("rebuild" .. (quiet and " (on save)" or "") .. ": " ..
    table.concat(cmd, " ") .. "  [" .. root .. "]")
  local function on_exit(_, code)
    vim.schedule(function()
      if my_gen ~= build_gen then return end -- superseded by a newer build
      build_job = nil
      finish_build(root, before, code, (uv.hrtime() - t0) / 1e9, quiet)
    end)
  end
  if quiet then
    build_note = show_note("rebuilding ...")
    build_job = vim.fn.jobstart(cmd, { cwd = root, on_exit = on_exit })
  else
    -- a terminal split with the live build log (manual :VostokRebuild / vB)
    vim.cmd(M.config.split)
    local buf = vim.api.nvim_create_buf(false, true)
    vim.api.nvim_win_set_buf(0, buf)
    vim.fn.termopen(cmd, { cwd = root, on_exit = on_exit })
  end
end

--- :VostokRebuild [args] / vB - manual rebuild in a terminal split, then the
--- views refresh + a popup of what moved. Extra args go to rebuild.py
--- (e.g. :VostokRebuild logging builds one project first).
function M.rebuild(args)
  local root = project_root(0)
  if not root then
    return vim.notify("vostok_pdb: no PDB evidence databases above this file",
      vim.log.levels.ERROR)
  end
  do_build(root, args or {}, false)
end

--- BufWritePost hook: when build-on-save is on, kick a quiet incremental
--- rebuild so the inline %s (and open views) update right after you save.
function M.on_save(buf)
  if not M.config.build_on_save then return end
  local root = project_root(buf)
  if root then do_build(root, {}, true) end
end

--- Close every open vostok_pdb view window (splits, peeks, the side-by-side vo).
function M.close()
  for _, w in ipairs(vim.api.nvim_list_wins()) do
    if vim.api.nvim_win_is_valid(w) and vim.api.nvim_buf_get_name(
        vim.api.nvim_win_get_buf(w)):match("^pdbfetch://") then
      pcall(vim.api.nvim_win_close, w, true)
    end
  end
end

-- ------------------------------------------------------ toggles / dispatch --
-- The hints and build-on-save toggles are remembered per checkout under
-- binaries/ (already gitignored), so each worktree keeps its own setting across
-- restarts; a remembered toggle wins over the setup default. Loaded once per root.
local loaded_roots = {}
local function state_path(root) return root .. "/binaries/vostok_pdb-nvim.json" end

local function save_state(root)
  if not root then return end
  vim.fn.mkdir(root .. "/binaries", "p")
  local fd = io.open(state_path(root), "w")
  if not fd then return end
  fd:write(vim.json.encode({ hints = M.config.hints,
                             build_on_save = M.config.build_on_save,
                             diff_highlight = M.config.diff_highlight }))
  fd:close()
end

--- Apply a checkout's persisted toggle state over the current config. Loads once
--- per root; call with a buffer that lives in that checkout.
function M.load_state(buf)
  local root = project_root(buf or 0)
  if not root or loaded_roots[root] then return end
  loaded_roots[root] = true
  local fd = io.open(state_path(root), "r")
  if not fd then return end
  local ok, s = pcall(vim.json.decode, fd:read("*a"))
  fd:close()
  if ok and type(s) == "table" then
    if type(s.hints) == "boolean" then M.config.hints = s.hints end
    if type(s.build_on_save) == "boolean" then M.config.build_on_save = s.build_on_save end
    if type(s.diff_highlight) == "boolean" then M.config.diff_highlight = s.diff_highlight end
  end
end

--- The single-argument :Vostok forms (toggles + close); returns true if `arg`
--- was one of them, so the command knows it wasn't a {side} {kind} view.
function M.dispatch(arg)
  if arg == "hints" then
    M.config.hints = not M.config.hints
    save_state(project_root(0))
    refresh_all_hints()
    vim.notify("vostok_pdb: inline metrics " .. (M.config.hints and "on" or "off"))
  elseif arg == "autobuild" then
    M.config.build_on_save = not M.config.build_on_save
    save_state(project_root(0))
    vim.notify("vostok_pdb: build on save " ..
      (M.config.build_on_save and "ON" or "off"))
  elseif arg == "diffhl" then
    -- flip vDa/vDs coloring and apply it live to any open diff panes
    M.config.diff_highlight = not M.config.diff_highlight
    save_state(project_root(0))
    for _, w in ipairs(vim.api.nvim_list_wins()) do
      if vim.api.nvim_win_is_valid(w) and vim.wo[w].diff
          and vim.api.nvim_buf_get_name(
            vim.api.nvim_win_get_buf(w)):match("^pdbfetch://") then
        apply_diff_hl(w)
      end
    end
    vim.notify("vostok_pdb: diff highlighting " ..
      (M.config.diff_highlight and "on" or "off (aligned, no color)"))
  elseif arg == "close" then
    M.close()
  else
    return false
  end
  return true
end

local warned_missing
--- Startup check, called when a c/cpp buffer attaches: warn (once) if the
--- buffer belongs to a project but the vostok-pdb CLI is not on PATH - the
--- usual cause is nvim launched outside the project dev shell. Outside a
--- project the plugin is inert and stays silent.
function M.check(buf)
  if warned_missing or not project_root(buf) then return end
  if vim.fn.executable("vostok-pdb") == 0 then
    warned_missing = true
    vim.notify(
      "vostok-pdb.nvim: `vostok-pdb` not on PATH - launch nvim from the " ..
      "project dev shell (nix develop) for :Vostok views to work",
      vim.log.levels.WARN)
  end
end

-- completion for :Vostok - first arg is a side or a single-arg form; only the
-- sides take a second {stmt|asm|structure} argument.
function M.complete(_, cmdline)
  local first = cmdline:match("Vostok%s+(%S+)%s+%S*$")
  if first then
    if first == "base" or first == "target" or first == "diff" then
      return { "stmt", "asm", "structure" }
    end
    return {}
  end
  return { "base", "target", "diff", "hints", "autobuild", "diffhl", "close" }
end

function M.attach_keymaps(buf)
  -- sushi doesn't use visual mode: plain v-prefixed chords, and V itself
  -- peeks the current statement's asm (base in source; in plugin views the
  -- side comes from the table column / diff line prefix).
  local maps = {
    vbs = { "base", "structure" },
    vts = { "target", "structure" },
    vds = { "diff", "structure" },
    -- f (function) and a (asm) both work for the asm views
    vbf = { "base", "asm" },
    vtf = { "target", "asm" },
    vdf = { "diff", "asm" },
    vba = { "base", "asm" },
    vta = { "target", "asm" },
    vda = { "diff", "asm" },
  }
  for lhs, sv in pairs(maps) do
    vim.keymap.set("n", lhs, function() M.view(sv[1], sv[2]) end,
      { buffer = buf, silent = true,
        desc = ("vostok_pdb: %s %s"):format(sv[1], sv[2]) })
  end
  vim.keymap.set("n", "V", function() M.peek() end,
    { buffer = buf, silent = true, desc = "vostok_pdb: statement asm peek" })
  vim.keymap.set("n", "vo", function() M.view_pair("asm") end,
    { buffer = buf, silent = true,
      desc = "vostok_pdb: target|base asm side by side (objdiff look)" })
  -- vDa/vDs: target|base in two windows as a native diff (asm / structure)
  vim.keymap.set("n", "vDa", function() M.view_pair("asm", { diff = true }) end,
    { buffer = buf, silent = true,
      desc = "vostok_pdb: target|base asm diff, two windows" })
  vim.keymap.set("n", "vDs", function() M.view_pair("structure", { diff = true }) end,
    { buffer = buf, silent = true,
      desc = "vostok_pdb: target|base structure diff, two windows" })
  vim.keymap.set("n", "vB", function() M.rebuild({}) end,
    { buffer = buf, silent = true, desc = "vostok_pdb: rebuild (build + regen)" })
end

--- Optional config override (keymaps / hints / build_on_save / split). A
--- per-checkout remembered toggle still wins over these defaults (load_state).
function M.setup(opts)
  M.config = vim.tbl_deep_extend("force", M.config, opts or {})
end

return M
