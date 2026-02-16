cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Debug\
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DENABLE_RUN=1 \
  -DENABLE_DRUN=1 \
  -DENABLE_DMENU=1
cmake --build build
