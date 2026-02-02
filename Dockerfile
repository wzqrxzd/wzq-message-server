FROM debian:trixie

RUN apt update && apt install -y \
  git \
  build-essential \
  cmake \
  libspdlog-dev \
  libpq-dev \
  libssl-dev \
  libargon2-dev \
  libsodium-dev \
  libasio-dev \
  libpqxx-dev \
  nlohmann-json3-dev \
  && rm -rf /var/lib/apt/lists/*


WORKDIR /usr/local/app

RUN mkdir -p libs && \
  git clone --depth 1 https://github.com/Thalhammer/jwt-cpp.git libs/jwt-cpp && \
  git clone --depth 1 https://github.com/CrowCpp/Crow.git libs/crow

COPY . .
EXPOSE 8080

RUN cmake -S . -B build
COPY .env ./build
ENV JWT_SECRET=test
RUN cd build && make -j$(nproc) && ctest -V
