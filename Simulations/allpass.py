import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal


class DelayLine:
    """
    Class for a Delay Line with an integer sample length
    """

    def __init__(self):
        self.__max_delay = int(10*8192)
        self.__buffer = np.zeros(self.__max_delay)
        self.__wp = 0  # write pointer
        self.__rp = 0  # read pointer
        self.__delay_len_samp = 0  # delay length in samples

    def set_delay(self, length_samples):
        """Set the length of the delay line

        Parameters
        ----------
        length_samples : int
            new length of the delay line
        """
        self.__delay_len_samp = int(
            np.floor(np.min([length_samples, self.__max_delay])))

    def write(self, data):
        """Write data to the delay buffer"""
        self.__buffer[self.__wp] = data

    def read(self):
        """Read data from the delay buffer"""
        return self.__buffer[self.__rp]

    def update_ptrs(self):
        """Update the delay line pointers"""
        self.__wp -= 1
        if (self.__wp < 0):  # wrap write ptr
            self.__wp = self.__max_delay - 1

        self.__rp = self.__wp + self.__delay_len_samp
        if(self.__rp > self.__max_delay-1):  # wrap read ptr
            self.__rp -= self.__max_delay
        if(self.__rp < 0):  # make sure ptr doesn't go negative
            self.__rp = 0


class Allpass:
    def __init__(self, N, g):
        self.N = N
        self.g = g
        self.delay = DelayLine()
        self.delay.set_delay(N)

    def process(self, block):
        cpy = np.zeros_like(block)
        for n, x in enumerate(block):
            y = self.delay.read() + -self.g * x
            self.delay.write(self.g * y + x)
            self.delay.update_ptrs()

            cpy[n] = y
        
        return cpy


x = np.zeros(4096); x[0] = 1.0

aps = []
for _ in range(50):
    aps.append(Allpass(5, 0.75))

y = np.copy(x)
for ap in aps:
    y = ap.process(y)

plt.plot(x)
plt.plot(y)

plt.show()
