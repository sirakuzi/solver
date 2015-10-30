	
/* Program usage:  mpiexec ex23 [-help] [all PETSc options] */

static char help[] = "Solves a tridiagonal linear system.\n\n";

#include "petscksp.h"
#include "petsctime.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  Vec            x, b;      /* approx solution, RHS, exact solution */
  Mat            A;            /* linear system matrix */
  KSP            ksp;         /* linear solver context */
  PC             pc;           /* preconditioner context */
  PetscErrorCode ierr;
  PetscInt       vecsize,matcolsize,matrowsize;
  char           fileinm[PETSC_MAX_PATH_LEN], fileinv[PETSC_MAX_PATH_LEN], fileout[PETSC_MAX_PATH_LEN];   //Sirakuzi: for input matrix&vector file
  PetscViewer    fd, view;   //Sirakuzi: for input matrix&vector file
  PetscLogDouble starttime,endtime;

  ierr = PetscInitialize(&argc,&args,(char *)0,help); CHKERRQ(ierr);
  ierr = PetscTime(&starttime);CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"\nReading input files start...\n"); CHKERRQ(ierr);

  ierr = PetscOptionsGetString(PETSC_NULL,"-inpm",fileinm,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileinm,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = MatCreate(PETSC_COMM_WORLD,&A); CHKERRQ(ierr);
  ierr = MatSetFromOptions(A); CHKERRQ(ierr);
  ierr = MatLoad(A,fd); CHKERRQ(ierr); //Sirakuzi: MATAIJ is identical to MATSEQAIJ when constructed with a single process communicator, and MATMPIAIJ otherwise
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);
//  matcolsize = PETSC_NULL;
//  matrowsize = PETSC_NULL;
  ierr = MatGetSize(A,&matrowsize,&matcolsize); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"matrowsize %i; ",matrowsize); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"matcolsize %i\n",matcolsize); CHKERRQ(ierr);

  ierr = PetscOptionsGetString(PETSC_NULL,"-inpv",fileinv,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileinv,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = VecCreate(PETSC_COMM_WORLD,&b); CHKERRQ(ierr);
  ierr = VecSetFromOptions(b); CHKERRQ(ierr);
  ierr = VecLoad(b,fd); CHKERRQ(ierr); //Sirakuzi: PETSC_NULL indicates using VECSEQ if the communicator in the Viewer is of size 1; otherwise use VECMPI
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);
  ierr = VecGetSize(b,&vecsize); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"vecsize %i\n",vecsize); CHKERRQ(ierr);

  ierr = PetscPrintf(PETSC_COMM_WORLD,"Reading input files end...\n"); CHKERRQ(ierr);

  ierr = VecCreate(PETSC_COMM_WORLD,&x); CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) x, "Solution"); CHKERRQ(ierr);
  ierr = VecSetSizes(x,PETSC_DECIDE,vecsize); CHKERRQ(ierr);
  ierr = VecSetFromOptions(x); CHKERRQ(ierr);

//  ierr = VecGetOwnershipRange(x,&rstart,&rend);CHKERRQ(ierr);   //Sirakuzi: vect parallel
//  ierr = VecGetLocalSize(x,&nlocal);CHKERRQ(ierr);   //Sirakuzi: vect parallel

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                Create the linear solver and set various options
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*
     Create linear solver context
  */
  ierr = PetscPrintf(PETSC_COMM_WORLD,"\nStart solving system...\n"); CHKERRQ(ierr);
  ierr = KSPCreate(PETSC_COMM_WORLD,&ksp); CHKERRQ(ierr);

  /*
     Set operators. Here the matrix that defines the linear system
     also serves as the preconditioning matrix.
  */
  //ierr = KSPSetOperators(ksp,A,A,DIFFERENT_NONZERO_PATTERN); CHKERRQ(ierr);
  ierr = KSPSetOperators(ksp,A,A); CHKERRQ(ierr);

  /*
     Set linear solver defaults for this problem (optional).
     - By extracting the KSP and PC contexts from the KSP context,
       we can then directly call any KSP and PC routines to set
       various options.
     - The following four statements are optional; all of these
       parameters could alternatively be specified at runtime via
       KSPSetFromOptions();
  */

  ierr = KSPGetPC(ksp,&pc); CHKERRQ(ierr);
  ierr = PCSetType(pc,PCJACOBI); CHKERRQ(ierr);

  ierr = KSPSetTolerances(ksp,1.e-7,PETSC_DEFAULT,PETSC_DEFAULT,PETSC_DEFAULT); CHKERRQ(ierr);


  /*
    Set runtime options, e.g.,
        -ksp_type <type> -pc_type <type> -ksp_monitor -ksp_rtol <rtol>
    These options will override those specified above as long as
    KSPSetFromOptions() is called _after_ any other customization
    routines.
  */
  ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                      Solve the linear system
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /*
     Solve linear system
  */
  ierr = KSPSolve(ksp,b,x); CHKERRQ(ierr);
  

//  CHKERRQ(PetscPrintf(PETSC_COMM_WORLD,"\n\nmatrix:\n"));
//  MatView(A,PETSC_VIEWER_STDOUT_WORLD);

//  CHKERRQ(PetscPrintf(PETSC_COMM_WORLD,"\n\nrhs:\n"));
//  VecView(b,PETSC_VIEWER_STDOUT_WORLD);

//  CHKERRQ(PetscPrintf(PETSC_COMM_WORLD,"\n\nsolution:\n"));
//  VecView(x,PETSC_VIEWER_STDOUT_WORLD);

  ierr = PetscPrintf(PETSC_COMM_WORLD,"\nEnd solving system...\n"); CHKERRQ(ierr);

// Sirakuzi 16 dec 09: Print to binary file -out
  ierr = PetscOptionsGetString(PETSC_NULL,"-out",fileout,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileout,FILE_MODE_WRITE,&view); CHKERRQ(ierr);
  ierr = VecView(x,view); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&view); CHKERRQ(ierr);

  /*
     View solver info; we could instead use the option -ksp_view to
     print this info to the screen at the conclusion of KSPSolve().
  */
  ierr = KSPView(ksp,PETSC_VIEWER_STDOUT_WORLD); CHKERRQ(ierr);


  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&b); CHKERRQ(ierr);
  ierr = MatDestroy(&A); CHKERRQ(ierr);
  ierr = KSPDestroy(&ksp); CHKERRQ(ierr);

  ierr = PetscTime(&endtime); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Done. All spent time %le\n",endtime-starttime); CHKERRQ(ierr);

  /*
     Always call PetscFinalize() before exiting a program.  This routine
       - finalizes the PETSc libraries as well as MPI
       - provides summary and diagnostic information if certain runtime
         options are chosen (e.g., -log_summary).
  */
  ierr = PetscFinalize(); CHKERRQ(ierr);
  return 0;
}
