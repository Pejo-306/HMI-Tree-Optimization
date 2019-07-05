# Name of 'solution' module.
SOLUTION__MODULE_NAME := solution

# Produced executable file name.
SOLUTION__EXECUTABLE := solution

# C++ header files of 'solution' executable module.
# Each implementation file ('.cc' file) in this module depends on the interface
# which each header provides. Source files are recompiled if any one of these 
# headers is altered.
SOLUTION__INCLUDE :=
SOLUTION__INCLUDE := $(patsubst %,$(INCLUDE)/$(SOLUTION__MODULE_NAME)/%,$(SOLUTION__INCLUDE))

# C++ source files of 'solution' executable module.
# NOTE: do not include source file with 'main' function definition (see below).
SOLUTION__SRC :=
SOLUTION__SRC := $(patsubst %,$(SRC)/$(SOLUTION__MODULE_NAME)/%,$(SOLUTION__SRC))
# C++ source file with 'main' function definition.
SOLUTION__MAIN_SRC := main.cc
SOLUTION__MAIN_SRC := $(patsubst %,$(SRC)/$(SOLUTION__MODULE_NAME)/%,$(SOLUTION__MAIN_SRC))

# Object files of 'solution' executable module.
SOLUTION__OBJ := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(SOLUTION__SRC))
SOLUTION__MAIN_OBJ := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(SOLUTION__MAIN_SRC))

# Create required subdirectories for module's object files
SOLUTION__OBJ_DIRS := $(subst $(SRC),$(OBJ),$(shell find $(SRC)/$(SOLUTION__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(SOLUTION__OBJ_DIRS),$(shell mkdir -p $(obj_dir)))

# Add executable module's target as dependency of the 'all' target.
ALL_TARGETS += solution

# Make target for 'solution' module's object files
$(OBJ)/$(SOLUTION__MODULE_NAME)/%.o: $(SRC)/$(SOLUTION__MODULE_NAME)/%.cc $(SOLUTION__INCLUDE)
	@# Create temporary directory for object files.
	@mkdir -p $(OBJ)/$(SOLUTION__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

