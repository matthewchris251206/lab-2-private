# Tester for Merge Sort (array_sort)
add_executable(GeneralReturnValueChecker general_return_value_checker.c)
target_link_libraries(GeneralReturnValueChecker iss)

# add MergeSortTester and SudokuSolverTester
add_test(NAME MergeSortTester COMMAND GeneralReturnValueChecker ${PROJECT_BINARY_DIR}/asm-prog-assignment/ArraySort)
add_test(NAME SudokuSolverTester COMMAND GeneralReturnValueChecker ${PROJECT_BINARY_DIR}/asm-prog-assignment/Sudoku)
