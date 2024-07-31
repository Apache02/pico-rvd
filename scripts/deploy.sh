#!/bin/sh

if [ $# -eq 0 ]; then
  echo "[ERROR] Argument required"
  exit 1
fi

if [ ! -f $1 ]; then
  echo "[ERROR] File not found!"
  exit 1
fi

UF2_FILE="$1"
DEPLOY_TARGET="/media/$USER/RPI-RP2"
TTY_PATH="/dev/ttyACM0"

echo "  .uf2 file: $UF2_FILE"
echo "  target: $DEPLOY_TARGET"
stat $UF2_FILE -c "  size: %s"

if [ ! -d $DEPLOY_TARGET ]; then
    stty -F "$TTY_PATH" 1200 || echo "[WARN] Could not connect to device $TTY_PATH, try to load bootloader manually"
fi

while [ ! -d $DEPLOY_TARGET ]; do sleep .25; done

cat $DEPLOY_TARGET/INFO_UF2.TXT || (echo "[ERROR] Incorrect target! Aborted." && exit 1)

sleep 1
cp $UF2_FILE "$DEPLOY_TARGET/"
