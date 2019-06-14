#!/usr/bin/env python3
""" Converte o log gerado para um formato unificado.
"""

import fileinput

for line in fileinput.input():
    a = line[:32]
    b = line[46:49]

    print("{} | {}".format(a, b))
