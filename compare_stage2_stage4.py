import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

plt.rcParams["font.family"] = "DejaVu Sans"
plt.rcParams["axes.unicode_minus"] = False


df2 = pd.read_csv("stage2_results.csv")
df4 = pd.read_csv("stage4_results.csv")


x2 = df2["percent"].to_numpy() * 100.0
x4 = df4["percent"].to_numpy() * 100.0


def plot_single(df, title_prefix, filename_prefix):
    x = df["percent"].to_numpy() * 100.0
    plt.figure()
    plt.plot(x, df["trueF0"], label="Истинное $F_0^t$")
    plt.plot(x, df["meanNt"], label="Оценка $E(N_t)$")
    plt.xlabel("Обработано потока (%)")
    plt.ylabel("Число уникальных элементов")
    plt.title(f"{title_prefix}: истинное $F_0^t$ и оценка")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{filename_prefix}_истина_и_оценка.png", dpi=200)
    plt.show()
    plt.figure()
    plt.plot(x, df["meanNt"], label="$E(N_t)$")
    plt.fill_between(
        x,
        df["meanMinusSigma"],
        df["meanPlusSigma"],
        alpha=0.2,
        label="$E(N_t) \\pm 1\\sigma$"
    )
    plt.xlabel("Обработано потока (%)")
    plt.ylabel("Оценка числа уникальных")
    plt.title(f"{title_prefix}: среднее и разброс (±1σ)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{filename_prefix}_среднее_плюс_минус_сигма.png", dpi=200)
    plt.show()

plot_single(df2, "Этап 2 (1 HLL)", "этап2")
plot_single(df4, "Этап 4 (MultiHLL, медиана)", "этап4")
plt.figure()
plt.plot(x2, df2["trueF0"], label="Истинное $F_0^t$")
plt.plot(x2, df2["meanNt"], label="Этап 2: $E(N_t)$")
plt.plot(x4, df4["meanNt"], label="Этап 4: $E(N_t)$")
plt.xlabel("Обработано потока (%)")
plt.ylabel("Число уникальных элементов")
plt.title("Сравнение средних оценок (Этап 2 vs Этап 4)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("сравнение_средних_оценок.png", dpi=200)
plt.show()
plt.figure()
plt.plot(x2, df2["meanNt"], label="Этап 2: среднее")
plt.fill_between(x2, df2["meanMinusSigma"], df2["meanPlusSigma"], alpha=0.2, label="Этап 2: ±1σ")
plt.plot(x4, df4["meanNt"], label="Этап 4: среднее")
plt.fill_between(x4, df4["meanMinusSigma"], df4["meanPlusSigma"], alpha=0.2, label="Этап 4: ±1σ")
plt.xlabel("Обработано потока (%)")
plt.ylabel("Оценка")
plt.title("Сравнение разброса (зоны ±1σ)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("сравнение_зон_сигма.png", dpi=200)
plt.show()
s2 = df2["stdevNt"].to_numpy(dtype=float)
s4 = df4["stdevNt"].to_numpy(dtype=float)
ratio = np.array([(s4[i] / s2[i]) if s2[i] != 0 else np.nan for i in range(len(s2))])
plt.figure()
plt.plot(x2, ratio, label="Отношение $\\sigma_4 / \\sigma_2$")
plt.axhline(1.0, linestyle="--", label="= 1 (без изменений)")
plt.xlabel("Обработано потока (%)")
plt.ylabel("Отношение σ")
plt.title("Насколько Этап 4 уменьшил разброс (σ)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("сравнение_отношения_сигма.png", dpi=200)
plt.show()
B = 10
m = 2 ** B
bound1 = 1.04 / np.sqrt(m)
bound2 = 1.3 / np.sqrt(m)
x = x2
true2 = df2["trueF0"].to_numpy(dtype=float)
mean2 = df2["meanNt"].to_numpy(dtype=float)
err2 = np.abs((mean2 - true2) / true2)
true4 = df4["trueF0"].to_numpy(dtype=float)
mean4 = df4["meanNt"].to_numpy(dtype=float)
err4 = np.abs((mean4 - true4) / true4)
plt.figure()
plt.plot(x, err2, label="Этап 2: |(E(Nt)-F0)/F0|")
plt.plot(x, err4, label="Этап 4: |(E(Nt)-F0)/F0|")
plt.axhline(bound1, linestyle="--", label="1.04 / sqrt(2^B)")
plt.axhline(bound2, linestyle="--", label="1.3 / sqrt(2^B)")
plt.xlabel("Обработано потока (%)")
plt.ylabel("Абсолютная относительная ошибка")
plt.title("Сравнение относительной ошибки (Этап 2 vs Этап 4)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("сравнение_относительной_ошибки.png", dpi=200)
plt.show()