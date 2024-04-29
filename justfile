lint-imports:
    tools/sort_imports

lint-format:
    pre-commit run -a

lint: (lint-imports) (lint-format)
