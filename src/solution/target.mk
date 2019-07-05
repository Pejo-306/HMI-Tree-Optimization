.PHONY: solution

# Make target of 'solution' executable module.
solution: $(SOLUTION__MAIN_OBJ) $(SOLUTION__OBJ) $(HELPER_OBJ) $(THIRD_PARTY_LIBS_OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN)/$(SOLUTION__EXECUTABLE) $(SOLUTION__MAIN_OBJ) $(SOLUTION__OBJ) $(HELPER_OBJ) $(THIRD_PARTY_LIBS_OBJ) $(LIBS)
	@echo $(LOG_STAMP): $(SOLUTION__MODULE_NAME) executable module compilation successful.

