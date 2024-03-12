from Crypto.Cipher import DES
import random
import math
import struct

def convert_to_bytes(list):
    return struct.pack('B' * len(list), *list)

def des_enc(k: bytes, m: bytes) -> bytes:
    """
    Encrypt a message m with a key k using DES as follows:
    c = DES(k, m)

    Note that for DES the key is given as 8 bytes, where the last bit of
    each byte is just a parity bit, giving the actual key of 56 bits, as
    expected for DES. The parity bits are ignored.

    Args:
        k (str): bytestring of length exactly 8 bytes.
        m (str): bytestring containing the message (i.e. a sequence of
            characters such as 'Hello' or '\x02\x04')

    Return:
        bytes: The bytestring ciphertext c
    """
    d = DES.new(k, DES.MODE_ECB)
    c = d.encrypt(m)
    return c


def des_dec(k: bytes, c: bytes) -> bytes:
    """
    Decrypt a message c with a key k using DES as follows:
    m = DES(k, c)

    Note that for DES the key is given as 8 bytes, where the last bit of
    each byte is just a parity bit, giving the actual key of 56 bits, as
    expected for DES. The parity bits are ignored.

    Args:
        k (str): bytestring of length exactly 8 bytes.
        c (str): bytestring containing the ciphertext (i.e. a sequence of
            characters such as 'Hello' or '\x02\x04')

    Return:
        bytes: The bytestring plaintext m
    """
    d = DES.new(k, DES.MODE_ECB)
    m = d.decrypt(c)
    return m

def des_enc_list(k, m):
    key = k
    key.append(0)
    
    key = convert_to_bytes(key)
    msg = convert_to_bytes(m)
    cipher = des_enc(key, msg)
    return [c for c in cipher]

def random_bytes(length):
    result = []
    for _ in range(length):
        result.append(random.randint(0, 255))
    return result

def group_bytes(bytes, length):
    result = []
    for i in range(math.floor(length / 2)):
        result.append(bytes[2 * i] * bytes[2 * i + 1])
    if length > 2 * math.floor(length / 2):
        result.append(bytes[-1])
    return result