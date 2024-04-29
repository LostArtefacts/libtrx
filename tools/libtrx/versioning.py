from pathlib import Path

from libtrx.git import Git


def generate_version(repo_dir: Path | None = None) -> str:
    git = Git(repo_dir=repo_dir)
    return git.get_branch_version(branch=None) or "?"
