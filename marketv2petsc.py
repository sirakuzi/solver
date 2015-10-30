#!/usr/bin/python

import os, sys
log = 'marketv2petsc_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log
prog_name = 'marketv2petsc'
inp = '/home/sirakuzi/Programming/solver_cusp/models/tab/tab_rhs_mm.txt'
out = '/home/sirakuzi/Programming/solver_cusp/models/tab/fortest/tab_rhs_mm_petsc.mtx'


os.system('./' + prog_name + ' -fin ' + inp + ' -fout ' + out)
