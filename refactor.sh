#!/bin/bash
# Usage: ./refactor.sh [-r] old_code new_code

USE_REGEX=false
if [ "$1" == "-r" ]; then
  USE_REGEX=true
  shift
fi

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 [-r] old_code new_code"
  exit 1
fi

OLD=$1
NEW=$2

ack_args=(--ignore-dir=objs)

if [ "$USE_REGEX" = true ]; then
  ack_args+=(-r)
fi

ack -l "${ack_args[@]}" "$OLD" | while read -r file; do
  echo "Refactoring '$OLD' -> '$NEW' in $file"
  if [ "$USE_REGEX" = true ]; then
    perl -i.bak -pe "s/$OLD/$NEW/g" "$file"
  else
    perl -i.bak -pe "s/\Q$OLD\E/$NEW/g" "$file"
  fi
  rm "${file}.bak"
done
