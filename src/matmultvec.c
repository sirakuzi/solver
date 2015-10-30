	
/* Program usage:  mpiexec ex23 [-help] [all PETSc options] */

static char help[] = "Solves a tridiagonal linear system.\n\n";

#include "petscksp.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  Vec            x, b;      /* approx solution, RHS, exact solution */
  Mat            A;            /* linear system matrix */
  PetscErrorCode ierr;
  PetscInt       vecsize,matcolsize,matrowsize;
  char           fileinm[PETSC_MAX_PATH_LEN], fileinv[PETSC_MAX_PATH_LEN], fileout[PETSC_MAX_PATH_LEN];   //Sirakuzi: for input matrix&vector file
  PetscViewer    fd, view;   //Sirakuzi: for input matrix&vector file

  ierr = PetscInitialize(&argc,&args,(char *)0,help); CHKERRQ(ierr);
  ierr = PetscOptionsGetString(PETSC_NULL,"-inpm",fileinm,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileinm,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = MatCreate(PETSC_COMM_WORLD,&A); CHKERRQ(ierr);
  ierr = MatLoad(A,fd); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);
  ierr = MatGetSize(A,&matrowsize,&matcolsize); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"matrowsize %i; ",matrowsize); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"matcolsize %i\n",matcolsize); CHKERRQ(ierr);

  ierr = PetscOptionsGetString(PETSC_NULL,"-inpv",fileinv,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileinv,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = VecCreate(PETSC_COMM_WORLD,&b); CHKERRQ(ierr);
  ierr = VecLoad(b,fd); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);
  ierr = VecGetSize(b,&vecsize); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"vecsize %i\n",vecsize); CHKERRQ(ierr);

  ierr = VecCreate(PETSC_COMM_WORLD,&x); CHKERRQ(ierr);
  ierr = PetscObjectSetName((PetscObject) x, "Solution"); CHKERRQ(ierr);
  ierr = VecSetSizes(x,PETSC_DECIDE,vecsize); CHKERRQ(ierr);
  ierr = VecSetFromOptions(x); CHKERRQ(ierr);

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                MatMult
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ierr = MatMult(A,b,x); CHKERRQ(ierr);
  
// Sirakuzi 16 dec 09: Print to binary file -out
  ierr = PetscOptionsGetString(PETSC_NULL,"-out",fileout,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileout,FILE_MODE_WRITE,&view); CHKERRQ(ierr);
  ierr = VecView(x,view); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&view); CHKERRQ(ierr);

  /*
     View solver info; we could instead use the option -ksp_view to
     print this info to the screen at the conclusion of KSPSolve().
  */


  /*
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = VecDestroy(&x); CHKERRQ(ierr);
  ierr = VecDestroy(&b); CHKERRQ(ierr);
  ierr = MatDestroy(&A); CHKERRQ(ierr);

  /*
     Always call PetscFinalize() before exiting a program.  This routine
       - finalizes the PETSc libraries as well as MPI
       - provides summary and diagnostic information if certain runtime
         options are chosen (e.g., -log_summary).
  */
  ierr = PetscFinalize(); CHKERRQ(ierr);
  return 0;
}
