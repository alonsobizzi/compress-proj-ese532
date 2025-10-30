#######################################################################################
.PHONY: help
help:
	@echo "Makefile Usage:"
	@echo ""
	@echo "  make cpu"
	@echo "      Command to build mmult_cpu."
	@echo ""
	@echo "  make fpga "
	@echo "      Command to build mmult_fpga."
	@echo ""
	@echo "  make host "
	@echo "      Command to build only the host program of mmult_fpga."
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""
#######################################################################################

HOST_CXX ?= aarch64-linux-gnu-g++
VPP = ${XILINX_VITIS}/bin/v++
RM = rm -f
RMDIR = rm -rf

VITIS_PLATFORM = u96v2_sbc_base
VITIS_PLATFORM_DIR = ${PLATFORM_REPO_PATHS}
VITIS_PLATFORM_PATH = $(VITIS_PLATFORM_DIR)/u96v2_sbc_base.xpfm

# host compiler global settings

CXXFLAGS += -march=armv8-a+simd -mtune=cortex-a53 -std=c++11 -DVITIS_PLATFORM=$(VITIS_PLATFORM) -D__USE_XOPEN2K8 -I$(XILINX_VIVADO)/include/ -I$(VITIS_PLATFORM_DIR)/sw/$(VITIS_PLATFORM)/PetaLinux/sysroot/aarch64-xilinx-linux/usr/include/xrt/ -O2 -g -Wall -c -fmessage-length=0 --sysroot=$(VITIS_PLATFORM_DIR)/sw/$(VITIS_PLATFORM)/PetaLinux/sysroot/aarch64-xilinx-linux

LDFLAGS += -lxilinxopencl -lpthread -lrt -ldl -lcrypt -lstdc++ -L$(VITIS_PLATFORM_DIR)/sw/$(VITIS_PLATFORM)/PetaLinux/sysroot/aarch64-xilinx-linux/usr/lib/ --sysroot=$(VITIS_PLATFORM_DIR)/sw/$(VITIS_PLATFORM)/PetaLinux/sysroot/aarch64-xilinx-linux

# hardware compiler shared settings
VPP_OPTS = --target hw --jobs 24


#
# OpenCL kernel files
#
XO := kernel.xo
XCLBIN := kernel.xclbin
ALL_MESSAGE_FILES = $(subst .xo,.mdb,$(XO)) $(subst .xclbin,.mdb,$(XCLBIN))

#
# host files
#
HOST_SOURCES = ./src/host.cpp ./src/cpu/Scale_SW.cpp ./src/cpu/Differentiate.cpp ./src/cpu/Compress.cpp ./src/common/Utilities.cpp
# src/Host.cpp src/common/*.cpp  src/cpu/*.cpp 
HOST_OBJECTS =$(HOST_SOURCES:.cpp=.o)
HOST_EXE = host

.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.NOTPARALLEL: clean

clean-cpu:
	-$(RM) $(CPU_EXE) $(CPU_OBJECTS) 

clean-host:
	-$(RM) $(HOST_EXE) $(HOST_OBJECTS) 

clean-accelerators:
	-$(RM) $(XCLBIN) $(XO) $(ALL_MESSAGE_FILES)
	-$(RM) *.xclbin.sh *.xclbin.info *.xclbin.link_summary* *.compile_summary
	-$(RMDIR) .Xil fpga/hls/proj_mmult

clean-package:
	-${RMDIR} package
	-${RMDIR} package.build

clean: clean-cpu clean-host clean-accelerators clean-package
	-$(RM) *.log *.package_summary
	-${RMDIR} _x .ipcache

.PHONY: cpu
cpu: $(CPU_EXE)

$(CPU_EXE): $(CPU_OBJECTS)
	$(HOST_CXX) -I./src -I./src/cpu -I./src/common/ -I./fpga/hls/ -o "$@" $(+) $(LDFLAGS)

$(HOST_EXE): $(HOST_OBJECTS)
	$(HOST_CXX) -o "$@" $(+) $(LDFLAGS)

.cpp.o:
	$(HOST_CXX) $(CXXFLAGS) -I./src -I./src/cpu -I./src/common/ -o "$@" "$<"
