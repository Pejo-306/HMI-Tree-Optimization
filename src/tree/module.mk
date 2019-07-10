# Name of 'tree' module.
TREE__MODULE_NAME := tree

# C++ header files of 'tree' module.
# Each implementation file ('.cc' file) in this module depends on the interface
# which each header provides. Source files are recompiled if any one of these 
# headers is altered.
TREE__INCLUDE := \
	hmi_tree.hh \
	node.hh \
	hmi_exception.hh \
	hmi_objects/hmi_view.hh \
	hmi_objects/hmi_widget.hh \
	hmi_objects/hmi_text.hh \
	cache_entry.hh \
	cache_entries/hmi_view_cache_entry.hh \
	cache_entries/hmi_widget_cache_entry.hh \
	cache_entries/hmi_text_cache_entry.hh
TREE__INCLUDE := $(patsubst %,$(INCLUDE)/$(TREE__MODULE_NAME)/%,$(TREE__INCLUDE))

# C++ source files of 'tree' module.
# They are prepended their module's relative path to the main Makefile.
TREE__SRC := \
	hmi_tree.cc \
	node.cc \
	hmi_exception.cc \
	hmi_objects/hmi_view.cc \
	hmi_objects/hmi_widget.cc \
	hmi_objects/hmi_text.cc \
	cache_entry.cc \
	cache_entries/hmi_view_cache_entry.cc \
	cache_entries/hmi_widget_cache_entry.cc \
	cache_entries/hmi_text_cache_entry.cc
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

