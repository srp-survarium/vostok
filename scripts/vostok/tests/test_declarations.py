# SPDX-License-Identifier: GPL-3.0-or-later

import unittest

from vostok.diff.declarations import collect


def declaration(kind, name, type_name, access, *attributes):
    return {
        "kind": kind,
        "name": name,
        "type_name": type_name,
        "access": access,
        "attributes": list(attributes),
    }


def class_record(name, target, base, *, status="different", differences=()):
    return {
        "name": name,
        "status": status,
        "target_variants": [{"declarations": target}],
        "base_variants": [{"declarations": base}],
        "differences": list(differences),
    }


class DeclarationAuditTests(unittest.TestCase):
    def test_reports_access_static_and_virtual_as_one_exact_qualifier_error(self):
        target = [declaration(
            "method", "construct", "static void <method>(int)", "public", "static",
        )]
        base = [declaration(
            "method", "construct", "void <method>(int)", "private", "virtual",
        )]
        findings, stats = collect(
            {"classes": [class_record(
                "vostok::network_core::packet", target, base,
            )]},
            ("network_core",),
        )
        self.assertEqual(len(findings), 1)
        self.assertEqual(findings[0].target, ("public static",))
        self.assertEqual(findings[0].base, ("private virtual",))
        self.assertEqual(stats["network_core"]["qualifier_errors"], 1)

    def test_static_fields_are_compared_with_instance_fields(self):
        target = [declaration("static-field", "m_value", "u32", "protected")]
        base = [declaration("field", "m_value", "u32", "public")]
        findings, _ = collect(
            {"classes": [class_record(
                "vostok::network::holder", target, base,
            )]},
            ("network",),
        )
        self.assertEqual(findings[0].member_kind, "field")
        self.assertEqual(findings[0].target, ("protected static",))
        self.assertEqual(findings[0].base, ("public instance",))

    def test_reports_declaration_order_with_raw_target_and_base_positions(self):
        difference = {
            "category": "declaration-order",
            "member": "construct_packet",
            "target": "#12 method construct_packet access=public attributes=static",
            "base": "#27 method construct_packet access=private attributes=static",
        }
        findings, stats = collect(
            {"classes": [class_record(
                "vostok::network_core::client", [], [], differences=[difference],
            )]},
            ("network_core",),
        )
        self.assertEqual(findings[0].category, "order")
        self.assertEqual(stats["network_core"]["order_errors"], 1)

    def test_reads_qualifiers_from_diagnostic_differences(self):
        differences = [{
            "category": "inheritance-access",
            "member": "boost::noncopyable",
            "target": "private",
            "base": "public",
        }, {
            "category": "method-qualifiers",
            "member": "tick",
            "target": "introducing-virtual (0x3)",
            "base": "0x0",
        }]
        findings, stats = collect(
            {"classes": [class_record(
                "vostok::network_core::client", [], [], differences=differences,
            )]},
            ("network_core",),
        )
        self.assertEqual([row.member_kind for row in findings], ["base", "method"])
        self.assertEqual(stats["network_core"]["qualifier_errors"], 2)

    def test_ignores_compiler_generated_provenance(self):
        differences = [{
            "category": "method-qualifiers",
            "member": "copy",
            "target": "compiler-generated (0x100)",
            "base": "0x0",
        }]
        findings, stats = collect(
            {"classes": [class_record(
                "vostok::network_core::client", [], [], differences=differences,
            )]},
            ("network_core",),
        )
        self.assertEqual(findings, [])
        self.assertEqual(stats["network_core"]["qualifier_errors"], 0)

    def test_pairs_static_and_instance_field_presence(self):
        differences = [{
            "category": "field-presence", "member": "m_count",
            "target": None, "base": "field m_count",
        }, {
            "category": "static-field-presence", "member": "m_count",
            "target": "static-field m_count", "base": None,
        }]
        findings, _ = collect(
            {"classes": [class_record(
                "vostok::network::client", [], [], differences=differences,
            )]},
            ("network",),
        )
        self.assertEqual(findings[0].target, ("static",))
        self.assertEqual(findings[0].base, ("instance",))

    def test_includes_source_owned_global_helpers(self):
        differences = [{
            "category": "inheritance-presence", "member": "boost::noncopyable",
            "target": "base boost::noncopyable", "base": None,
        }]
        findings, stats = collect(
            {"classes": [class_record(
                "delayed_packets_predicate", [], [], differences=differences,
            )]},
            ("network_core",),
        )
        self.assertEqual(findings[0].module, "network_core")
        self.assertEqual(stats["network_core"]["classes"], 1)

    def test_preserves_target_variant_ambiguity(self):
        method = "void <method>()"
        record = class_record(
            "vostok::network_core::client",
            [declaration("method", "tick", method, "public")],
            [declaration("method", "tick", method, "private")],
            status="variant-overlap",
        )
        record["target_variants"].append({
            "declarations": [declaration("method", "tick", method, "protected")]
        })
        findings, stats = collect({"classes": [record]}, ("network_core",))
        self.assertEqual(findings[0].target, ("protected instance", "public instance"))
        self.assertEqual(stats["network_core"]["variant_ambiguous_classes"], 1)

    def test_filters_requested_module(self):
        records = [
            class_record(
                "vostok::network::client",
                [declaration("field", "m_x", "u32", "public")],
                [declaration("field", "m_x", "u32", "private")],
            ),
            class_record(
                "vostok::sound::world",
                [declaration("field", "m_x", "u32", "public")],
                [declaration("field", "m_x", "u32", "private")],
            ),
        ]
        findings, stats = collect({"classes": records}, ("network", "network_core"))
        self.assertEqual([row.module for row in findings], ["network"])
        self.assertEqual(stats["network_core"]["classes"], 0)


if __name__ == "__main__":
    unittest.main()
