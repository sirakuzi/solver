
/* Program usage:  mpiexec ex23 [-help] [all PETSc options] */

static char help[] = "Compare two vectors from petsc binary(sparse) format.\n\
Input parameters are:\n\
  -inp1 <filename> : first input vector file\n\
  -inp2 <filename> : second input vector file\n\n";

/*T
   Sirakuzi 15 dec 09
   Concepts: compare inp1 and inp2;
   Processors: n
T*/

#include "petscksp.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
  Vec            a, b;      /* approx solution, RHS, exact solution */
  PetscReal      norm, norm1, norm2;         /* norm of solution error */
  PetscErrorCode ierr;
  PetscScalar    neg_one = -1.0;
  char           filein[PETSC_MAX_PATH_LEN];
  PetscViewer    fd;

  ierr = PetscInitialize(&argc,&args,(char *)0,help);
  ierr = PetscOptionsGetString(PETSC_NULL,"-inp1",filein,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,filein,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = VecCreate(PETSC_COMM_WORLD,&a); CHKERRQ(ierr);
  ierr = VecLoad(a,fd); CHKERRQ(ierr); 
  ierr = PetscPrintf(PETSC_COMM_WORLD,"File %s loaded successful.\n",filein); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);

  ierr = PetscOptionsGetString(PETSC_NULL,"-inp2",filein,PETSC_MAX_PATH_LEN-1,PETSC_NULL);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,filein,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = VecCreate(PETSC_COMM_WORLD,&b); CHKERRQ(ierr);
  ierr = VecLoad(b,fd); CHKERRQ(ierr); 
  ierr = PetscPrintf(PETSC_COMM_WORLD,"File %s loaded successful.\n",filein); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);


  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
                      Check solution and clean up
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* 
     Check the error
  */
  ierr = VecNorm(a,NORM_2,&norm1); CHKERRQ(ierr); //Sirakuzi 16dec09: smotrim normu vektora1 (solver)
  ierr = VecNorm(b,NORM_2,&norm2); CHKERRQ(ierr); //Sirakuzi 16dec09: smotrim normu vektora2 (stadyo)
  ierr = VecAXPY(a,neg_one,b); CHKERRQ(ierr);
  ierr = VecNorm(a,NORM_2,&norm); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of error is %.100E\n", norm); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm 1st vector is %.100E\n", norm1); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm 2nd vector is %.100E\n", norm2); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm error/2nd is %.100E\n", norm/norm2); CHKERRQ(ierr);
  /* 
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = VecDestroy(&a); CHKERRQ(ierr); 
  ierr = VecDestroy(&b); CHKERRQ(ierr);

  /*
     Always call PetscFinalize() before exiting a program.  This routine
       - finalizes the PETSc libraries as well as MPI
       - provides summary and diagnostic information if certain runtime
         options are chosen (e.g., -log_summary).
  */
  ierr = PetscFinalize();CHKERRQ(ierr);
  return 0;
}
