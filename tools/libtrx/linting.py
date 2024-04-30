#!/usr/bin/env python3
import json
from collections.abc import Callable, Iterable
from dataclasses import dataclass
from pathlib import Path


@dataclass
class LintWarning:
    path: Path
    message: str
    line: int | None = None

    def __str__(self) -> str:
        prefix = str(self.path)
        if self.line is not None:
            prefix += f":{self.line}"
        return f"{prefix}: {self.message}"


def lint_json_validity(path: Path) -> Iterable[LintWarning]:
    if path.suffix != ".json":
        return
    try:
        json.loads(path.read_text())
    except json.JSONDecodeError as ex:
        yield LintWarning(path, f"malformed JSON: {ex!s}")


def lint_newlines(path: Path) -> Iterable[LintWarning]:
    text = path.read_text(encoding="utf-8")
    if not text:
        return
    if not text.endswith("\n"):
        yield LintWarning(path, "missing newline character at end of file")
    if text.endswith("\n\n"):
        yield LintWarning(path, "extra newline character at end of file")


def lint_trailing_whitespace(path: Path) -> Iterable[LintWarning]:
    if path.suffix == ".md":
        return
    for i, line in enumerate(path.open("r"), 1):
        if line.rstrip("\n").endswith(" "):
            yield LintWarning(path, "trailing whitespace", line=i)


ALL_LINTERS: list[Callable[[], Iterable[LintWarning]]] = [
    lint_json_validity,
    lint_newlines,
    lint_trailing_whitespace,
]


def lint_file(file: Path) -> Iterable[LintWarning]:
    for linter_func in ALL_LINTERS:
        yield from linter_func(file)
