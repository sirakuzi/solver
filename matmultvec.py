#!/usr/bin/python

import os, sys
log = 'matmultvec_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log

prog_name = 'matmultvec'

inpm = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1.mtx'
inpv = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1sol_new.mtx'
out =  '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1_matmulsol.mtx'

os.system('/home/sirakuzi/Workspace/solver/bin/' + prog_name + ' -inpm ' + inpm + ' -inpv ' + inpv + ' -out ' + out + ' refine_always')
