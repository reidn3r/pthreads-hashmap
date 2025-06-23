import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./../io/output/benchmark.csv")

df["n_threads"] = df["n_threads"].replace({0: "Sequencial"})

threads_unicas = df["n_threads"].unique()

plt.figure(figsize=(12, 6))
for thread in threads_unicas:
    dados = df[df["n_threads"] == thread]
    plt.plot(dados["tamanho_buffer"], dados["tempo_execucao"], marker='o', label=f"{thread} Threads" if thread != "Sequencial" else "Sequencial")

plt.title("Benchmark: Tempo vs Tamanho do Buffer")
plt.xlabel("Tamanho do Buffer (bytes)")
plt.ylabel("Tempo de Execução (segundos)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("./../io/output/grafico_linear.png")
plt.show()

# grafico log
plt.figure(figsize=(12, 6))
for thread in threads_unicas:
    dados = df[df["n_threads"] == thread]
    plt.plot(dados["tamanho_buffer"], dados["tempo_execucao"], marker='o', label=f"{thread} Threads" if thread != "Sequencial" else "Sequencial")

plt.xscale("log")
plt.yscale("log")
plt.title("Benchmark: Tempo vs Tamanho do Buffer")
plt.xlabel("Tamanho do Buffer (bytes)")
plt.ylabel("Tempo de Execução (segundos)")
plt.legend()
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.tight_layout()
plt.savefig("./../io/output/grafico_loglog.png")
plt.show()
