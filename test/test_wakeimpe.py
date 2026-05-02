import numpy as np
import matplotlib.pyplot as plt
import collective_effects.wakeimpe as we

# wt = we.DipoleWakefield(filename = "./skekblerwy20211222.dat")
# # z = np.arange(-3e-2, 1e-3, 1e-6)
# z = [i for i in np.arange(-3e-2, 1e-3, 1e-6)]
# w = wt(z)
# print(type(z))
# print(type(w))
# plt.plot(z, [i / 1e12 for i in w])
# plt.grid(alpha=0.6)
# plt.show()

sigmaz = 4.60074e-3
zlist = np.arange(-5*sigmaz, sigmaz*0.1, sigmaz*0.001)
freq = np.arange(-20e9, 20e9, 1e6)
wl = we.longitudinal.MonopoleWakefield(filename="./skekblerwl20211222.dat", scale=1)
zl = we.longitudinal.MonopoleImpedance(filename="./skekblerzl20211222.dat", scale=1)
wy = we.transverse.DipoleWakefield(filename="./skekblerwy20211222.dat", twiss_beta=1.0, scale=1.0)
zy = we.transverse.DipoleImpedance(filename="./skekblerzy20211222.dat", twiss_beta=1.0, scale=1.0)

# fig1, ax1 = plt.subplots(figsize=(8, 6))
# fig2, ax2 = plt.subplots(figsize=(8, 6))
fig, axes = plt.subplots(2, 2, figsize=(12, 8))

# ax1.plot(zlist, wl(zlist))
# ax2.plot(zlist, wy(zlist))
axes[0, 0].plot(freq, np.array(zl(freq)).real, label="real")
axes[0, 0].plot(freq, np.array(zl(freq)).imag, label="imag")
axes[1, 0].plot(freq, np.array(zy(freq)).real, label="real")
axes[1, 0].plot(freq, np.array(zy(freq)).imag, label="imag")

zl = we.longitudinal.MonopoleImpedance(wakefield=wl)
zy = we.transverse.DipoleImpedance(wakefield=wy)
axes[0, 1].plot(freq, np.array(zl(freq)).real, label="real")
axes[0, 1].plot(freq, np.array(zl(freq)).imag, label="imag")
axes[1, 1].plot(freq, np.array(zy(freq)).real, label="real")
axes[1, 1].plot(freq, np.array(zy(freq)).imag, label="imag")

axes_flat = axes.flat
for i, ax in enumerate(axes_flat):
    ax.legend()
    ax.grid(alpha=0.6)

fig.tight_layout()
plt.show()

