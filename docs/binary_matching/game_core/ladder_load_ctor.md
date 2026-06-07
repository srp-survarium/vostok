# ladder::load and ladder::ladder (ctor) - now 100%

Both functions previously carried PARTIAL-style STATE markers blaming LTCG:
- load: `STATE[87.95%|DONE]: LTCG for value_exists and malloc`
- ctor: `STATE[89.26%|DONE]: LTCG for resources::managed_resource`

## Current state (report.json)
- `?load@ladder@survarium@@UAEXABVbinary_config_value@configs@vostok@@@Z` -> 100.0
- `??0ladder@survarium@@QAE@ABV?$resource_ptr@...@@ABVplane@math@4@@Z` -> 100.0

## structure-diff
load: target 6 / base 6 stmts, aligned 6, size-diffs 0, quantity-diffs 0.
ctor (rva 0x5866c0): target 0 / base 0 stmts (init-list attributed to decl line),
aligned 0, size-diffs 0, quantity-diffs 0.

## Why they are now 100%
The earlier walls (value_exists / malloc inline-vs-call in load; managed_resource
resource_ptr copy in the ctor init-list) were whole-program LTCG inline decisions
that depended on which consumers were matched. Since the markers were written, the
dependent callers / surrounding TU state changed so the linker now lowers these the
same way the target does, closing the residuals. Both verify at 100% structurally
and by fuzzy %.

## Changes made
- Synced both STATE markers to bare `// STATE[100%|DONE]`.
- Deleted the `// FUNCTION BODY` carcasses (load + dtor) per the clean-100% rule.
- load: refined the ASSERT to `ASSERT( UNKNOWN_EXPRESSION_T( !m_occluder ) )` - the
  `_T` form is byte-identical (both expand to `true`) and documents the guess (the
  load-once guard). Rebuild confirmed load stays 100%.
