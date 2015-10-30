#!/usr/bin/python

import os, sys
log = 'mmsym2petsc_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log

inp = '/home/sirakuzi/solver/models/bak/mm/bak_mm.mtx'
out = '/home/sirakuzi/solver/models/bak/SYM_MATRIX.mtx'
#inp = '/home/sirakuzi/Programming/solver_cusp/models/tab/tab_mm.txt'
#out = '/home/sirakuzi/Programming/solver_cusp/models/tab/fortest/tab_matrix_petsc.mtx'

prog_name = 'mmsym2petsc'

os.system('/share/mpi/openmpi-1.5.4-gcc/bin/mpirun ./' + prog_name + ' -fin ' + inp + ' -fout ' + out)

