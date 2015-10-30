#!/usr/bin/python

import os, sys
log = 'matrixcompare_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log

prog_name = 'matrixcompare'

inp1 = '/home/sirakuzi/solver/models/bak/mm/TEST_MATRIX.mtx'
inp2 = '/home/sirakuzi/solver/models/bak/bak_mm.mtx'

os.system('./' + prog_name + ' -inp1 ' + inp1 + ' -inp2 ' + inp2)
