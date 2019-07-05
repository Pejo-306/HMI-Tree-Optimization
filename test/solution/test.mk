# C++ source files of 'solution' module's tests.
# They are prepended their module's relative path to the main Makefile.
SOLUTION__TEST_SRC :=
SOLUTION__TEST_SRC := $(patsubst %,$(TEST)/$(SOLUTION__MODULE_NAME)/%,$(SOLUTION__TEST_SRC))

# Object files of 'solution' module's tests.
SOLUTION__TEST_OBJ := $(patsubst $(TEST)/%.cc,$(TEST_OBJ)/%.o,$(SOLUTION__TEST_SRC))

# Create required subdirectories for module's tests' object files
SOLUTION__TEST_OBJ_DIRS := $(subst $(TEST),$(TEST_OBJ),$(shell find $(TEST)/$(SOLUTION__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(SOLUTION__TEST_OBJ_DIRS),$(shell mkdir -p $(obj_dir)))
# Create required subdirectories for module's tests' binaries
SOLUTION__TEST_BIN_DIRS := $(subst $(TEST),$(TEST_OUT),$(shell find $(TEST)/$(SOLUTION__MODULE_NAME)/ -type d))
$(foreach bin_dir,$(SOLUTION__TEST_BIN_DIRS),$(shell mkdir -p $(bin_dir)))

# Include module's tests as dependencies of the 'test' target.
ALL_TEST_TARGETS += solution_test

.PHONY: solution_test

# Make target for all of 'solution' module's tests.
solution_test: $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) $(SOLUTION__TEST_OBJ) \
		$(SOLUTION__OBJ) $(HELPER_OBJ)
	@# Create temporary directory for tests' binaries.
	mkdir -p $(TEST_OUT)/$(SOLUTION__MODULE_NAME)
	@# Compile each test file into its own test binary.
	$(foreach obj, \
			  $(SOLUTION__TEST_OBJ), \
			  $(CXX) $(CXXFLAGS) -o $(patsubst $(TEST_OBJ)/%.o,$(TEST_OUT)/%,$(obj)) \
			  $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) \
			  $(obj) $(SOLUTION__OBJ) $(HELPER_OBJ) $(LIBS)$(NEWLINE))
	@echo $(LOG_STAMP): $(SOLUTION__MODULE_NAME) tests compilation successful.

# Make target for 'solution' module's tests' object files.
$(TEST_OBJ)/$(SOLUTION__MODULE_NAME)/%_test.o: $(TEST)/$(SOLUTION__MODULE_NAME)/%_test.cc \
		$(SRC)/$(SOLUTION__MODULE_NAME)/%.cc
	@# Create temporary directory for tests' object files.
	@mkdir -p $(TEST_OBJ)/$(SOLUTION__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

