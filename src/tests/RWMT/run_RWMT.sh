#!/bin/bash

# OpenMP environment variables
if [ "$IN_CONTAINER" = true ]; then
    export OMP_NUM_THREADS=$(nproc)
else
    export OMP_NUM_THREADS=8
fi
export OMP_PROC_BIND=spread
export OMP_PLACES=threads

export KK_TOOLS_DIR=${HOME}/kokkos-tools
export MACHINE=`hostname`

# kernel logger
# export KOKKOS_PROFILE_LIBRARY=${KK_TOOLS_DIR}/kp_kernel_logger.so
# export PATH=${PATH}:${KK_TOOLS_DIR}/

# simple kernel timer location
# export KOKKOS_PROFILE_LIBRARY=${KK_TOOLS_DIR}/kp_kernel_timer.so
# export PATH=${PATH}:${KK_TOOLS_DIR}

export PT_EXE=./../../bin/parPT
export YAML_IN=/tests/RWMT/data/RWMT_input.yaml
export YAML_PTS=../RWMT/data/RWMT_input.yaml

./../utils/gen_pts.py3 --fname=${YAML_PTS}

# run the program and redirect the error output
$PT_EXE $YAML_IN -v > a.out 2> a.err

# kp_reader *.dat > time_data.txt
# # mike's laptop
# rm ${MACHINE}*.dat
