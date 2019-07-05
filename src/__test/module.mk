# Name of '__test_module' module.
__TEST_MODULE__MODULE_NAME := __test

# C++ header files of '__test_module' module.
# Each implementation file ('.cc' file) in this module depends on the interface
# which each header provides. Source files are recompiled if any one of these 
# headers is altered.
__TEST_MODULE__INCLUDE := 
__TEST_MODULE__INCLUDE := $(patsubst %,$(INCLUDE)/$(__TEST_MODULE__MODULE_NAME)/%,$(__TEST_MODULE__INCLUDE))

# C++ source files of '__test_module' module.
# They are prepended their module's relative path to the main Makefile.
__TEST_MODULE__SRC := 
__TEST_MODULE__SRC := $(patsubst %,$(SRC)/$(__TEST_MODULE__MODULE_NAME)/%,$(__TEST_MODULE__SRC))

# Object files of '__test_module' helper module.
__TEST_MODULE__OBJ := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(__TEST_MODULE__SRC))

# Create required subdirectories for module's object files
__TEST_MODULE__OBJ_DIRS := $(subst $(SRC),$(OBJ),$(shell find $(SRC)/$(__TEST_MODULE__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(__TEST_MODULE__OBJ_DIRS),$(shell mkdir -p $(obj_dir)))

# Include test module's object files in the project.
TEST_MODULE_OBJ += $(__TEST_MODULE__OBJ)

# Make target for '__test_module' module's object files
$(OBJ)/$(__TEST_MODULE__MODULE_NAME)/%.o: $(SRC)/$(__TEST_MODULE__MODULE_NAME)/%.cc $(__TEST_MODULE__INCLUDE)
	@# Create temporary directory for object files.
	@mkdir -p $(OBJ)/$(__TEST_MODULE__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

