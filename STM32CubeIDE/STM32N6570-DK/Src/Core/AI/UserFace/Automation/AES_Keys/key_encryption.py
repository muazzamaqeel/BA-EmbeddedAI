import hashlib
from Crypto.Cipher import AES

AES_KEY = bytes.fromhex("603DEB1015CA71BE2B73AEF0857D7781")  # 128-bit key
AES_IV  = bytes.fromhex("000102030405060708090A0B0C0D0E0F")   # 16-byte IV

# ---- UID FROM BOARD ----
UID_HEX = "003031564236500300320026"  # replace with extractor result
UID_BYTES = bytes.fromhex(UID_HEX)

# ---- Derive MASTER KEY/IV from UID ----
sha = hashlib.sha256(UID_BYTES).digest()
MASTER_KEY = sha[:16]
MASTER_IV = sha[16:32]

def aes_encrypt(raw16):
    cipher = AES.new(MASTER_KEY, AES.MODE_CBC, MASTER_IV)
    return cipher.encrypt(raw16)

# ---- Encrypt ----
enc_key = aes_encrypt(AES_KEY)
enc_iv  = aes_encrypt(AES_IV)

# ---- Save encrypted files ----
with open("aes_key.enc", "wb") as f:
    f.write(enc_key)

with open("aes_iv.enc", "wb") as f:
    f.write(enc_iv)

print("Encrypted files generated:")
print("aes_key.enc")
print("aes_iv.enc")