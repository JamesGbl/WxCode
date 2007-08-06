
# Include makefile for building a sub directory

# Expects: 
#   SRCS     - list of source files
#   SUBDIR   - Path to directory with source files
#   BINDIR   - Path to output directory
#   CPPBUILD - Build command for C++ files
#   SDFLAGS  - Local flags (for this directory) for compiling

# Flatten path in output bin dir
SDFN := $(subst /,_,$(SUBDIR))
SDFN := $(subst .,-,$(SDFN))
LOBJS = $(addprefix $(BINDIR)/$(SDFN)_, $(SRCS:.cpp=.o))
OBJS += $(LOBJS) $(LCOBJS)

# Build cpp files
-include $(addsuffix .d,$(LOBJS))
$(BINDIR)/$(SDFN)_%.o: tspec:=$(SDFLAGS) 
$(BINDIR)/$(SDFN)_%.o: $(SUBDIR)/%.cpp 
	$(CPP) $(CPPFLAGS) $(tspec) $(DEFS) $(INCZPCH) $(wxCPPFLAGS) -MT$@ -MF$@.d -MD -c $< -o $@
	echo -

# Reset local compile flags after using it
SDFLAGS = 
SRCS=


# Addition for DynObj header processing
PDO_HDRS:= $(addprefix $(SUBDIR)/, $(DO_HDRS))
PDO_SRCS:= $(addprefix $(SUBDIR)/, $(DO_SRCS))
DO_HDRS:=
DO_SRCS:=
DO_ACC_SRCS+= $(PDO_SRCS)
DO_ACC_HDRS+= $(PDO_HDRS)



