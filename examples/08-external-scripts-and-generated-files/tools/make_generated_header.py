from __future__ import annotations

import sys
from pathlib import Path


def main() -> int:
    if len(sys.argv) != 3:
        print(
            "Usage: make_generated_header.py <output_header> <label>",
            file=sys.stderr,
        )
        return 1

    output_header = Path(sys.argv[1])
    label = sys.argv[2]

    content = f"""#pragma once

namespace generated_build_info
{{
    inline const char* label()
    {{
        return "{label}";
    }}
}}
"""

    output_header.write_text(content, encoding="utf-8")
    print(f"Wrote generated header: {output_header}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())