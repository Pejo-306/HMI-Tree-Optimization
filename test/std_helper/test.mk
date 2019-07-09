# C++ source files of 'std_helper' module's tests.
# They are prepended their module's relative path to the main Makefile.
STD_HELPER__TEST_SRC :=
STD_HELPER__TEST_SRC := $(patsubst %,$(TEST)/$(STD_HELPER__MODULE_NAME)/%,$(STD_HELPER__TEST_SRC))

# Object files of 'std_helper' module's tests.
STD_HELPER__TEST_OBJ := $(patsubst $(TEST)/%.cc,$(TEST_OBJ)/%.o,$(STD_HELPER__TEST_SRC))

# Create required subdirectories for module's tests' object files
STD_HELPER__TEST_OBJ_DIRS := $(subst $(TEST),$(TEST_OBJ),$(shell find $(TEST)/$(STD_HELPER__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(STD_HELPER__TEST_OBJ_DIRS),$(shell mkdir -p $(obj_dir)))
# Create required subdirectories for module's tests' binaries
STD_HELPER__TEST_BIN_DIRS := $(subst $(TEST),$(TEST_OUT),$(shell find $(TEST)/$(STD_HELPER__MODULE_NAME)/ -type d))
$(foreach bin_dir,$(STD_HELPER__TEST_BIN_DIRS),$(shell mkdir -p $(bin_dir)))

# Include module's tests as dependencies of the 'test' target.
ALL_TEST_TARGETS += std_helper_test

.PHONY: std_helper_test

# Make target for all of 'std_helper' module's tests.
std_helper_test: $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) $(STD_HELPER__TEST_OBJ) \
		$(STD_HELPER__OBJ) $(HELPER_OBJ)
	@# Create temporary directory for tests' binaries.
	mkdir -p $(TEST_OUT)/$(STD_HELPER__MODULE_NAME)
	@# Compile each test file into its own test binary.
	$(foreach obj, \
			  $(STD_HELPER__TEST_OBJ), \
			  $(CXX) $(CXXFLAGS) -o $(patsubst $(TEST_OBJ)/%.o,$(TEST_OUT)/%,$(obj)) \
			  $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) \
			  $(obj) $(STD_HELPER__OBJ) $(HELPER_OBJ) $(LIBS)$(NEWLINE))
	@echo $(LOG_STAMP): $(STD_HELPER__MODULE_NAME) tests compilation successful.

# Make target for 'std_helper' module's tests' object files.
$(TEST_OBJ)/$(STD_HELPER__MODULE_NAME)/%_test.o: $(TEST)/$(STD_HELPER__MODULE_NAME)/%_test.cc \
		$(SRC)/$(STD_HELPER__MODULE_NAME)/%.cc
	@# Create temporary directory for tests' object files.
	@mkdir -p $(TEST_OBJ)/$(STD_HELPER__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

