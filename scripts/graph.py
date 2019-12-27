import matplotlib.pyplot as plt
import sys


def parse(fn):
    dimension = 0
    tours = []
    instance = dict()

    with open(fn, "r") as f:
        lines = [line[:-1].replace(' ', '').split(';') for line in f]
        dimension = int(lines[1][1])
        for i in range(4, 4+dimension):
            instance[int(lines[i][0])] = (int(lines[i][1]), int(lines[i][2]))
        for l in lines[4+dimension+1:]:
            tours.append((l[0], eval(l[3])))

    return [(tours[i][0], [instance[j] for j in tours[i][1]]) for i in range(len(tours))]


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
    w = len(ll)
    plt.figure(figsize=(w*3, 3*(1 + w//3)))
    plt.axis('scaled')
    print(w//3)
    for i, l in enumerate(ll):
        plt.subplot(1 + w//3, 3, i+1)
        plt.title(l[0])
        ic = i % len(colors)
        c = colors[ic]
        long = len(l[1])
        plt.plot([l[1][i % long][0] for i in range(long+1)],
                 [l[1][i % long][1] for i in range(long+1)], f'o{c}-')
    plt.show()


graph(parse(sys.argv[1]))
