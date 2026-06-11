# LOG_* sites: pushed verbosity picks the macro; pushed __LINE__ pins the layout (do NOT pad)
tags: cpp:macro | asm:push | topic:logging
symptoms: push 2 push 3 push 4 verbosity twice, push 0ACh line immediate, __LINE__
confidence: 9/10
variants: log-file-path-wall.md, log-callback-ctor-schedule.md

A `__LOG`-macro site pushes the verbosity TWICE (has_passed_filters + append): `push 2` =
LOG_ERROR, `push 3` = LOG_WARNING, `push 4` = LOG_INFO (logging/api.h: silent=1, error=2,
warning=3, info=4, debug=5, trace=6). Pick the macro from the pushed literal, NOT the
message tone. The site also pushes `__LINE__` as an immediate; VS2008 takes it (and the
/Od line-table entry) from the CLOSING-PAREN line of a multi-line invocation.

```asm
push 2          ; LOG_ERROR (caught: "disconnection initiated..." sounded like a warning, target pushes 2)
push 0ACh       ; __LINE__ = 172
```
**Do NOT pad the .cpp to land __LINE__ (sushi)** - accept the few-byte residual and note it (`__LINE__ immediate, N sites; +1 size class if the true line crosses the imm8/imm32 boundary at 128`). Padding is line-FRAGILE (proven by udp_match_client.cpp pin churn); that file's pre-existing pins (97/103/109 + 172) stay while it lives, but create no new ones.
Evidence: udp_match_client::enqueue (push 3 vs push 2 caught).
