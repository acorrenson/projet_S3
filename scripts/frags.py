
def frags(t1, t2):
    n_left = len(t1)
    i1 = 0
    i2 = 0
    frags = []
    dim = len(t1)
    while n_left > 0:
        frags.append([t1[i1]])
        while t2[i2] != t1[i1]:
            i2 += 1
        if t1[(i1+1) % dim] != t2[(i2+1) % dim] and t1[(i1+1) % dim] != t2[(i2-1) % dim]:
            frags.append([])


t1 = [1, 2, 3]
t2 = [3, 2, 1]
