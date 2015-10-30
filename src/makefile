#Sirakuzi 30 oct 15

CFLAGS = -I${PETSC_DIR}/src/dm/mesh/sieve ${BOOST_INCLUDE} ${TRIANGLE_INCLUDE} ${TETGEN_INCLUDE}
PETSC_DIR = /opt/petsc/linux-c-opt/lib/petsc

include ${PETSC_DIR}/conf/variables
include ${PETSC_DIR}/conf/rules

all: solver marketm2petsc marketv2petsc mmsym2petsc matrixcompare matmultvec vectorcompare

solver: solver.o chkopts
	-${CLINKER} -o ../bin/solver solver.o  ${PETSC_KSP_LIB}
	${RM} solver.o

marketm2petsc: marketm2petsc.o chkopts
	-${CLINKER} -o ../bin/marketm2petsc marketm2petsc.o ${PETSC_MAT_LIB}
	${RM} marketm2petsc.o

marketv2petsc: marketv2petsc.o chkopts
	-${CLINKER} -o ../bin/marketv2petsc marketv2petsc.o ${PETSC_MAT_LIB}
	${RM} marketv2petsc.o

mmsym2petsc: mmsym2petsc.o chkopts
	-${CLINKER} -o ../bin/mmsym2petsc mmsym2petsc.o ${PETSC_MAT_LIB}
	${RM} mmsym2petsc.o

matrixcompare: matrixcompare.o  chkopts
	-${CLINKER} -o ../bin/matrixcompare matrixcompare.o  ${PETSC_KSP_LIB}
	${RM} matrixcompare.o

matmultvec: matmultvec.o  chkopts
	-${CLINKER} -o ../bin/matmultvec matmultvec.o  ${PETSC_KSP_LIB}
	${RM} matmultvec.o

vectorcompare: vectorcompare.o  chkopts
	-${CLINKER} -o ../bin/vectorcompare vectorcompare.o  ${PETSC_KSP_LIB}
	${RM} vectorcompare.o
