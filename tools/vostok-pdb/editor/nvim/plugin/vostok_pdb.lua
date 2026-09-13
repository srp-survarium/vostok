if vim.g.loaded_vostok_pdb then return end
vim.g.loaded_vostok_pdb = true

vim.api.nvim_create_user_command("Vostok", function(opts)
  local pf = require("vostok_pdb")
  local a1, a2 = opts.fargs[1], opts.fargs[2]
  -- single-arg forms: toggles + close (hints / autobuild / close)
  if a1 and not a2 and pf.dispatch(a1) then return end
  local ok_side = a1 == "base" or a1 == "target" or a1 == "diff"
  local ok_kind = a2 == "stmt" or a2 == "asm" or a2 == "structure"
  if not (ok_side and ok_kind) then
    return vim.notify(
      "usage: :Vostok {base|target|diff} {stmt|asm|structure}" ..
      "  |  :Vostok {hints|autobuild|close}", vim.log.levels.ERROR)
  end
  pf.view(a1, a2)
end, {
  nargs = "+",
  complete = function(arglead, cmdline)
    return vim.tbl_filter(function(c) return vim.startswith(c, arglead) end,
      require("vostok_pdb").complete(arglead, cmdline))
  end,
  desc = "vostok_pdb views / toggles for the function at cursor",
})

vim.api.nvim_create_user_command("VostokLog", function()
  require("vostok_pdb").show_log()
end, { desc = "show recent vostok_pdb invocations + resolved binary path" })

vim.api.nvim_create_user_command("VostokRebuild", function(opts)
  require("vostok_pdb").rebuild(opts.fargs)
end, { nargs = "*", desc = "run vostok build, then refresh views + hints" })

local group = vim.api.nvim_create_augroup("vostok_pdb", { clear = true })

-- On a c/cpp buffer: warn if the CLI is missing, load the per-checkout toggle
-- state, attach the chords, and render the inline match metrics.
vim.api.nvim_create_autocmd("FileType", {
  pattern = { "c", "cpp" },
  group = group,
  callback = function(ev)
    local pf = require("vostok_pdb")
    pf.check(ev.buf)
    pf.load_state(ev.buf)
    if pf.config.keymaps then pf.attach_keymaps(ev.buf) end
    pf.hints(ev.buf)
  end,
})

-- Re-render hints when a buffer becomes visible (a build in another window may
-- have moved the %s) ...
vim.api.nvim_create_autocmd("BufEnter", {
  pattern = { "*.c", "*.cpp", "*.cc", "*.cxx", "*.h", "*.hpp", "*.inl" },
  group = group,
  callback = function(ev) require("vostok_pdb").hints(ev.buf) end,
})

-- ... and on save: re-anchor the hints to the (possibly shifted) lines, and kick
-- a quiet rebuild when build-on-save is on.
vim.api.nvim_create_autocmd("BufWritePost", {
  pattern = { "*.c", "*.cpp", "*.cc", "*.cxx", "*.h", "*.hpp", "*.inl" },
  group = group,
  callback = function(ev)
    local pf = require("vostok_pdb")
    pf.hints(ev.buf)
    pf.on_save(ev.buf)
  end,
})
