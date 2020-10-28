add_library(front
        lexer.cpp
        token.cpp
        )

target_compile_features(front PUBLIC cxx_std_17)
add_library(RJIT::front ALIAS front)