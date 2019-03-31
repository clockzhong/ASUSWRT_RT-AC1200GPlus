#!/bin/bash
export ASUSWRT_ROOT=`pwd`
export PATH=$PATH:$ASUSWRT_ROOT/release/src-rt-9.x/src/toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/bin
#export SRCBASE=$ASUSWRT_ROOT/release/src-rt-9.x/src
SRC=$ASUSWRT_ROOT/release/src-rt-9.x/src
#export STAGING_DIR=$ASUSWRT_ROOT/release/src-rt-9.x/src/toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3
#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ASUSWRT_ROOT/release/src-rt-9.x/src/toolchains/hndtools-arm-linux-2.6.36-uclibc-4.5.3/lib
#export CC=arm-uclibc-linux-2.6.36-gcc
#export CXX=arm-uclibc-linux-2.6.36-g++
#export AR=arm-uclibc-linux-2.6.36-ar
#export RANLIB=arm-uclibc-linux-2.6.36-ranlib
alias python=python2
alias python-config=python2-config