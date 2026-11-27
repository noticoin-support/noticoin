<img src="docs/noticoin.png" width="80" alt="Noticoin logo">

# Noticoin

Noticoin is a privacy-focused cryptocurrency powered by RandomX proof of work. This repository contains the blockchain node, command-line wallet, and Wallet RPC service.

[Official website](https://noticoin.com/) · [Desktop wallet](https://github.com/noticoin-support/noticoin-wallet)

## Build on Ubuntu 24.04

```sh
sudo apt update
sudo apt install build-essential cmake ninja-build pkg-config libboost-all-dev libssl-dev \
  libzmq3-dev libsodium-dev libunbound-dev libminiupnpc-dev libreadline-dev libunwind-dev \
  libhidapi-dev libprotobuf-dev protobuf-compiler libusb-1.0-0-dev libexpat1-dev

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DARCH=x86-64 \
  -DMANUAL_SUBMODULES=ON -DBUILD_TESTS=OFF -DBUILD_DOCUMENTATION=OFF -DUSE_DEVICE_TREZOR=OFF
cmake --build build --target daemon simplewallet wallet_rpc_server --parallel
```

## Build on Windows x64

Run these commands in the MSYS2 UCRT64 shell.

```sh
pacman -S --needed make patch \
  mingw-w64-ucrt-x86_64-{gcc,cmake,ninja,pkgconf,boost,openssl,zeromq,libsodium,unbound,hidapi,protobuf,libusb}

cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DARCH=default -DARCH_ID=x86_64 \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DMANUAL_SUBMODULES=ON -DBUILD_TESTS=OFF \
  -DBUILD_DOCUMENTATION=OFF -DUSE_DEVICE_TREZOR=OFF
cmake --build build --target daemon simplewallet wallet_rpc_server --parallel
```

Run the build commands from the repository root. Find `noticoind`, `noticoin-wallet-cli`, and `noticoin-wallet-rpc` in `build/bin/`. Windows executables use the `.exe` suffix.

[License](LICENSE)
