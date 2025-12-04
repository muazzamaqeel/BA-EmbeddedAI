rw(){ STM32_Programmer_CLI -c port=SWD mode=UR -r32 $1 1 \
| tr -d '\r' \
| grep -E "^0x[0-9A-Fa-f]+[[:space:]]*:[[:space:]]*[0-9A-Fa-f]+$" \
| awk '{print $3}'; }; \
W0=$(rw 0x46009014); W1=$(rw 0x46009018); W2=$(rw 0x4600901C); \
echo "UID[31:0]   = $W0"; \
echo "UID[63:32]  = $W1"; \
echo "UID[95:64]  = $W2"; \
echo "FULL UID    = ${W2}${W1}${W0}"
