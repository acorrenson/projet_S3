
seq = [1, 2, 3]

seq = sorted(seq)
print(seq)


def next(seq):
    i = len(seq) - 1
    while i > 0 and seq[i-1] >= seq[i]:
        assert 0 <= i, "invariant"
        i = i - 1

    beg_suff = i
    i = len(seq) - 1

    if beg_suff <= 0:
        return False

    while seq[beg_suff-1] >= seq[i]:
        i -= 1

    seq[i], seq[beg_suff-1] = seq[beg_suff-1], seq[i]
    seq[beg_suff:] = seq[beg_suff:][::-1]

    return True


while next(seq):
    print(seq)
