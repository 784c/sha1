def _left_rotate(n, b):
    return ((n << b) | (n >> (32 - b))) & 0xffffffff

def _right_rotate(n, b):
    return ((n >> b) | (n << (32 - b))) & 0xffffffff

def setup_chunk(chunk):
    return bytearray.fromhex(chunk.replace(' ', ''))

def reverse_sha1(start_round, chunk, a_i_plus_1, b_i_plus_1, c_i_plus_1, d_i_plus_1, e_i_plus_1, e_list):
    start_a = a_i_plus_1
    start_b = b_i_plus_1
    start_c = c_i_plus_1
    start_d = d_i_plus_1
    start_e = e_i_plus_1

    k_list = [1518500249] * 20 + [1859775393] * 20 + [2400959708] * 20 + [3395469782] * 20

    assert len(chunk) == 64
    w = [0] * 80
    for i in range(16):
        w[i] = int.from_bytes(chunk[i*4:i*4+4], 'big')
    for i in range(16, 80):
        w[i] = _left_rotate(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1)

    for i in range(start_round, -1, -1):
        if i == start_round:
            a_i = a_i_plus_1
            b_i = b_i_plus_1
            c_i = c_i_plus_1
            d_i = d_i_plus_1
            e_i = e_i_plus_1
        else:
            a_i = b_i_plus_1
            b_i = _right_rotate(c_i_plus_1, 30)
            c_i = d_i_plus_1
            d_i = e_i_plus_1

        k_i = k_list[i]

        if i < 20:
            f_i_plus_1 = (d_i ^ (b_i & (c_i ^ d_i))) & 0xffffffff
        elif i < 40:
            f_i_plus_1 = (b_i ^ c_i ^ d_i) & 0xffffffff
        elif i < 60:
            f_i_plus_1 = ((b_i & c_i) | (b_i & d_i) | (c_i & d_i)) & 0xffffffff
        else:
            f_i_plus_1 = (b_i ^ c_i ^ d_i) & 0xffffffff

        if i % 2 == 1:
            e_i = (e_list[i]) & 0xffffffff
            w_i = (w[i]) & 0xffffffff
        else:
            e_i = (a_i_plus_1 - (_left_rotate(a_i, 5) + f_i_plus_1 + k_i + w_i)) & 0xffffffff
            w_i = (a_i_plus_1 - (_left_rotate(a_i, 5) + f_i_plus_1 + e_i + k_i)) & 0xffffffff
        
        if i < 20:
            f_i = e_i ^ (_left_rotate(c_i, 2) & (d_i ^ e_i))
        elif i < 40:
            f_i = _left_rotate(c_i, 2) ^ d_i ^ e_i
        elif i < 60:
            f_i = (_left_rotate(c_i, 2) & d_i) | (_left_rotate(c_i, 2) & e_i) | (d_i & e_i)
        else:
            f_i = _left_rotate(c_i, 2) ^ d_i ^ e_i

        print(f"i={i} : a = {a_i}, b = {b_i}, c = {c_i}, d = {d_i}, e = {e_i}, f = {f_i}, w = {w_i}")
        a_i_plus_1, b_i_plus_1, c_i_plus_1, d_i_plus_1, e_i_plus_1 = a_i, b_i, c_i, d_i, e_i

chunk = setup_chunk("00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00")

start_a = 2652194797
start_b = 1475121602
start_c = 821726152
start_d = 3346553428
start_e = 2817500643
start_round = 79

e_list = [0, 0, 0, 0, 1453366886, 716466993, 2905474187, 4244722412, 3480690589, 1188676058, 1808697654, 3080133689, 442220879, 1755967616, 1182204408, 3325654730, 3461885680, 3227039037, 3446640380, 3351676987, 4086698205, 2957732703, 1107172058, 3148963204, 350032329, 4229577255, 895441630, 4024600217, 2523018668, 3404223380, 2559885000, 1345235468, 758809951, 760300948, 3618456256, 3919073525, 2055468826, 246108336, 830707174, 746733994, 2465103991, 3144677536, 588208272, 3740744509, 1953161077, 1689418799, 3722334640, 3725589061, 4204961371, 1952780513, 2168015050, 2030654340, 430875378, 290277185, 1121414787, 4115551922, 4062675718, 2205830274, 1201366268, 625290977, 2925281768, 1302308911, 32428537, 2099187664, 417997254, 1769425752, 3166723034, 992600361, 3619920653, 2139799467, 1955098220, 4152800796, 1356075940, 4166127151, 2362756119, 4225431107, 315519473, 2274835022, 849509686, 2817500643]

reverse_sha1(start_round, chunk, start_a, start_b, start_c, start_d, start_e, e_list)