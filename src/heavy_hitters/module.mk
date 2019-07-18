# Name of 'heavy_hitters' module.
HEAVY_HITTERS__MODULE_NAME := heavy_hitters

# C++ header files of 'heavy_hitters' module.
# Each implementation file ('.cc' file) in this module depends on the interface
# which each header provides. Source files are recompiled if any one of these 
# headers is altered.
HEAVY_HITTERS__INCLUDE := \
	cms.hh
HEAVY_HITTERS__INCLUDE := $(patsubst %,$(INCLUDE)/$(HEAVY_HITTERS__MODULE_NAME)/%,$(HEAVY_HITTERS__INCLUDE))

# C++ source files of 'heavy_hitters' module.
# They are prepended their module's relative path to the main Makefile.
HEAVY_HITTERS__SRC := \
	cms.cc
HEAVY_HITTERS__SRC := $(patsubst %,$(SRC)/$(HEAVY_HITTERS__MODULE_NAME)/%,$(HEAVY_HITTERS__SRC))

# Object files of 'heavy_hitters' helper module.
HEAVY_HITTERS__OBJ := $(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(HEAVY_HITTERS__SRC))

# Create required subdirectories for module's object files
HEAVY_HITTERS__OBJ_DIRS := $(subst $(SRC),$(OBJ),$(shell find $(SRC)/$(HEAVY_HITTERS__MODULE_NAME)/ -type d))
$(foreach obj_dir,$(HEAVY_HITTERS__OBJ_DIRS),$(shell mkdir -p $(obj_dir)))

# Include module's object files in the project.
HELPER_OBJ += $(HEAVY_HITTERS__OBJ)

# Make target for 'heavy_hitters' module's object files
$(OBJ)/$(HEAVY_HITTERS__MODULE_NAME)/%.o: $(SRC)/$(HEAVY_HITTERS__MODULE_NAME)/%.cc $(HEAVY_HITTERS__INCLUDE)
	@# Create temporary directory for object files.
	@mkdir -p $(OBJ)/$(HEAVY_HITTERS__MODULE_NAME)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

