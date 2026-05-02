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
wl1 = we.longitudinal.MonopoleWakefield(filename="./skekblerwl20211222.dat", scale=1)
zl1 = we.longitudinal.MonopoleImpedance(filename="./skekblerzl20211222.dat", scale=1)
wy1 = we.transverse.DipoleWakefield(filename="./skekblerwy20211222.dat", twiss_beta=1.0, scale=1.0)
zy1 = we.transverse.DipoleImpedance(filename="./skekblerzy20211222.dat", twiss_beta=1.0, scale=1.0)

# fig1, axes1 = plt.subplots(2, 2, figsize=(12, 8))
fig2, axes2 = plt.subplots(2, 2, figsize=(12, 8))

# ax1.plot(zlist, wl(zlist))
# ax2.plot(zlist, wy(zlist))
# axes1[0, 0].plot(freq, np.array(zl1(freq)).real, label="real")
# axes1[0, 0].plot(freq, np.array(zl1(freq)).imag, label="imag")
# axes1[1, 0].plot(freq, np.array(zy1(freq)).real, label="real")
# axes1[1, 0].plot(freq, np.array(zy1(freq)).imag, label="imag")
axes2[0, 0].plot(zlist, wl1(zlist))
axes2[1, 0].plot(zlist, wy1(zlist))

wl = we.longitudinal.MonopoleWakefield(impedance=zl1)
# zl = we.longitudinal.MonopoleImpedance(wakefield=wl1)
wy = we.transverse.DipoleWakefield(impedance=zy1)
# zy = we.transverse.DipoleImpedance(wakefield=wy1)
# axes1[0, 1].plot(freq, np.array(zl(freq)).real, label="real")
# axes1[0, 1].plot(freq, np.array(zl(freq)).imag, label="imag")
# axes1[1, 1].plot(freq, np.array(zy(freq)).real, label="real")
# axes1[1, 1].plot(freq, np.array(zy(freq)).imag, label="imag")
axes2[0, 1].plot(zlist, wl(zlist))
axes2[1, 1].plot(zlist, wy(zlist))

# axes1_flat = axes1.flat
axes2_flat = axes2.flat
# for i, ax in enumerate(axes1_flat):
#     ax.legend()
#     ax.grid(alpha=0.6)
for i, ax in enumerate(axes2_flat):
    ax.legend()
    ax.grid(alpha=0.6)

# fig1.tight_layout()
fig2.tight_layout()
plt.show()

