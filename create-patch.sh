#!/usr/bin/env bash

VERSION="${1}"
FILE="${2:-patch-"${1}".diff}"

git config --local diff.mnemonicprefix true
git diff --ignore-space-at-eol "${VERSION}" --  . ':!*.diff' ':!.git*' ':!create-patch.sh' > "${FILE}"
