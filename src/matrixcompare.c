
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
  Mat            A, B;      /* approx solution, RHS, exact solution */
  PetscReal      norm;         /* norm of solution error */
  PetscErrorCode ierr;
  PetscScalar    neg_one = -1.0;
  char           filein[PETSC_MAX_PATH_LEN];
  PetscViewer    fd;

//  PetscInt       col,row;
//  PetscScalar    val;

  ierr = PetscInitialize(&argc,&args,(char *)0,help); CHKERRQ(ierr);

  ierr = PetscOptionsGetString(PETSC_NULL,"-inp1",filein,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,filein,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = MatCreate(PETSC_COMM_WORLD,&A); CHKERRQ(ierr);
  ierr = MatSetFromOptions(A); CHKERRQ(ierr);
  ierr = MatLoad(A,fd); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"File %s loaded successful.\n",filein); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);

  ierr = PetscOptionsGetString(PETSC_NULL,"-inp2",filein,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
  ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,filein,FILE_MODE_READ,&fd); CHKERRQ(ierr);
  ierr = MatCreate(PETSC_COMM_WORLD,&B); CHKERRQ(ierr);
  ierr = MatSetFromOptions(B); CHKERRQ(ierr);
  ierr = MatLoad(B,fd); CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"File %s loaded successful.\n",filein); CHKERRQ(ierr);
  ierr = PetscViewerDestroy(&fd); CHKERRQ(ierr);


/*// Portim matrix
  col=100;
  row=100;
  val=9.802173018455505E+15;
  ierr = MatSetValues(a,1,&row,1,&col,&val,INSERT_VALUES);CHKERRQ(ierr);

  ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
  ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);CHKERRQ(ierr);
*/

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
                      Check solution and clean up
     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* 
     Check the error
  */
  ierr = MatNorm(A,NORM_1,&norm);CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of 1st matrix error is %A\n", norm);CHKERRQ(ierr);

  ierr = MatNorm(B,NORM_1,&norm);CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of 2nd matrix error is %A\n", norm);CHKERRQ(ierr);

  ierr = MatAXPY(A,neg_one,B,DIFFERENT_NONZERO_PATTERN);CHKERRQ(ierr);
  ierr = MatNorm(A,NORM_1,&norm);CHKERRQ(ierr);
  ierr = PetscPrintf(PETSC_COMM_WORLD,"Norm of A-B error is %A\n", norm);CHKERRQ(ierr);
  /* 
     Free work space.  All PETSc objects should be destroyed when they
     are no longer needed.
  */
  ierr = MatDestroy(&A);CHKERRQ(ierr); 
  ierr = MatDestroy(&B);CHKERRQ(ierr);

  /*
     Always call PetscFinalize() before exiting a program.  This routine
       - finalizes the PETSc libraries as well as MPI
       - provides summary and diagnostic information if certain runtime
         options are chosen (e.g., -log_summary).
  */
  ierr = PetscFinalize();CHKERRQ(ierr);
  return 0;
}
