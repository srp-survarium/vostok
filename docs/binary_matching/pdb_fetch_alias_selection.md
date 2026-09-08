<!-- SPDX-License-Identifier: GPL-3.0-or-later -->

# pdb_fetch ICF alias-selection defect

Observed during PR 571 with parser pin
`6262ce150b12729b865a7eca6d82ad563256ba20` and the existing unrebuilt indexes.
Source baseline: `561b8cf02`. The concrete example is
`survarium::player_input_handler::input_priority()`.

## Evidence

| Query | Selected function | RVA | Body |
|---|---|---:|---|
| Target only | `player_input_handler::input_priority` | `0x868f0` | `mov eax,10; ret` |
| Base only | `player_input_handler::input_priority` | `0xbc470` | `xor eax,eax; ret` |
| Both indexes, base view | `vostok::assert_log_times_in_gold` | `0x982a0` | `mov eax,10; ret` |

The two-sided `--view diff` explicitly labels those different functions and
reports **2/2 instructions equal (100.0%)**. It is comparing the wrong candidate,
not proving the requested source or virtual override correct. The source
literally returned zero, and the base-only body confirms that result.

Reproduce against the recorded pre-fix artifact state:

```sh
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --function 'player_input_handler::input_priority(' --view target
pdb_fetch --base-index binaries/rich/base/index.jsonl \
  --function 'player_input_handler::input_priority(' --view base
pdb_fetch --target-index binaries/rich/target/index.jsonl \
  --base-index binaries/rich/base/index.jsonl \
  --function 'player_input_handler::input_priority(' --view diff
```

The source is now corrected to 10. After the deferred rebuild, this particular
mismatch may disappear, so retain the pre-fix facts above for a parser regression
fixture rather than assuming future artifacts still reproduce it.

## Working rule and required tool correction

For an ICF-prone accessor, virtual stub or tiny constant return, fetch target
and base **independently** and verify function identity before trusting a
two-sided equality. A structure match alone never proves bytes, and a printed
byte equality is also insufficient if the selected function is wrong.

The parser needs to prefer an available candidate for the explicitly requested
function identity over a different retail fold alias. Add a regression case
where the target folds requested A with B, but candidate A differs and candidate
B still matches; requesting A must expose its mismatch. Also retain a separate
case where A genuinely has no candidate body and alias fallback is appropriate.
That parser change is **not yet implemented**; no tool build was run here.
