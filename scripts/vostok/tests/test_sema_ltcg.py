# SPDX-License-Identifier: GPL-3.0-or-later

"""vostok.sema.ltcg: the pure helpers that tag an LTCG residual.

Instruction texts below are taken from the ltcg-lab probes and retail bodies
quoted in docs/binary_matching/ltcg.md.
"""

import unittest

from vostok.sema.ltcg import (_qualified_name, call_sites, entry_registers, flag_class,
                              frame_size, framed, owner_hint, reg_canonical, ret_bytes,
                              trust_class, unmatched_calls)


class EntryRegisterTests(unittest.TestCase):
    def test_standard_thiscall_reads_only_ecx(self):
        body = ["push esi", "mov esi, ecx", "call f", "ret"]
        self.assertEqual(entry_registers(body), {"ecx"})

    def test_ltcg_this_in_eax(self):
        # retail weapon_core::is_aimed
        self.assertEqual(entry_registers(["mov al, [eax+488h]", "ret"]), {"eax"})

    def test_saved_callee_registers_are_not_arguments(self):
        body = ["push ebx", "push esi", "mov esi, [esp+0Ch]", "mov eax, esi", "ret"]
        self.assertEqual(entry_registers(body), set())

    def test_argument_in_esi_is_live(self):
        # a3 under /LTCG: lea eax,[esi*8]; sub eax,esi ...
        body = ["lea eax, [esi*8+0]", "sub eax, esi", "lea edx, [edx+edx*4]", "ret"]
        self.assertEqual(entry_registers(body), {"esi", "edx"})

    def test_xor_self_is_a_write(self):
        self.assertEqual(entry_registers(["xor eax, eax", "add eax, ecx", "ret"]), {"ecx"})


class RetAndFrameTests(unittest.TestCase):
    def test_ret_bytes(self):
        self.assertEqual(ret_bytes(["ret 4"]), 4)
        self.assertEqual(ret_bytes(["ret 0Ch", "ret 0Ch"]), 12)
        self.assertEqual(ret_bytes(["ret"]), 0)
        self.assertIsNone(ret_bytes(["jmp f"]))

    def test_framed_and_frame_size(self):
        od = ["push ebp", "mov ebp, esp", "sub esp, 130h", "ret"]
        self.assertTrue(framed(od))
        self.assertEqual(frame_size(od), 0x130)
        self.assertFalse(framed(["push esi", "mov esi, ecx", "ret"]))

    def test_register_renaming_equivalence(self):
        a = ["mov eax, [ecx+4]", "add eax, 1", "ret"]
        b = ["mov edx, [eax+4]", "add edx, 1", "ret"]
        self.assertEqual(reg_canonical(a), reg_canonical(b))


class CallSiteTests(unittest.TestCase):
    def test_pushed_register_is_a_stack_argument(self):
        sites = call_sites(["mov eax, [ebx]", "push eax", "lea ecx, [esi+4]", "call f"])
        self.assertEqual(sites, [("f", 1, {"ecx"})])

    def test_register_argument_after_call(self):
        sites = call_sites(["call f", "mov edx, 1", "push 3", "call g"])
        self.assertEqual(sites[1], ("g", 1, {"edx"}))

    def test_icf_equal_callees_pair_up(self):
        t_only, b_only = unmatched_calls(
            ["finalize_impl", "f"], ["~noncopyable", "f"],
            lambda t, b: {t, b} == {"finalize_impl", "~noncopyable"})
        self.assertFalse(t_only)
        self.assertFalse(b_only)

    def test_real_inline_difference_survives(self):
        t_only, b_only = unmatched_calls(["strip_pointer", "g"], ["g"], lambda t, b: False)
        self.assertEqual(dict(t_only), {"strip_pointer": 1})
        self.assertFalse(b_only)


class NameTests(unittest.TestCase):
    def test_template_with_pointer_argument(self):
        name = ("void stlp_std::__destroy_range_aux<stlp_std::reverse_iterator<a *>,a>"
                "(stlp_std::reverse_iterator<a *>, a*)")
        self.assertEqual(_qualified_name(name), "stlp_std::__destroy_range_aux<stlp_std::reverse_iterator<a *>,a>")

    def test_operators(self):
        self.assertEqual(_qualified_name("bool x::y::operator bool() const"), "x::y::operator bool")
        self.assertEqual(_qualified_name("char const* x::operator char const *() const"),
                         "x::operator char const *")
        self.assertEqual(_qualified_name("int foo::operator()(int)"), "foo::operator()")


class ClassTests(unittest.TestCase):
    def test_flag_classes(self):
        self.assertEqual(flag_class("vostok/game_core/sources/jump_logic.cpp"), "od")
        self.assertEqual(flag_class("vostok/vfs/sources/free_node.cpp"), "noog")
        self.assertEqual(flag_class("vostok/render/engine/sources/stage_lights.cpp"), "ox")
        self.assertEqual(flag_class("vostok/sound/sources/x.cpp"), "nogl")
        self.assertEqual(flag_class("vostok/memory_allocator_helper.h"), "header")
        self.assertEqual(flag_class("src/gfx/kernel/x.cpp"), "nogl")

    def test_owner_hint(self):
        od_copy = [{"instructions": [{"text": "push ebp"}, {"text": "mov ebp, esp"}]}]
        og_copy = [{"instructions": [{"text": "mov eax, [esp+4]"}, {"text": "ret"}]}]
        self.assertIn("owner-class barrier", owner_hint(False, od_copy))
        self.assertIn("owner-class barrier", owner_hint(True, og_copy))
        self.assertIn("not the owner barrier", owner_hint(True, od_copy))
        self.assertIn("outside the image", owner_hint(False, []))

    def test_trust_classes(self):
        leaf = {"file": "vostok/game_core/sources/a.cpp",
                "instructions": [{"text": "mov eax, [ecx+4]"}, {"text": "ret"}]}
        self.assertEqual(trust_class(leaf, "thiscall", lambda c: [])[0], "exact")
        custom = {"file": "vostok/game/sources/a.cpp",
                  "instructions": [{"text": "mov al, [eax+488h]"}, {"text": "ret"}]}
        klass, why = trust_class(custom, "thiscall", lambda c: [])
        self.assertEqual(klass, "unreliable")
        self.assertIn("own custom convention", why)
        caller = {"file": "vostok/game_core/sources/a.cpp",
                  "instructions": [{"text": "lea eax, [ebp-1]"}, {"text": "call identity"},
                                   {"text": "ret"}]}
        klass, why = trust_class(caller, "thiscall", lambda c: [(False, True)])
        self.assertEqual(klass, "unreliable")
        self.assertTrue(any("COMDAT owner class" in w for w in why))
        self.assertTrue(trust_class({"file": "vostok/sound/sources/a.cpp"}, "cdecl",
                                    lambda c: [])[0].startswith("exact"))


if __name__ == "__main__":
    unittest.main()
