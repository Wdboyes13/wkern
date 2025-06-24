#!/usr/bin/env bash
echo "==========================================="
echo " Lang          Files     Chars       Lines "
echo "==========================================="

total_files=0
total_chars=0
total_lines=0

asm_lines=0
c_lines=0
h_lines=0
ld_lines=0
py_lines=0
sh_lines=0


for ext in asm c h ld py sh; do

  case "$ext" in
    c)   lang="C";           comment_re='^\s*//';;
    py)  lang="Python";      comment_re='^\s*#';;
    asm) lang="Assembly";    comment_re='^\s*;';;
    ld)  lang="LD Script";   comment_re='^\s*#';;
    sh)  lang="Shell Script"; comment_re='^\s*#';;
    h)   lang="C Header";    comment_re='^\s*//';;
  esac

  files=$(find . -name "*.$ext" | wc -l)
  chars=$(find . -name "*.$ext" -exec cat {} + 2>/dev/null | wc -m)
  lines=$(find . -name "*.$ext" -exec cat {} + 2>/dev/null \
    | grep -Ev '^\s*$' \
    | grep -Ev "$comment_re" \
    | wc -l)

  total_files=$((total_files + files))
  total_chars=$((total_chars + chars))
  total_lines=$((total_lines + lines))

  case "$ext" in
    c)   c_lines=$lines;;
    py)  py_lines=$lines;;
    asm) asm_lines=$lines;;
    ld)  ld_lines=$lines;;
    sh)  sh_lines=$lines;;
    h)   h_lines=$lines;;
  esac

  printf " %-12s %5d %10d %10d\n" "$lang" "$files" "$chars" "$lines"
done

echo "==========================================="
printf " %-12s %5d %10d %10d\n" "Total" "$total_files" "$total_chars" "$total_lines"
echo "==========================================="

cat > loc.md <<EOF
# Total Lines
![Lines Pie Chart](https://quickchart.io/chart?bkg=white&c=%7B%22type%22%3A%22pie%22%2C%22data%22%3A%7B%22labels%22%3A%5B%22Assembly%22%2C%22C%22%2C%22C%20Header%22%2C%22LD%20Script%22%2C%22Python%22%2C%22Shell%20Script%22%5D%2C%22datasets%22%3A%5B%7B%22label%22%3A%22Lines%22%2C%22data%22%3A%5B$asm_lines%2C$c_lines%2C$h_lines%2C$ld_lines%2C$py_lines%2C$sh_lines%5D%7D%5D%7D%7D)
EOF