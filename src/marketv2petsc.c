// Last modification 28.09.2011 by Sirakuzi

#if !defined(PETSC_USE_COMPLEX)

static char help[] = "Reads in a Vector in MatrixMarket format. Writes\n\
it using the PETSc sparse format. Input parameters are:\n\
    -fin <filename> : input file\n\
    -fout <filename> : output file\n\n";

#include "petscmat.h"

#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **args)
{
    Vec                b;
    char               filein[PETSC_MAX_PATH_LEN],fileout[PETSC_MAX_PATH_LEN],fileout1[PETSC_MAX_PATH_LEN],buf[PETSC_MAX_PATH_LEN];
    PetscInt           i,m,n,nnz,col,row,col_check;
    PetscErrorCode     ierr;
    PetscMPIInt        size;
    PetscScalar        val;
    FILE*              file;
    PetscViewer        view;

    PetscInitialize(&argc,&args,(char *)0,help);

    ierr = MPI_Comm_size(PETSC_COMM_WORLD,&size);CHKERRQ(ierr);
    if (size > 1) SETERRQ(PETSC_COMM_WORLD,1,"Uniprocessor Example only\n");

    ierr = PetscOptionsGetString(PETSC_NULL,"-fin",filein,PETSC_MAX_PATH_LEN-1,PETSC_NULL);CHKERRQ(ierr);
    ierr = PetscFOpen(PETSC_COMM_SELF,filein,"r",&file);CHKERRQ(ierr);

    /* Ignore the first line */    
    /* while (getc(file) != '\n'); */
    fgets(buf,PETSC_MAX_PATH_LEN-1,file);
    while (buf[0] == '%') {
        printf("%s",buf);
        fgets(buf,PETSC_MAX_PATH_LEN-1,file);
    }

    sscanf(buf,"%d %d %d\n",&m,&n,&nnz);
    printf ("m = %d, n = %d, nnz = %d\n",m,n,nnz);

    ierr = PetscOptionsGetString(PETSC_NULL,"-fout",fileout,PETSC_MAX_PATH_LEN-1,PETSC_NULL);CHKERRQ(ierr);

    ierr = VecCreate(PETSC_COMM_WORLD,&b);CHKERRQ(ierr);
    ierr = VecSetSizes(b,PETSC_DECIDE,m);CHKERRQ(ierr);
    ierr = VecSetFromOptions(b);CHKERRQ(ierr);


    for (i=0; i<nnz; i++) {
        fscanf(file,"%d %d %le\n",&row,&col,(double*)&val);
        if (i == 0) col_check = col;
        row = row-1; // col = col-1 ;
        if (col != col_check) {
            ierr = VecAssemblyBegin(b);CHKERRQ(ierr);
            ierr = VecAssemblyEnd(b);CHKERRQ(ierr);
            printf ("Reading %d vector complete.\n",col_check);
            sprintf(fileout1, "%s_%d.mtx", fileout, col_check);
            ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileout1,FILE_MODE_WRITE,&view);CHKERRQ(ierr);
            ierr = VecView(b,view);CHKERRQ(ierr);
            ierr = PetscViewerDestroy(&view);CHKERRQ(ierr);
            ierr = VecDestroy(&b);CHKERRQ(ierr);
            col_check = col;
            ierr = VecCreate(PETSC_COMM_WORLD,&b);CHKERRQ(ierr);
            ierr = VecSetSizes(b,PETSC_DECIDE,m);CHKERRQ(ierr);
            ierr = VecSetFromOptions(b);CHKERRQ(ierr);
        }
        ierr = VecSetValues(b,1,&row,&val,INSERT_VALUES);CHKERRQ(ierr);
    }
    fclose(file);

    ierr = VecAssemblyBegin(b);CHKERRQ(ierr);
    ierr = VecAssemblyEnd(b);CHKERRQ(ierr);

    //printf ("Reading %d vector complete.\n",col_check); //
    ierr = PetscPrintf(PETSC_COMM_SELF,"Reading %d vector complete.\n", col_check);CHKERRQ(ierr);
    sprintf(fileout1, "%s_%d.mtx", fileout, col_check);
    ierr = PetscViewerBinaryOpen(PETSC_COMM_WORLD,fileout1,FILE_MODE_WRITE,&view);CHKERRQ(ierr);
    ierr = VecView(b,view);CHKERRQ(ierr);
    ierr = PetscViewerDestroy(&view);CHKERRQ(ierr);
    ierr = VecDestroy(&b);CHKERRQ(ierr);
    ierr = PetscFinalize();CHKERRQ(ierr);
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
