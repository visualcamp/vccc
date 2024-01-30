include(CompileWith)

set(_source "
int main() {
static constexpr int x = 1;
static constexpr auto p = __builtin_addressof(x);
}
")

CompileWith("${_source}" VCCC_CONSTEXPR_BUILTIN_ADDRESSOF_CXX14 14)
