import matplotlib.pyplot as plt
import sys


def read_from_file(fn):
    l = []
    with open(fn, "r") as f:
        for line in f.readlines():
            a, b = line.split(" ")
            l.append((float(a), float(b)))

    return l


def graphe(ll):
    colors = ['r', 'g', 'c']
    for i, l in enumerate(ll):
        ic = i % 3
        c = colors[ic]
        long = len(l)
        plt.plot([l[i % long][0] for i in range(long+1)],
                 [l[i % long][1] for i in range(long+1)], 'o'+c+'-')
    plt.show()


graphe([read_from_file(sys.argv[i]) for i in range(1, len(sys.argv))])
