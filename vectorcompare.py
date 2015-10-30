#!/usr/bin/python

import os, sys
log = 'vectorcompare_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log

prog_name = 'vectorcompare'

inp1 = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1rhs.mtx'
inp2 = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1_matmulsol.mtx'

os.system('/home/sirakuzi/Workspace/solver/bin/' + prog_name + ' -inp1 ' + inp1 + ' -inp2 ' + inp2 + ' refine_always')
