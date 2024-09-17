import argparse
import functools
import re
from pathlib import Path
from shutil import which
from subprocess import run

from libtrx.files import find_versioned_files


def custom_sort(source: list[str], forced_order: list[str]) -> list[str]:
    def key_func(item: str) -> tuple[int, int, str]:
        if item in forced_order:
            return (forced_order[0], forced_order.index(item))
        return (item, 0)

    return sorted(source, key=key_func)


def sort_imports(
    path: Path,
    root_dir: Path,
    own_include_map: dict[str, str],
    fix_map: dict[str, str],
    forced_order: list[str],
    include_dirs: list[Path],
) -> None:
    source = path.read_text()
    try:
        rel_path = path.relative_to(root_dir)
    except ValueError:
        matches = []
        for include_dir in include_dirs:
            try:
                rel_path = path.relative_to(include_dir)
            except ValueError:
                pass
            matches.append(rel_path)
        rel_path = sorted(matches, key=lambda path: len(str(path)))[0]

    own_include = str(rel_path.with_suffix(".h"))
    own_include = own_include_map.get(str(rel_path), own_include)

    for key, value in fix_map.items():
        source = re.sub(
            r'(#include ["<])' + re.escape(key) + '([">])',
            r"\1" + value + r"\2",
            source,
        )

    def cb(match):
        includes = re.findall(r'#include (["<][^"<>]+[">])', match.group(0))
        groups = {
            "self": set(),
            "local": set(),
            "shared": set(),
            "external": set(),
        }
        for include in includes:
            if include.strip('"') == own_include:
                groups["self"].add(include)
            elif include.startswith("<libtrx"):
                groups["shared"].add(include)
            elif include.startswith("<"):
                groups["external"].add(include)
            elif include.startswith('"'):
                groups["local"].add(include)

        groups = {key: value for key, value in groups.items() if value}

        ret = "\n\n".join(
            "\n".join(
                f"#include {include}"
                for include in custom_sort(group, forced_order)
            )
            for group in groups.values()
        ).strip()
        return ret

    source = re.sub(
        "^#include [^\n]+(\n*#include [^\n]+)*",
        cb,
        source,
        flags=re.M,
    )
    if source != path.read_text():
        path.write_text(source)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("path", type=Path, nargs="*")
    return parser.parse_args()


def run_script(
    root_dir: Path,
    include_dirs: list[Path],
    system_include_dirs: list[Path],
    own_include_map: dict[str, str],
    fix_map: dict[str, str],
    forced_order: list[str],
) -> None:
    args = parse_args()
    if args.path:
        paths = (path.absolute() for path in args.path)
    else:
        paths = (
            path
            for path in find_versioned_files(root_dir)
            if path.suffix in [".c", ".h"]
        )

    for path in paths:
        sort_imports(
            path,
            root_dir=root_dir,
            own_include_map=own_include_map,
            fix_map=fix_map,
            forced_order=forced_order,
            include_dirs=include_dirs,
        )
