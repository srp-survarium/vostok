"""`python3 -m vostok.ledger <verb> ...`"""

import sys

from vostok.ledger import main

sys.argv[0] = "vostok.ledger"
raise SystemExit(main())
