#!/usr/bin/env bash
echo "=========================================="
echo " Ext      Files     Chars         Lines   "
echo "=========================================="

for ext in py c asm ld sh h; do
  files=$(find . -name "*.$ext" | wc -l)
  chars=$(find . -name "*.$ext" -exec cat {} + 2>/dev/null | wc -m)
  lines=$(find . -name "*.$ext" -exec cat {} + 2>/dev/null | wc -l)
  printf " %-8s %5d %10d %10d\n" ".$ext" "$files" "$chars" "$lines"
done

echo "=========================================="