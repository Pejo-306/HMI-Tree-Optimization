# Name of 'tree' module.
TREE__MODULE_NAME := tree

# C++ header files of 'tree' module.
# Each implementation file ('.cc' file) in this module depends on the interface
# which each header provides. Source files are recompiled if any one of these 
# headers is altered.
TREE__INCLUDE :=
TREE__INCLUDE := $(patsubst %,$(INCLUDE)/$(TREE__MODULE_NAME)/%,$(TREE__INCLUDE))

# C++ source files of 'tree' module.
# They are prepended their module's relative path to the main Makefile.
TREE__SRC :=
TREE__SRC := $(patsubst %,$(SRC)/$(TREE__MODULE_NAME)/%,$(TREE__SRC))

# Object files of 'tree' helper module.
TREE__OBJ := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(TREE__SRC))

# Create required subdirectories for module's object files
TREE__OBJ_DIRS := $(subst $(SRC),$(OBJ),$(shell find $(SRC)/$(TREE__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(TREE__OBJ_DIRS),$(shell mkdir -p $(obj_dir)))

# Include module's object files in the project.
HELPER_OBJ += $(TREE__OBJ)

# Make target for 'tree' module's object files
$(OBJ)/$(TREE__MODULE_NAME)/%.o: $(SRC)/$(TREE__MODULE_NAME)/%.cc $(TREE__INCLUDE)
	@# Create temporary directory for object files.
	@mkdir -p $(OBJ)/$(TREE__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

