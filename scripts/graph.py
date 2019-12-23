import matplotlib.pyplot as plt
import sys


def read_from_file(fn):
    """
        Read a sequence of coordinates from a csv file.
    """
    l = []
    with open(fn, "r") as f:
        for line in f.readlines():
            a, b = line.split(" ")
            l.append((float(a), float(b)))

    return l


def graph(ll):
    """
        Plot a list of solutions to a TSP instance.
    """
    colors = ['r', 'g', 'c', 'y', 'm']
    for i, l in enumerate(ll):
        ic = i % len(colors)
        c = colors[ic]
        long = len(l)
        plt.plot([l[i % long][0] for i in range(long+1)],
                 [l[i % long][1] for i in range(long+1)], f'o{c}-')
    plt.show()


graph([read_from_file(sys.argv[i]) for i in range(1, len(sys.argv))])
