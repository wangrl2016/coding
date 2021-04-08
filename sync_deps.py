#!/usr/bin/env python3

import os
import subprocess
import sys

# 项目所需要的依赖
dependencies = {
    "engine/googletest": "https://github.com/google/googletest@3ff1e8b98a3d1d3abc24a5bacb7651c9b32faedd",
    "sfml/googletest": "https://github.com/google/googletest@3ff1e8b98a3d1d3abc24a5bacb7651c9b32faedd",
    "opengl/glm": "https://github.com/g-truc/glm@ace16e47780dcef815294715237f51e9129b6eb3",
}


def status(directory, commit_hash, change):
    def truncate(s, length):
        return s if len(s) <= length else s[:(length - 3)] + '...'

    dir_len = 36
    directory = truncate(directory, dir_len)
    commit_hash = truncate(commit_hash, 40)
    symbol = '>' if change else '@'
    sys.stdout.write('%-*s %s %s\n' % (dir_len, directory, symbol, commit_hash))


def is_sha1_sum(s):
    """
    SHA1 sums are 160 bits, encoded as lowercase hexadecimal.
    """
    return len(s) == 40 and all(c in '0123456789abcdef' for c in s)


def check_executable():
    with open(os.devnull, 'w') as devnull:
        try:
            subprocess.run(['git', '--version'], stdout=devnull)
            subprocess.call(['java', '--version'], stdout=devnull)
            subprocess.call(['cmake', '--version'], stdout=devnull)
            subprocess.call(['ffmpeg', '-version'], stdout=devnull)
        except (OSError,):
            return None
    return 'OK'


def git_sync_deps():
    for directory in sorted(dependencies):
        if '@' in dependencies[directory]:
            repo, commit_hash = dependencies[directory].split('@', 1)
        else:
            raise Exception("Please specify commit")

        if not is_sha1_sum(commit_hash):
            raise Exception("Poorly formed commit hash: %r" % commit_hash)

        if not os.path.isdir(directory):
            subprocess.run(['git', 'clone', '--quiet', '--no-checkout', repo, directory])
            subprocess.run(['git', 'checkout', '--quiet', commit_hash], cwd=directory)

            status(directory, commit_hash, True)  # success
            continue

        with open(os.devnull, 'w') as devnull:
            # If this fails, we will fetch before trying again.
            # Don't spam user with error information.
            if 0 == subprocess.run(['git', 'checkout', '--quiet', commit_hash],
                                   cwd=directory, stderr=devnull).returncode:
                status(directory, commit_hash, False)  # success
                continue

        subprocess.run(['git', 'remote', 'set-url', 'origin', repo], cwd=directory)
        subprocess.run(['git', 'fetch', '--quiet'], cwd=directory)
        subprocess.run(['git', 'checkout', '--quiet', commit_hash], cwd=directory)

        status(directory, commit_hash, True)  # success


def main():
    assert check_executable()
    git_sync_deps()


if __name__ == '__main__':
    exit(main())
