def _left_rotate(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xffffffff

def _right_rotate(n, b):
    return ((n >> b) | (n << (32 - b))) & 0xffffffff

def setup_chunk(chunk):
    return bytearray.fromhex(chunk.replace(' ', ''))

def reverse_sha1(start_round, chunk, start_a, start_b, start_c, start_d, start_e):
    a_i_plus_1 = start_a
    b_i_plus_1 = start_b
    c_i_plus_1 = start_c
    d_i_plus_1 = start_d
    e_i_plus_1 = start_e

    assert len(chunk) == 64
    w = [0] * 80
    for i in range(16):
        w[i] = int.from_bytes(chunk[i*4:i*4+4], 'big')
    for i in range(16, 80):
        w[i] = _left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1)

    for i in range(start_round, -1, -1):
        a_i = b_i_plus_1
        b_i = _right_rotate(c_i_plus_1, 30)
        c_i = d_i_plus_1
        d_i = e_i_plus_1

        if i < 20:
            k_i = 1518500249
            f_i_plus_1 = d_i ^ (b_i & (c_i ^ d_i))
        elif i < 40:
            k_i = 1859775393
            f_i_plus_1 = b_i ^ c_i ^ d_i
        elif i < 60:
            k_i = 2400959708
            f_i_plus_1 = (b_i & c_i) | (b_i & d_i) | (c_i & d_i)
        else:
            k_i = 3395469782
            f_i_plus_1 = b_i ^ c_i ^ d_i

        e_i = (a_i_plus_1 - (_left_rotate(a_i, 5) + f_i_plus_1 + k_i + w[i])) & 0xffffffff

        print(f"i={i} : a = {a_i_plus_1}, b = {b_i_plus_1}, c = {c_i_plus_1}, d = {d_i_plus_1}, e = {e_i_plus_1}, f = {f_i_plus_1}, w = {w[i]}")

        a_i_plus_1, b_i_plus_1, c_i_plus_1, d_i_plus_1, e_i_plus_1 = a_i, b_i, c_i, d_i, e_i

    return (
        a_i_plus_1,
        b_i_plus_1,
        c_i_plus_1,
        d_i_plus_1,
        e_i_plus_1,
        (start_a + a_i) & 0xffffffff,
        (start_b + b_i) & 0xffffffff,
        (start_c + c_i) & 0xffffffff,
        (start_d + d_i) & 0xffffffff,
        (start_e + e_i) & 0xffffffff
    )

chunk = setup_chunk("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00")

start_a = 2652194797
start_b = 1475121602
start_c = 821726152
start_d = 3346553428
start_e = 2817500643
start_round = 79

h0, h1, h2, h3, h4, a, b, c, d, e = reverse_sha1(start_round, chunk, start_a, start_b, start_c, start_d, start_e)

print(f"i=79 : h0 = {h0}, h1 = {h1}, h2 = {h2}, h3 = {h3}, h4 = {h4}")
print(f"i=79 (final) : a = {a}, b = {b}, c = {c}, d = {d}, e = {e}")