#!/usr/bin/env bash
shopt -s globstar


# REMOVE ALL UNUSED FILES
read -p "Do you want to remove unused files [Y/N]: " dc
if [ $dc = "Y" ]; then
    find src/ -type f \( -name "**/*.c" -o -name "**/*.h" \) > all_files.txt
    grep -hoE '\S+\.(c|h)' objs/**/*.d | sort -u > used_files.txt
    comm -23 <(sort all_files.txt) <(sort used_files.txt) > unused_files.txt
    rm all_files.txt
    rm used_files.txt

    while IFS= read -r file; do
        if [[ -f "$file" ]]; then
            echo "Deleting: $file"
            rm "$file"
        else
            echo "Skipping (not a file): $file"
        fi
    done < unused_files.txt
    echo "Removed: "
    cat unused_files.txt
    rm unused_files.txt
fi


# REMOVE EMACS CACHE FILES
rm -f **/*#
rm -f **/*~

# CLEAN OBJS
rm -f **/*.o