# Functions that determine compiler capabilities

include(CheckCXXSourceCompiles)

# Determines if the selected C++ compiler has deducing this support. Sets
# 'result_var' to whether support is detected.
function(beman_iterator_check_deducing_this result_var)
  check_cxx_source_compiles( "
// clang-specific check due to http://github.com/llvm/llvm-project/issues/113174
#if defined(__cpp_explicit_this_parameter) || (defined(__clang__) && __has_extension(cxx_explicit_this_parameter))
#else
#error No deducing this support
#endif
int main(){}
" HAVE_DEDUCING_THIS )
  set(${result_var} ${HAVE_DEDUCING_THIS} PARENT_SCOPE)
endfunction()
