# C++ source files of 'tree' module's tests.
# They are prepended their module's relative path to the main Makefile.
TREE__TEST_SRC :=
TREE__TEST_SRC := $(patsubst %,$(TEST)/$(TREE__MODULE_NAME)/%,$(TREE__TEST_SRC))

# Object files of 'tree' module's tests.
TREE__TEST_OBJ := $(patsubst $(TEST)/%.cc,$(TEST_OBJ)/%.o,$(TREE__TEST_SRC))

# Create required subdirectories for module's tests' object files
TREE__TEST_OBJ_DIRS := $(subst $(TEST),$(TEST_OBJ),$(shell find $(TEST)/$(TREE__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(TREE__TEST_OBJ_DIRS),$(shell mkdir -p $(obj_dir)))
# Create required subdirectories for module's tests' binaries
TREE__TEST_BIN_DIRS := $(subst $(TEST),$(TEST_OUT),$(shell find $(TEST)/$(TREE__MODULE_NAME)/ -type d))
$(foreach bin_dir,$(TREE__TEST_BIN_DIRS),$(shell mkdir -p $(bin_dir)))

# Include module's tests as dependencies of the 'test' target.
ALL_TEST_TARGETS += tree_test

.PHONY: tree_test

# Make target for all of 'tree' module's tests.
tree_test: $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) $(TREE__TEST_OBJ) \
		$(TREE__OBJ) $(HELPER_OBJ)
	@# Create temporary directory for tests' binaries.
	mkdir -p $(TEST_OUT)/$(TREE__MODULE_NAME)
	@# Compile each test file into its own test binary.
	$(foreach obj, \
			  $(TREE__TEST_OBJ), \
			  $(CXX) $(CXXFLAGS) -o $(patsubst $(TEST_OBJ)/%.o,$(TEST_OUT)/%,$(obj)) \
			  $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) \
			  $(obj) $(TREE__OBJ) $(HELPER_OBJ) $(LIBS)$(NEWLINE))
	@echo $(LOG_STAMP): $(TREE__MODULE_NAME) tests compilation successful.

# Make target for 'tree' module's tests' object files.
$(TEST_OBJ)/$(TREE__MODULE_NAME)/%_test.o: $(TEST)/$(TREE__MODULE_NAME)/%_test.cc \
		$(SRC)/$(TREE__MODULE_NAME)/%.cc
	@# Create temporary directory for tests' object files.
	@mkdir -p $(TEST_OBJ)/$(TREE__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

