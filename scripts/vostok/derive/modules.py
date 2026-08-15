"""vostok.derive.modules - which module owns a function.

Ownership is a source question, not a link-layout one, and ICF makes the two
disagree: the linker folds identical bodies, so the PDB's owner for a folded
body can be any of the TUs that contributed it. `logical_module` therefore
resolves in precedence order - a reviewed override first, then the dynamic
initializer's real owner, then the unit/file the record names.
"""

from vostok.core import tsv
from vostok.core.paths import MODULE_OWNERSHIP_OVERRIDES

from vostok.derive.aliases import (_LOCAL_FUNCTION_SCOPE_RE, dyn_canon_base, dyn_canon_rich)


def module_of(unit_or_file):
    parts = unit_or_file.split("/")
    if parts[0] == "vostok" and len(parts) > 2:
        return parts[1]
    return parts[0]  # third-party root, or a shared vostok/*.h -> "vostok"


def dynamic_local_owner_modules(records):
    """Resolve function-local static thunks through their enclosing PDB owner.

    ICF may attribute the thunk body to an unrelated inline header. Its rich
    name still preserves the complete enclosing function scope and local-scope
    ordinal. When that enclosing function has one source owner in the target
    PDB, that source module is the thunk's logical owner.
    """
    scopes_by_canon = {}
    for rec in records:
        canon = dyn_canon_rich(rec["mangled"])
        if not canon:
            continue
        match = _LOCAL_FUNCTION_SCOPE_RE.match(canon[1])
        if match:
            scopes_by_canon[canon] = match.group(1)

    files_by_scope = {}
    for scope in set(scopes_by_canon.values()):
        marker = f"{scope}("
        files = {
            rec["file"] for rec in records
            if marker in (rec.get("name") or "") and rec.get("file")
        }
        if len(files) == 1:
            files_by_scope[scope] = next(iter(files))

    return {
        canon: module_of(files_by_scope[scope])
        for canon, scope in scopes_by_canon.items()
        if scope in files_by_scope
    }


def load_module_ownership_overrides(path=MODULE_OWNERSHIP_OVERRIDES):
    """Load reviewed mangled-name owners for PDB records defeated by ICF."""
    if not path.is_file():
        return {}
    overrides = {}
    for number, fields in tsv.read(path, strip=False):
        if len(fields) != 3:
            raise ValueError(f"{path}:{number}: expected symbol, module, source file")
        mangled, module, source_file = fields
        if not mangled or not module or module_of(source_file) != module:
            raise ValueError(f"{path}:{number}: inconsistent module ownership row")
        if mangled in overrides:
            raise ValueError(f"{path}:{number}: duplicate symbol {mangled}")
        overrides[mangled] = module
    return overrides


def logical_module(mangled, rec, units, dynamic_owners, overrides):
    """Return logical source ownership, independent of an ICF body owner."""
    override = overrides.get(mangled)
    if override:
        return override
    canon = dyn_canon_rich(mangled) or dyn_canon_base(mangled)
    if canon and canon in dynamic_owners:
        return dynamic_owners[canon]
    unit_or_file = rec["file"] if units and rec["file"] in units else (
        units[0] if units else rec["file"]
    )
    return module_of(unit_or_file)
