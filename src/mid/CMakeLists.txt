add_library(mid
        visitor/visitor.cpp
        walker/dumper/dumper.cpp
        walker/analyzer/sema.cpp
        walker/analyzer/sema.h
        ir/value.cpp
        ir/use.cpp
        ir/ssa.cpp
        )

target_compile_features(mid PUBLIC cxx_std_17)
add_library(RJIT::mid ALIAS mid)