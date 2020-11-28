add_library(mid
        visitor/visitor.cpp
        walker/dumper/dumper.cpp
        walker/analyzer/sema.cpp walker/analyzer/sema.h walker/analyzer/symbol.cpp walker/analyzer/symbol.h)

target_compile_features(mid PUBLIC cxx_std_17)
add_library(RJIT::mid ALIAS mid)