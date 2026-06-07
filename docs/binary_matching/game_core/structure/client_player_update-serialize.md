# client_player_update::serialize - structure audit

Signature: `void survarium::client_player_update::serialize(vostok::network_core::udp_match_packet&) const`
target rva 0x771190 | base rva 0x44d2e0
report.json fuzzy_match_percent: 32.57% (mangled
`?serialize@client_player_update@survarium@@QBEXAAVudp_match_packet@network_core@vostok@@@Z`)

## Verdict

STRUCTURE MISMATCH (quantity) - EXPECTED, function is correctly tagged BLOCKED.

The base is an empty stub (prologue/epilogue only): the body cannot be written
because the `udp_match_packet` type cluster does not compile yet
(`network_core/udp_match_packet.h`). The body is deliberately left empty so the
module keeps building. This is a BLOCKED, not a mislabeled DONE - no downgrade
needed (it was never marked DONE), no source restructure available until the
blocker clears.

## Skeletons (condensed structure-diff)

```
target: 0x771190            base: 0x44d2e0
; ... serialize(...) const ; target 4 stmts / base 0 stmts
<0>         | --          |       EMPTY only target
0x009 <0xc> | --          | L23   ONLY target   input.serialize( packet );
0x015 <0xf> | --          | L24   ONLY target   state.serialize( packet );
0x024 <0xf> | --          | L25   ONLY target   packet.append( time_in_ms );
; aligned 0, size-diffs 0, quantity-diffs 4
```

Target body (3 statements, from raw disassembly):

| off   | size | srcline | statement                          | call                                                     |
|-------|------|---------|------------------------------------|----------------------------------------------------------|
| 0x009 | 0xc  | L23     | `input.serialize( packet );`       | `player_input::serialize` (this+0x00)                    |
| 0x015 | 0xf  | L24     | `state.serialize( packet );`       | `player_state::serialize` (this+0x14)                    |
| 0x024 | 0xf  | L25     | `packet.append( time_in_ms );`     | `packet<udp_match_packet>::append( u32 )`, time at +0x58 |

Base body: none (stub).
```
0x00: push ebp / mov ebp,esp / push ecx / mov [ebp-4],ecx / mov esp,ebp / pop ebp / ret 4
```

## Divergences

All 4 are QUANTITY diffs, every one `ONLY target` (plus one `EMPTY only target`
source-line gap). There are zero SIZE diffs and zero ORDER issues - because the
base contributes no statements to compare against. The 32.57% in report.json is
pure prologue/epilogue overlap over a 57-byte target; the percent is high enough
to look like partial progress but the entire functional body is absent. This is
exactly the "high % over the wrong/absent structure" trap, here legitimately
gated behind a compile blocker rather than a coding error.

## Audit result / fix

Nothing new found beyond what the STATE[BLOCKED] note already records. The
documented 3-statement plan (`input.serialize`, `state.serialize`,
`packet.append( time_in_ms )`) matches the target disassembly exactly in count
and order. No source change to the body is possible or warranted now.

Fix to apply when the blocker clears (udp_match_packet cluster compiles):
write the three body statements in order - `input.serialize( packet );`,
`state.serialize( packet );`, `packet.append( time_in_ms );` - then rebuild and
re-diff. Unblocks together with `player_input::serialize`,
`player_state::serialize`, `inventory_item::serialize` on the same blocker.

The one-sided `// FUNCTION BODY` carcass in the .cpp has been replaced with the
condensed structure-diff per the structure-verifier instructions. STATE line
kept as BLOCKED (no logic change, no rebuild).
