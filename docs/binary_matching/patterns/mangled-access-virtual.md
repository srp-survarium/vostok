# Virtual member access codes after @@: U=public, M=protected, E=private virtual
tags: cpp:virtual cpp:member | topic:mangling
symptoms: @@UAE, @@MAE, @@EAE, @@EBE, scores None for a virtual override
confidence: 9/10
variants: mangled-access-instance.md, mangled-access-static-fn.md, mangled-join-failure.md

For virtuals the char after `@@` encodes access AND virtual-ness: U=public virtual,
M=protected virtual, E=private virtual (then AE=__thiscall, BE=const). A virtual override
may legally be `private:` (dispatch ignores access) - the target often privatizes
overrides the base declares public.

```cpp
private:    // ?initialize@...@@EAEXXZ - private virtual
	virtual void initialize( );
	virtual void finalize( );
```
To anchor private virtuals non-virtually from a free `use_*`, befriend the anchor in the class (free decls/friends emit no bytes).
Evidence: weapon_core_idle_state_base::{initialize,finalize} EAEXXZ - `private:` flipped both None->100; the non-virtual ctor in the same class was IAE (protected) -> 100.
