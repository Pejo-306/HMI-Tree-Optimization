# Name of 'std_helper' module.
STD_HELPER__MODULE_NAME := std_helper

# C++ header files of 'std_helper' module.
# Each implementation file ('.cc' file) in this module depends on the interface
# which each header provides. Source files are recompiled if any one of these 
# headers is altered.
STD_HELPER__INCLUDE := \
	split.hh
STD_HELPER__INCLUDE := $(patsubst %,$(INCLUDE)/$(STD_HELPER__MODULE_NAME)/%,$(STD_HELPER__INCLUDE))

# C++ source files of 'std_helper' module.
# They are prepended their module's relative path to the main Makefile.
STD_HELPER__SRC := \
	split.cc
STD_HELPER__SRC := $(patsubst %,$(SRC)/$(STD_HELPER__MODULE_NAME)/%,$(STD_HELPER__SRC))

# Object files of 'std_helper' helper module.
STD_HELPER__OBJ := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(STD_HELPER__SRC))

# Create required subdirectories for module's object files
STD_HELPER__OBJ_DIRS := $(subst $(SRC),$(OBJ),$(shell find $(SRC)/$(STD_HELPER__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(STD_HELPER__OBJ_DIRS),$(shell mkdir -p $(obj_dir)))

# Include module's object files in the project.
HELPER_OBJ += $(STD_HELPER__OBJ)

# Make target for 'std_helper' module's object files
$(OBJ)/$(STD_HELPER__MODULE_NAME)/%.o: $(SRC)/$(STD_HELPER__MODULE_NAME)/%.cc $(STD_HELPER__INCLUDE)
	@# Create temporary directory for object files.
	@mkdir -p $(OBJ)/$(STD_HELPER__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

