import argparse
import os
from dataclasses import dataclass
from pathlib import Path
from subprocess import check_call, run
from typing import Any

from libtrx.packaging import create_zip
from libtrx.versioning import generate_version


@dataclass
class Options:
    ship_dir: Path
    build_root: Path
    compile_args: list[str]
    release_zip_filename: str
    release_zip_files: list[tuple[Path, str]]
    strip_tool = "strip"
    upx_tool = "upx"
    target = os.environ.get("TARGET", "debug")
    compressable_exes: list[Path] | None = None


def compress_exe(options: Options, path: Path) -> None:
    if run([options.upx_tool, "-t", str(path)]).returncode != 0:
        check_call([options.strip_tool, str(path)])
        check_call([options.upx_tool, str(path)])


class BaseCommand:
    name: str = NotImplemented
    help: str = NotImplemented

    def decorate_parser(self, parser: argparse.ArgumentParser) -> None:
        pass

    def run(self, args: argparse.Namespace) -> None:
        raise NotImplementedError("not implemented")


class CompileCommand(BaseCommand):
    name = "compile"

    def run(self, args: argparse.Namespace, options: Options) -> None:
        pkg_config_path = os.environ.get("PKG_CONFIG_PATH")

        if not Path("/app/build/linux/build.jinja").exists():
            command = [
                "meson",
                "--buildtype",
                options.target,
                *options.compile_args,
                options.build_root,
            ]
            if pkg_config_path:
                command.extend(["--pkg-config-path", pkg_config_path])
            check_call(command)

        check_call(["meson", "compile"], cwd=options.build_root)

        if options.target == "release":
            for exe_path in options.compressable_exes:
                compress_exe(options, exe_path)


class PackageCommand(BaseCommand):
    name = "package"

    def decorate_parser(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument("-o", "--output", type=Path)

    def run(self, args: argparse.Namespace, options: Options) -> None:
        if args.output:
            zip_path = args.output
        else:
            zip_path = Path(
                options.release_zip_filename.format(version=generate_version())
            )

        source_files = [
            *[
                (path, path.relative_to(options.ship_dir))
                for path in options.ship_dir.rglob("*")
                if path.is_file()
            ],
            *options.release_zip_files,
        ]

        create_zip(zip_path, source_files)
        print(f"Created {zip_path}")


def parse_args(commands: dict[str, BaseCommand]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Docker entrypoint")
    subparsers = parser.add_subparsers(dest="action", help="Subcommands")
    parser.set_defaults(action="compile", command=commands["compile"])

    for command in commands.values():
        subparser = subparsers.add_parser(command.name, help=command.help)
        command.decorate_parser(subparser)
        subparser.set_defaults(command=command)
    result = parser.parse_args()
    # if not hasattr(result, "command"):
    #     args.action = "compile"
    #     args.command = CompileCommand
    return result


def run_script(**kwargs: Any) -> None:
    commands = {
        command_cls.name: command_cls()
        for command_cls in BaseCommand.__subclasses__()
    }
    args = parse_args(commands)
    options = Options(**kwargs)
    args.command.run(args, options)
