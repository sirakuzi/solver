#!/usr/bin/python

import os, sys
log = 'marketm2petsc_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log
inp = '/home/sirakuzi/Workspace/solver/models/'
out = '/home/sirakuzi/Workspace/solver/models/'
prog_name = 'marketm2petsc'

os.system('/home/sirakuzi/Workspace/solver/bin/' + prog_name + ' -fin ' + inp + ' -fout ' + out)
