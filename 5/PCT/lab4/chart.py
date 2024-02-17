#!/usr/bin/env python3

import os

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np


def draw(filenames, labels):
    dataX = []
    dataY = []

    
    plt.rcParams["legend.markerscale"] = 2.0
    plt.rcParams['font.family'] = 'sans-serif'
    plt.rcParams['font.size'] = '12'
    # 'upper left', 'upper right', 'lower left', 'lower right', 'center'
    plt.rcParams["legend.loc"] = "upper left"
    plt.rcParams["legend.fontsize"] = "5"

    cm = 1/2.54  # centimeters in inches
    fig = plt.figure(figsize=(9*cm, 7*cm))
    ax = fig.add_subplot(111)
    ax.set_title("")
    ax.set(xlabel="Количество p процессов", ylabel="Ускорение S(p)")
    ax.label_outer()

    ax.xaxis.set_tick_params(direction='in', which='both')
    ax.yaxis.set_tick_params(direction='in', which='both')
    ax.xaxis.set_major_locator(ticker.MultipleLocator(2))
    ax.yaxis.set_major_locator(ticker.MultipleLocator(2))

    plt.figtext(0.224, 0.15, "(1x4)", fontsize=5, ha='center', va='center')
    plt.figtext(0.334, 0.15, "(1x6)", fontsize=5, ha='center', va='center')
    plt.figtext(0.444, 0.15, "(2x4)", fontsize=5, ha='center', va='center')
    plt.figtext(0.557, 0.15, "(2x5)", fontsize=5, ha='center', va='center')
    plt.figtext(0.677, 0.15, "(2x6)", fontsize=5, ha='center', va='center')
    plt.figtext(0.786, 0.15, "(2x7)", fontsize=5, ha='center', va='center')
    plt.figtext(0.896, 0.15, "(2x8)", fontsize=5, ha='center', va='center')
    #plt.figtext(0.90, 0.15, "(2x8)", fontsize=5, ha='center', va='center')

    count_versions = 0
    lbls = []
    for (fname, datalabel) in zip(filenames, labels):
        data = np.loadtxt(fname)
        dataX.append((data[:, 0]))
        dataY.append(data[:, 1])
        lbls.append(datalabel)
        count_versions += 1

    thrd_cur = min(dataX[0])
    thrd_step = dataX[0][1] - dataX[0][0] if len(dataX[0]) > 1 else 0
    thrd_end = max(dataX[0])
    wdh = 0.8 / (count_versions + 1)

    first_iteration = True
    p_offset = -wdh * count_versions * 0.5
    while thrd_cur <= thrd_end:
        if first_iteration:
            lbl = "Линейное ускорение"
            first_iteration = False
        else:
            lbl = ""
        ax.bar(thrd_cur + p_offset, thrd_cur,
               label=lbl, color="#AC6869", width=wdh)
        thrd_cur += thrd_step

    # p_offset += wdh * 0.5
    p_offset += wdh
    for i in range(0, len(dataX)):
        colors = ["#ECA4A4", "#F7E2E1"]  # Красный и желтый цвета для данных
        ax.bar(dataX[i] + p_offset, dataY[i], label=lbls[i], color=colors[i], width=wdh)
        p_offset += wdh

   # ax.set_yticks([1, 2, 3, 4, 5, 6, 7, 8, 9])
    ax.set_yticks(np.arange(10) * 2)
    ax.set_yticklabels(np.arange(10))
    plt.tight_layout()
    ax.legend()
    fig.savefig('chart.png', dpi=300)
    # fig.savefig('chart.pdf', dpi=300)

if __name__ == "__main__":
    draw(["data1.dat", "data2.dat"], ["n=2500", "n=5000"])
