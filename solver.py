#!/usr/bin/python

import os, sys
log = 'solver_log.txt'
to_log2 = '>>' + log
to_log1 = '>' + log

prog_name = 'solver'

inpm = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1.mtx'
inpv = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1rhs.mtx'
out  = '/home/sirakuzi/Workspace/solver/models/petsc/tdom1/tdom1sol_new.mtx'

#options = ' -ksp_type preonly -pc_type lu -pc_factor_mat_solver_package superlu_dist -ksp_monitor_short -ksp_gmres_cgs_refinement_type refine_always '
#options = ' -ksp_type preonly -ksp_view -log_summary -options_left -pc_factor_mat_solver_package mumps -pc_type lu '
#options = ' -ksp_monitor_short -ksp_compute_eigenvalues -ksp_converged_reason -help -ksp_max_it 100000 -ksp_rtol 1e-7 -ksp_type cg -ksp_view -log_summary -options_left -pc_type jacobi -vec_type standard -mat_type aij'
#options = ' -ksp_monitor_short -ksp_compute_eigenvalues -mat_view_draw -draw_pause -1 -mat_ascii_output_large -ksp_max_it 100000 -ksp_rtol 1e-7 -ksp_type cg -ksp_view -log_summary -options_left -pc_type jacobi -vec_type standard -mat_type aij'
options = ' -ksp_monitor_short -ksp_compute_eigenvalues -ksp_plot_eigenvalues -mat_view draw -ksp_max_it 100000 -ksp_rtol 1e-7 -ksp_type cg -ksp_view -log_summary -options_left -pc_type jacobi -vec_type standard -mat_type aij'
#options = ' -ksp_monitor_short -ksp_compute_eigenvalues -ksp_max_it 100000 -ksp_rtol 1e-7 -ksp_type cr -ksp_view -log_summary -options_left -pc_type sor -vec_type cusp -mat_type aijcusp'
#options = ' -ksp_monitor_short -ksp_compute_eigenvalues -ksp_max_it 100000 -ksp_rtol 1e-7 -ksp_type cr -ksp_view -log_summary -options_left -pc_type sor -vec_type standard -mat_type aij'
#options = ' -ksp_monitor_short -ksp_gmres_cgs_refinement_type refine_always -ksp_type cr -pc_type jacobi'
#options = ' -ksp_monitor_short -ksp_gmres_cgs_refinement_type refine_always '
#options = ' -mat_view draw -draw_pause -1 -geometry'

#os.system('mpiexec -n 32 -host tesis-master,node-1,node-2,node-3 ./' + prog_name + ' -inpm ' + inpm + ' -inpv ' + inpv + ' -out ' + out + options + to_log1 + ' 2>solver_err.txt')
#os.system('optirun mpirun -n 4 /home/sirakuzi/solver/' + prog_name + ' -inpm ' + inpm + ' -inpv ' + inpv + ' -out ' + out + options)
#os.system('/share/mpi/openmpi-1.5.4-gcc/bin/mpirun -host node24,node25 -n 24 /home/sirakuzi/solver/' + prog_name + ' -inpm ' + inpm + ' -inpv ' + inpv + ' -out ' + out + options)
os.system('/home/sirakuzi/Workspace/solver/bin/' + prog_name + ' -inpm ' + inpm + ' -inpv ' + inpv + ' -out ' + out + options)


# Info

# Preconditioners
#PCNONE                "none"
#PCJACOBI              "jacobi"
#PCSOR                 "sor"
#PCLU                  "lu"
#PCSHELL               "shell"
#PCBJACOBI             "bjacobi"
#PCMG                  "mg"
#PCEISENSTAT           "eisenstat"
#PCILU                 "ilu"
#PCICC                 "icc"
#PCASM                 "asm"
#PCGASM                "gasm"
#PCKSP                 "ksp"
#PCCOMPOSITE           "composite"
#PCREDUNDANT           "redundant"
#PCSPAI                "spai"
#PCNN                  "nn"
#PCCHOLESKY            "cholesky"
#PCPBJACOBI            "pbjacobi"
#PCMAT                 "mat"
#PCHYPRE               "hypre"
#PCPARMS               "parms"
#PCFIELDSPLIT          "fieldsplit"
#PCTFS                 "tfs"
#PCML                  "ml"
#PCGALERKIN            "galerkin"
#PCEXOTIC              "exotic"
#PCCP                  "cp"
#PCBFBT                "bfbt"
#PCLSC                 "lsc"
#PCPYTHON              "python"
#PCPFMG                "pfmg"
#PCSYSPFMG             "syspfmg"
#PCREDISTRIBUTE        "redistribute"
#PCSVD                 "svd"
#PCGAMG                "gamg"
#PCSACUSP              "sacusp"        /* these four run on NVIDIA GPUs using CUSP */
#PCSACUSPPOLY          "sacusppoly"
#PCBICGSTABCUSP        "bicgstabcusp"
#PCAINVCUSP            "ainvcusp"
#PCBDDC                "bddc"
#PCKACZMARZ            "kaczmarz"

#KSP
#KSPRICHARDSON         "richardson"
#KSPCHEBYSHEV          "chebyshev"
#KSPCG                 "cg"
#KSPGROPPCG            "groppcg"
#KSPPIPECG             "pipecg"
#KSPCGNE               "cgne"
#KSPNASH               "nash"
#KSPSTCG               "stcg"
#KSPGLTR               "gltr"
#KSPFCG                "fcg"
#KSPGMRES              "gmres"
#KSPFGMRES             "fgmres"
#KSPLGMRES             "lgmres"
#KSPDGMRES             "dgmres"
#KSPPGMRES             "pgmres"
#KSPTCQMR              "tcqmr"
#KSPBCGS               "bcgs"
#KSPIBCGS              "ibcgs"
#KSPFBCGS              "fbcgs"
#KSPFBCGSR             "fbcgsr"
#KSPBCGSL              "bcgsl"
#KSPCGS                "cgs"
#KSPTFQMR              "tfqmr"
#KSPCR                 "cr"
#KSPPIPECR             "pipecr"
#KSPLSQR               "lsqr"
#KSPPREONLY            "preonly"
#KSPQCG                "qcg"
#KSPBICG               "bicg"
#KSPMINRES             "minres"
#KSPSYMMLQ             "symmlq"
#KSPLCD                "lcd"
#KSPPYTHON             "python"
#KSPGCR                "gcr"

# External Solvers
#MATSOLVERSUPERLU      "superlu"
#MATSOLVERSUPERLU_DIST "superlu_dist"
#MATSOLVERUMFPACK      "umfpack"
#MATSOLVERCHOLMOD      "cholmod"
#MATSOLVERESSL         "essl"
#MATSOLVERLUSOL        "lusol"
#MATSOLVERMUMPS        "mumps"
#MATSOLVERMKL_PARDISO  "mkl_pardiso"
#MATSOLVERMKL_CPARDISO "mkl_cpardiso"
#MATSOLVERPASTIX       "pastix"
#MATSOLVERMATLAB       "matlab"
#MATSOLVERPETSC        "petsc"
#MATSOLVERBAS          "bas"
#MATSOLVERCUSPARSE     "cusparse"
#MATSOLVERBSTRM        "bstrm"
#MATSOLVERSBSTRM       "sbstrm"
#MATSOLVERELEMENTAL    "elemental"
#MATSOLVERCLIQUE       "clique"
#MATSOLVERKLU          "klu"
