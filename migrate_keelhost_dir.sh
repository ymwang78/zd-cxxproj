#!/bin/bash
# Moves hosts/HostVM to hosts/KeelHost (HostVM is KeelHost since ymwang78/KeelHost#48) and leaves
# hosts/HostVM as a symbolic link to it for one major version, so that every path written with the
# old name keeps working, and points the checkout's origin at ymwang78/KeelHost. Running it again
# does nothing.
# Usage: ./migrate_keelhost_dir.sh
set -e
cd "$(dirname "${BASH_SOURCE[0]}")/hosts"

if [ -L HostVM ]; then
    echo "hosts/HostVM is already a link to $(readlink HostVM): nothing to do"
    exit 0
fi
if [ -e KeelHost ]; then
    echo "hosts/KeelHost exists already, next to a real hosts/HostVM: sort them out by hand" >&2
    exit 1
fi
if [ ! -d HostVM ]; then
    echo "there is no hosts/HostVM here" >&2
    exit 1
fi

mv HostVM KeelHost
ln -s KeelHost HostVM
# Linked worktrees keep naming hosts/HostVM, which the link keeps valid. When it goes away with
# ymwang78/KeelHost#49, git worktree repair in hosts/KeelHost points them at the new place.
url=$(git -C KeelHost remote get-url origin 2>/dev/null || true)
if [ -n "$url" ]; then
    git -C KeelHost remote set-url origin "${url/ymwang78\/HostVM/ymwang78/KeelHost}"
fi
echo "hosts/KeelHost, with hosts/HostVM -> KeelHost; origin $(git -C KeelHost remote get-url origin 2>/dev/null)"
echo "Build directories configured from hosts/HostVM are reconfigured by build.sh on its next run."
