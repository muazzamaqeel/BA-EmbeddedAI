import hashlib
from Crypto.Cipher import AES

# =========================================================
# 1. Original AES KEY & IV (your secrets)
# =========================================================
AES_KEY = bytes.fromhex("603DEB1015CA71BE2B73AEF0857D7781")
AES_IV  = bytes.fromhex("000102030405060708090A0B0C0D0E0F")

assert len(AES_KEY) == 16
assert len(AES_IV) == 16

# =========================================================
# 2. CORRECT LITTLE-ENDIAN UID FROM STM32
# =========================================================
UID_HEX = "003031564236500300320026"   # <<<< FIXED
UID_BYTES = bytes.fromhex(UID_HEX)

# =========================================================
# 3. Derive MASTER KEY + MASTER IV via SHA256(UID)
# =========================================================
sha = hashlib.sha256(UID_BYTES).digest()
MASTER_KEY = sha[:16]
MASTER_IV  = sha[16:32]

print("MASTER_KEY =", MASTER_KEY.hex().upper())
print("MASTER_IV  =", MASTER_IV.hex().upper())

# =========================================================
# AES CBC encrypt/decrypt 16-byte block WITHOUT padding
# =========================================================
def aes_encrypt_block16(raw16: bytes) -> bytes:
    cipher = AES.new(MASTER_KEY, AES.MODE_CBC, MASTER_IV)
    return cipher.encrypt(raw16)

def aes_decrypt_block16(enc16: bytes) -> bytes:
    cipher = AES.new(MASTER_KEY, AES.MODE_CBC, MASTER_IV)
    return cipher.decrypt(enc16)

# =========================================================
# 4. Encrypt
# =========================================================
enc_key = aes_encrypt_block16(AES_KEY)
enc_iv  = aes_encrypt_block16(AES_IV)

# Write encrypted files
with open("aes_key.enc", "wb") as f:
    f.write(enc_key)
with open("aes_iv.enc", "wb") as f:
    f.write(enc_iv)

print("\nEncrypted:")
print(" enc_key =", enc_key.hex().upper())
print(" enc_iv  =", enc_iv.hex().upper())

# =========================================================
# 5. DECRYPT AGAIN (round-trip test)
# =========================================================
dec_key = aes_decrypt_block16(enc_key)
dec_iv  = aes_decrypt_block16(enc_iv)

print("\nDecrypted:")
print(" dec_key =", dec_key.hex().upper())
print(" dec_iv  =", dec_iv.hex().upper())

# =========================================================
# 6. Verify correctness
# =========================================================
print("\nVerification:")
print("AES_KEY OK:", dec_key == AES_KEY)
print("AES_IV  OK:", dec_iv == AES_IV)

if dec_key == AES_KEY and dec_iv == AES_IV:
    print("\n✅ ROUND-TRIP SUCCESS — Python encryption/decryption OK")
else:
    print("\n❌ ERROR — Round-trip failed! (STM32 will also fail)")
