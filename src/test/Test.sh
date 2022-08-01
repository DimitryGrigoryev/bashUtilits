#!/bin/bash
echo --------"Testing s21_cat" ----------

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    declare -a flags=("-b" "-e" "-s" "-t" "-n" "-E" "-T" "--number" "--number-nonblank" "--squeeze-blank")
    files=(../test/text/*)
else
    declare -a flags=("-b" "-e" "-s" "-t" "-n")
    declare -a files=("../test/text/test3.txt" "../test/text/test1.txt" "../test/text/test2.txt")
fi

SUCCESS=true

function test_flags() {
    flags=("$@")
    for file in "${files[@]}"; do
        for flag in "${flags[@]}"; do
            if diff <(../cat/s21_cat "$flag" "$file") <(cat "$flag" "$file") >/dev/null; then
                echo >/dev/null
            else
                echo s21_cat "$flag" "$file" ............................. FAIL
                SUCCESS=false
            fi
        done
    done
}

test_flags "${flags[@]}"

if $SUCCESS; then
    echo "All test OK!"
fi

echo -e "--------Fin testing s21_cat ----------\n"
echo -e "--------Testing s21_grep ----------"

declare -a flags=("-i" "-v" "-c" "-l" "-n" "-s" "-h" "-e")
declare -a patterns=("you" "1." "You" "a" "in" "break" "21" "what" "the" "^void")

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    files=(../test/text/*)
else
    declare -a files=("../test/text/test3.txt" "../test/text/test1.txt" "../test/text/test2.txt")
fi

SUCCESS=true

# Test flags "-i" "-v" "-c" "-l" "-n" "-s" "-h" "-e"

for file in "${files[@]}"; do
    for flag in "${flags[@]}"; do
        for pattern in "${patterns[@]}"; do
            if diff <(../grep/s21_grep "$flag" "$pattern" "$file") <(grep "$flag" "$pattern" "$file") >/dev/null; then
                echo >/dev/null
            else
                echo s21_grep "$flag" "$pattern" "$file" ________________________FAIL
                SUCCESS=false
            fi
        done
    done
done


if $SUCCESS; then
    echo -e "All test OK!"
fi

echo -e "--------Fin testing s21_grep ----------"
