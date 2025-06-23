#!/bin/bash
set -e

if [ "$1" = "regenerate" ]; then
  rm -rf build && mkdir build && cd build && cmake ..
fi

if [ "$1" = "build-all" ]; then
  cd build && cmake --build .
fi

if [ "$1" = "build-app" ]; then
  cd build && cmake --build . --target server
fi

if [ "$1" = "build-test" ]; then
  echo "// Auto-generated test runner\n" > main_test.cpp
  
  echo "#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN\n" >> main_test.cpp

  find utilities -type f -name '*Test.h' | while read file; do
    include_path="${file#utilities/}"
    echo "#include <$include_path>" >> main_test.cpp
  done

  find wrappers -type f -name '*Test.h' | while read file; do
    include_path="${file#wrappers/}"
    echo "#include <$include_path>" >> main_test.cpp
  done

  cd build && cmake --build . --target server_test
fi

if [ "$1" = "run" ]; then
  sh ./utils.sh build-app && cd build && ./server
fi

if [ "$1" = "test" ]; then
  sh ./utils.sh build-test && cd build && ./server_test
fi
