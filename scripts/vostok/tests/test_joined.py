# SPDX-License-Identifier: GPL-3.0-or-later

import unittest

from vostok.tool.joined import is_joined


class JoinedLineDetection(unittest.TestCase):
    def test_two_statements_on_one_line(self):
        self.assertTrue(is_joined("\tpstr res = strstr(m_begin, s); return res ? 1 : npos;"))
        self.assertTrue(is_joined("\tclear (); *this += s;"))
        self.assertTrue(is_joined("{ ASSERT( x ); m_state->reset( ); }"))

    def test_single_statements_are_not_joined(self):
        self.assertFalse(is_joined("\treturn res ? (size_type)(res - m_begin) : npos;"))
        self.assertFalse(is_joined("\tif ( this == &s ) return *this;"))
        self.assertFalse(is_joined("\tinline u16 amount ( ) const { return m_amount; }"))
        self.assertFalse(is_joined("\t}"))
        self.assertFalse(is_joined("\tvirtual bool is_sprinting ( ) const override { return true; /* no source */ }"))

    def test_for_headers_comments_and_strings_are_ignored(self):
        self.assertFalse(is_joined("\tfor ( u32 i = 0; i < n; ++i )"))
        self.assertFalse(is_joined("\t// a; b; c;"))
        self.assertFalse(is_joined('\tLOG( "x; y;" );'))
        self.assertFalse(is_joined("#define X( a ) a; b \\"))
        self.assertFalse(is_joined("typedef int a; typedef int b;"))

    def test_case_one_liners_and_wrapped_for_headers_are_idioms(self):
        self.assertFalse(is_joined("\tcase kSHOP:\t\tshow( 1 );\tbreak;"))
        self.assertFalse(is_joined("\tdefault: mouse_btn = 2; break;"))
        self.assertFalse(is_joined("\t\ti = 0; i < n; ++i )"))
        self.assertTrue(is_joined("\tcase kSHOP: a = 1; b = 2; break;"))


if __name__ == "__main__":
    unittest.main()
