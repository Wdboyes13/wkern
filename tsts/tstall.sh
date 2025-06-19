set -euxo pipefail
NASM=$1
CC=$2

./nasm-sanity-check.sh $NASM
./cc-arch-check.sh $CC
