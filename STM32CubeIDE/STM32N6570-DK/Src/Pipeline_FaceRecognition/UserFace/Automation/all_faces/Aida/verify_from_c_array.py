#!/usr/bin/env python3
"""
verify_from_c_array.py
Parses auto-generated embeddings_table_*.c arrays and decrypts them
using the same AES-CBC parameters as STM32.
"""

import re, numpy as np
from Crypto.Cipher import AES

# AES key and IV — must match crypto_utils.c and gen_refset_tflite.py
AES_KEY = bytes.fromhex("603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4")[:16]
AES_IV  = bytes.fromhex("000102030405060708090A0B0C0D0E0F")

# -----------------------------------------------------------------------------
# Parse one C file (e.g. embeddings_table_Aida.c)
# -----------------------------------------------------------------------------
def parse_c_embedded_array(c_path):
    with open(c_path, "r") as f:
        text = f.read()

    # Extract the hex bytes inside { ... }
    hex_bytes = re.findall(r"0x([0-9A-Fa-f]{2})", text)
    data = bytes(int(h, 16) for h in hex_bytes)
    print(f"✅ Parsed {len(data)} bytes from {c_path}")
    return data

# -----------------------------------------------------------------------------
# Decrypt with AES-CBC
# -----------------------------------------------------------------------------
def decrypt_embedding(cipher_bytes):
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    plain = cipher.decrypt(cipher_bytes)
    floats = np.frombuffer(plain, dtype=np.float32)
    return floats

# -----------------------------------------------------------------------------
# Main verification logic
# -----------------------------------------------------------------------------
if __name__ == "__main__":
    C_FILE = "embeddings_table_Aida.c"  # ⚠️ change path if needed
    cipher_data = parse_c_embedded_array(C_FILE)
    floats = decrypt_embedding(cipher_data)

    print(f"\n✅ Decrypted {len(floats)} floats ({len(cipher_data)} bytes)")
    print("First 8 floats:")
    print(np.array2string(floats[:8], precision=6, separator=' '))

    print("\nCompare with STM32 output line:")
    print("[DEC][Aida] first 4 floats: ...")
