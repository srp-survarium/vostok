# Instance member scores None: access letter Q/A/I after @@ (2nd letter B = const this)
tags: cpp:member | topic:mangling
symptoms: scores None, @@QBE, @@ABE, @@AAE, @@IBE, fuzzy_match_percent None for a compiled function
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
Evidence: get_target_koef / get_broken_hands_penalty are ABE - moving them to `private:` flipped both from None to scored.
