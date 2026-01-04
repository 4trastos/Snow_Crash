#!/bin/bash

TOKEN=$1
ALPHABET_LOWER="abcdefghijklmnopqrstuvwxyz"
ALPHABET_UPPER="ABCDEFGHIJKLMNOPQRSTUVWXYZ"

echo "Decrypting token: $TOKEN"
echo ""

for ROT in {0..25}
do
    # Crear alfabetos desplazados
    DISP_LOWER="${ALPHABET_LOWER:$ROT}${ALPHABET_LOWER:0:$ROT}"
    DISP_UPPER="${ALPHABET_UPPER:$ROT}${ALPHABET_UPPER:0:$ROT}"

    # Aplicar ROT (minúsculas y mayúsculas)
    DECRYPTED=$(echo "$TOKEN" | tr 'a-z' $DISP_LOWER | tr 'A-Z' $DISP_UPPER)

    # Mostrar resultado
    printf "ROT %02d: %s\n" $ROT "$DECRYPTED"
done

