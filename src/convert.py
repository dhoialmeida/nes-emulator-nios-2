#!/usr/bin/env python3
""" Converte o log padrão do nestest para um formato mais próximo deste emulador.
"""

import fileinput
import re

for line in fileinput.input():
    pc = line[0:4].strip()
    opcodes = line[6:14].strip()
    instr = line[16:48].strip()

    if "=" in instr:
        instr = instr[:-5]

    info = line[48:73].strip()
    cyc = line[86:].strip()
    print("{} | {} {:10} | {}".format(pc, info, cyc, instr))
