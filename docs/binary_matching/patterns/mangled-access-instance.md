# Instance member scores None: access letter Q/A/I after @@ (2nd letter B = const this)
tags: cpp:member | topic:mangling
symptoms: scores None, unpaired, @@QBE, @@ABE, @@AAE, @@IAE, @@IBE, fuzzy_match_percent None/unpaired for a compiled function, base obj has @@QAE while target obj has @@IAE
confidence: 9/10
variants: mangled-access-virtual.md, mangled-access-static-fn.md, mangled-access-static-data.md, mangled-join-failure.md

objdiff pairs base<->target by EXACT mangled name, and the access specifier is baked in
right after `@@` for instance members: Q=public, I=protected, A=private; 2nd letter
A=non-const `this`, B=const `this`. One wrong letter = silent pairing failure = None even
with byte-identical code. Read the target obj's symbol table and set the declaration's
access to match.

```cpp
private:    // target mangles ?get_target_koef@...@@ABE... - ABE = private, const this
	float get_target_koef( ) const;
```
Evidence: get_target_koef / get_broken_hands_penalty are ABE - moving them to `private:` flipped both from None to scored. weapon_core_shotgun_reload_base_substate ctor was QAE (public) but target IAE (protected) and get_weapon_lexeme_pair was QBE but target ABE (private) - both showed "unpaired" in report.json (the obj symbol tables differed only in the access letter); fixing the header access flipped both 100%. NOTE: top-level `const` on by-value params (e.g. `float const`) does NOT change the mangled name - it won't fix pairing (objdiff demangles it away too), only the access letter does.
