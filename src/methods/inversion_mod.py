

tab = ['a', 'b', 'c', 'f', 'e', 'd', 'g']


def inv_mod(t, a, b):
    dim = len(t)
    if (b < a):
        b = b+dim
    while a < b:
        t[a % dim], t[b % dim] = t[b % dim], t[a % dim]
        a += 1
        b -= 1


inv_mod(tab, 3, 5)
print(tab)
