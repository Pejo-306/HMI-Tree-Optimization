# C++ source files of 'heavy_hitters' module's tests.
# They are prepended their module's relative path to the main Makefile.
HEAVY_HITTERS__TEST_SRC :=
HEAVY_HITTERS__TEST_SRC := $(patsubst %,$(TEST)/$(HEAVY_HITTERS__MODULE_NAME)/%,$(HEAVY_HITTERS__TEST_SRC))

# Object files of 'heavy_hitters' module's tests.
HEAVY_HITTERS__TEST_OBJ := $(patsubst $(TEST)/%.cc,$(TEST_OBJ)/%.o,$(HEAVY_HITTERS__TEST_SRC))

# Create required subdirectories for module's tests' object files
HEAVY_HITTERS__TEST_OBJ_DIRS := $(subst $(TEST),$(TEST_OBJ),$(shell find $(TEST)/$(HEAVY_HITTERS__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(HEAVY_HITTERS__TEST_OBJ_DIRS),$(shell mkdir -p $(obj_dir)))
# Create required subdirectories for module's tests' binaries
HEAVY_HITTERS__TEST_BIN_DIRS := $(subst $(TEST),$(TEST_OUT),$(shell find $(TEST)/$(HEAVY_HITTERS__MODULE_NAME)/ -type d))
$(foreach bin_dir,$(HEAVY_HITTERS__TEST_BIN_DIRS),$(shell mkdir -p $(bin_dir)))

# Include module's tests as dependencies of the 'test' target.
ALL_TEST_TARGETS += heavy_hitters_test

.PHONY: heavy_hitters_test

# Make target for all of 'heavy_hitters' module's tests.
heavy_hitters_test: $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) $(HEAVY_HITTERS__TEST_OBJ) \
		$(HEAVY_HITTERS__OBJ) $(HELPER_OBJ)
	@# Create temporary directory for tests' binaries.
	mkdir -p $(TEST_OUT)/$(HEAVY_HITTERS__MODULE_NAME)
	@# Compile each test file into its own test binary.
	$(foreach obj, \
			  $(HEAVY_HITTERS__TEST_OBJ), \
			  $(CXX) $(CXXFLAGS) -o $(patsubst $(TEST_OBJ)/%.o,$(TEST_OUT)/%,$(obj)) \
			  $(TEST_CONFIG_OBJ) $(TEST_MODULE_OBJ) \
			  $(obj) $(HEAVY_HITTERS__OBJ) $(HELPER_OBJ) $(LIBS)$(NEWLINE))
	@echo $(LOG_STAMP): $(HEAVY_HITTERS__MODULE_NAME) tests compilation successful.

# Make target for 'heavy_hitters' module's tests' object files.
$(TEST_OBJ)/$(HEAVY_HITTERS__MODULE_NAME)/%_test.o: $(TEST)/$(HEAVY_HITTERS__MODULE_NAME)/%_test.cc \
		$(SRC)/$(HEAVY_HITTERS__MODULE_NAME)/%.cc
	@# Create temporary directory for tests' object files.
	@mkdir -p $(TEST_OBJ)/$(HEAVY_HITTERS__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

