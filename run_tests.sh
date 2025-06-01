#!/bin/bash

CWD=$(dirname "$0")

RED="\e[0;31m"
GREEN="\e[0;32m"
YELLOW="\e[0;33m"
# BLUE="\e[0;34m"
PURPLE="\e[0;35m"
RESET="\e[0m"

rc=0

if [[ ! -d "$CWD"/build ]]; then
    printf "%bno built tests found%b\n" "$YELLOW" "$RESET"
    printf "try using \`%bmake%b %bexamples-test%b\` to compile all tests\n" "$PURPLE" "$RESET" "$GREEN" "$RESET"
    exit 1
elif ! compgen -G "$CWD/build/*.test" > /dev/null; then
    printf "%bno built tests found%b\n" "$YELLOW" "$RESET"
    printf "try using \`%bmake%b %bexamples-test%b\` to compile all tests\n" "$PURPLE" "$RESET" "$GREEN" "$RESET"
    exit 1
fi

for file in "$CWD"/build/*.test; do
    if ! "$file"; then
        rc=1
        printf "%s %bfailed%b\n" "$(basename "$file")" "$RED" "$RESET"
    else
        printf "%s %bpassed%b\n" "$(basename "$file")" "$GREEN" "$RESET"
    fi
done

exit $rc
