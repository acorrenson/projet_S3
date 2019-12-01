import matplotlib.pyplot as plt
import sys


def read_from_file(fn):
    l = []
    with open(fn, "r") as f:
        for line in f.readlines():
            a, b = line.split(" ")
            l.append((float(a), float(b)))

    return l


def graphe(l):
    long = len(l)
    plt.plot([l[i % long][0] for i in range(long+1)],
             [l[i % long][1] for i in range(long+1)], 'or-')
    plt.show()


graphe(read_from_file(sys.argv[1]))
