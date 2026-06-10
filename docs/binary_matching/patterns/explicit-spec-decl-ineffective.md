# Explicit-specialization DECLARATION does not stop MSVC8 inlining a visible template (negative result)
tags: cpp:template cpp:inline | topic:inline-vs-call
symptoms: template<> declaration added, % unchanged, operator+ still inlined
confidence: 4/10
variants: mixing-expression-operator-plus.md

Adding `template<> R operator+<T,T>(...);` in the consuming .cpp does NOT make MSVC8 call
the standalone specialization when the primary inline template's DEFINITION is in scope
(via the included `_inline.h`) - it still inlines (weapon_and_hands_expression 85.65 ->
85.65). The forward-decl device works ONLY with the inline definition OUT of the TU (drop
the inline-header include + forward-decl the function AND its types) - a TU restructure
usually shared across sibling .cpp (collateral risk).
