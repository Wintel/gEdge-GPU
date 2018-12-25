#!/bin/bash
export LIBGL_DRIVERS_PATH=~/Mesa/mesa-18.0.0_Client/lib
export LD_LIBRARY_PATH=~/Mesa/mesa-18.0.0_Client/lib
LIBGL_DEBUG=verbose ./main.exec 
