add_library(mid
        ir/ssa.cpp
        ir/use.cpp
        ir/value.cpp
        ir/module.cpp
        visitor/visitor.cpp
        walker/dumper/dumper.cpp
        walker/analyzer/sema.cpp
        walker/irbuilder/irbuilder.cpp
        )

target_compile_features(mid PUBLIC cxx_std_17)
add_library(RJIT::mid ALIAS mid)