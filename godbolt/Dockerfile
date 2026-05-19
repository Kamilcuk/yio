# Compiler Explorer (Godbolt) for Yio
# Includes Yio library and GCC plugin extension

FROM node:20-slim AS builder

RUN apt-get update && apt-get install -y \
    curl \
    tar \
    make \
    cmake \
    gcc \
    g++ \
    git \
    python3 \
    libgmp-dev \
    libmpfr-dev \
    libmpc-dev \
    && rm -rf /var/lib/apt/lists/*

# Download and extract Compiler Explorer
WORKDIR /compiler-explorer
RUN curl -L https://github.com/compiler-explorer/compiler-explorer/archive/refs/tags/gh-9992.tar.gz | tar xz --strip-components=1
RUN npm install

# Build Yio and the GCC plugin
WORKDIR /yio
COPY . .
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Build GCC extension
WORKDIR /yio/extensions/gcc
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

# Setup Compiler Explorer Runtime
FROM node:20-slim
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    gcc-12-plugin-dev \
    clang \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /compiler-explorer
COPY --from=builder /compiler-explorer .

# Copy Yio artifacts
COPY --from=builder /yio/build/libyio.a /usr/local/lib/
COPY --from=builder /yio/src/yio.h /usr/local/include/
COPY --from=builder /yio/src/yio /usr/local/include/yio
COPY --from=builder /yio/extensions/gcc/build/yio_fstring_plugin.so /usr/local/lib/

# Add configuration to link Yio and use plugin by default
COPY godbolt/c.yio.properties /compiler-explorer/etc/config/c.yio.properties

EXPOSE 10240
CMD ["npm", "run", "serve"]
