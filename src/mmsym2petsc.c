// Test programm 30.04.2012 by Sirakuzi

#if !defined(PETSC_USE_COMPLEX)

static char help[] = "Reads in a Symmetric matrix in MatrixMarket format. Writes\n\
it using the PETSc sparse format. Input parameters are:\n\
    -fin <filename> : input file\n\
    -fout <filename> : output file\n\n";

#include "petscvec.h" 
#include "petscmat.h"
#include "malloc.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
    Mat                A;
    char               filein[PETSC_MAX_PATH_LEN],fileout[PETSC_MAX_PATH_LEN],buf[PETSC_MAX_PATH_LEN],line[40];
    PetscInt           i,m,n,nnz,col,row,*nnz_count;
    PetscErrorCode     ierr;
    PetscMPIInt        size;
    PetscScalar        val;
    FILE*              file;
    PetscViewer        view;

    ierr = PetscInitialize(&argc,&args,(char *)0,help); CHKERRQ(ierr);

    ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size); CHKERRQ(ierr);
    if (size > 1) SETERRQ(PETSC_COMM_WORLD,1,"Uniprocessor Example only\n");

    /* Making array containing the number of nonzeros in the various rows */
    ierr = PetscOptionsGetString(PETSC_NULL,"-fin",filein,PETSC_MAX_PATH_LEN-1,PETSC_NULL);CHKERRQ(ierr);
    ierr = PetscFOpen(PETSC_COMM_WORLD,filein,"r",&file);CHKERRQ(ierr);
    fgets(buf,PETSC_MAX_PATH_LEN-1,file);
    while (buf[0] == '%') {
        printf("%s",buf);
        fgets(buf,PETSC_MAX_PATH_LEN-1,file);
    }
    sscanf(buf,"%d %d %d\n",&m,&n,&nnz);
    printf ("m = %d, n = %d, nnz = %d\n",m,n,nnz);
    nnz_count=(PetscInt*)malloc(m*sizeof(PetscInt));
    if (nnz_count==NULL) printf ("FAIL?");
    for (i=0; i<m; i++) {
        nnz_count[i] = 0;
    }

    ierr = PetscPrintf(PETSC_COMM_SELF,"Making array containing the number of nonzeros in the various rows.\n"); CHKERRQ(ierr);
    for (i=0; i<nnz; i++) {
        fgets(line, 40, file);
        sscanf(line,"%d %d %le\n",&row,&col,(double*)&val);
        row = row-1; col = col-1;
        nnz_count[col]=nnz_count[col]+1; //Transpose the matrix
//        nnz_count[row]=nnz_count[row]+1;
//        if (row != col) {nnz_count[col]=nnz_count[col]+1;}    //If you want make non-symmetric matrix from symmetric
    }

    ierr = PetscFClose(PETSC_COMM_WORLD,file);


    /* Creating the Matrix */
    ierr = PetscPrintf(PETSC_COMM_SELF,"Creating the Matrix.\n"); CHKERRQ(ierr);
    ierr = MatCreate(PETSC_COMM_WORLD,&A); CHKERRQ(ierr);
    ierr = MatSetType(A, MATSBAIJ); CHKERRQ(ierr); //symmetric test
    ierr = MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, m, n); CHKERRQ(ierr);
    ierr = MatSeqSBAIJSetPreallocation(A,1,PETSC_DEFAULT,nnz_count); CHKERRQ(ierr);


    /* Reading Matrix Market file and writing to PETSc Matrix */
    ierr = PetscPrintf(PETSC_COMM_SELF,"Reading Matrix Market file and writing to PETSc Matrix.\n"); CHKERRQ(ierr);
    ierr = PetscFOpen(PETSC_COMM_WORLD,filein,"r",&file);CHKERRQ(ierr);
    fgets(buf,PETSC_MAX_PATH_LEN-1,file);
    while (buf[0] == '%') {
        fgets(buf,PETSC_MAX_PATH_LEN-1,file);
    }

    for (i=0; i<nnz; i++) {
        fgets(line, 40, file);
        sscanf(line,"%d %d %le\n",&row,&col,(double*)&val);
        row = row-1; col = col-1;
        ierr = MatSetValues(A,1,&col,1,&row,&val,INSERT_VALUES); CHKERRQ(ierr); //Transpose the matrix
//        ierr = MatSetValues(A,1,&row,1,&col,&val,INSERT_VALUES); CHKERRQ(ierr);
//        if (row != col) {ierr = MatSetValues(A,1,&col,1,&row,&val,INSERT_VALUES);CHKERRQ(ierr);} //If you want make non-symmetric matrix from symmetric
    }
    ierr = PetscFClose(PETSC_COMM_WORLD,file); 

    /* Finalizing */
    ierr = PetscPrintf(PETSC_COMM_SELF,"Finalizing.\n"); CHKERRQ(ierr);
    ierr = MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
    ierr = MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY); CHKERRQ(ierr);
    ierr = PetscOptionsGetString(PETSC_NULL,"-fout",fileout,PETSC_MAX_PATH_LEN-1,PETSC_NULL); CHKERRQ(ierr);
    ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileout,FILE_MODE_WRITE,&view); CHKERRQ(ierr);
    ierr = MatView(A,view); CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&view); CHKERRQ(ierr);
    ierr = MatDestroy(&A); CHKERRQ(ierr); 
    ierr = PetscFinalize(); CHKERRQ(ierr);
    return 0;
}
#else
#include <stdio.h>
int main(int argc,char **args)
{
    fprintf(stdout,"This example does not work for complex numbers.\n");
    return 0;
}
#endif
