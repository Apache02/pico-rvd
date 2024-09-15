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

if [ -z "$PICO_DEPLOY_TTY" ]; then
  PICO_DEPLOY_TTY="/dev/ttyACM0"
fi
if [ -z "$PICO_DEPLOY_TARGET" ]; then
  PICO_DEPLOY_TARGET="/media/$USER/RPI-RP2"
fi

echo "     tty: $PICO_DEPLOY_TTY"
echo "  target: $PICO_DEPLOY_TARGET"
echo "  .uf2 file: $UF2_FILE"
stat $UF2_FILE -c "  size: %s"

if [ ! -d $PICO_DEPLOY_TARGET ]; then
    stty -F "$PICO_DEPLOY_TTY" 1200 || echo "[WARN] Could not connect to device $PICO_DEPLOY_TTY, try to load bootloader manually"
fi

while [ ! -d $PICO_DEPLOY_TARGET ]; do sleep .25; done
sleep 1

cat $PICO_DEPLOY_TARGET/INFO_UF2.TXT || (echo "[ERROR] Incorrect target! Aborted." && exit 1)

cp $UF2_FILE "$PICO_DEPLOY_TARGET/"
