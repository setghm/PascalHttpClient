# PascalWeb Unit

This is a small and simple library for sending HTTP requests and receiving HTTP responses for Free Pascal.

To start using the library, you can simply copy the contents of the `src/` directory into your project source code and start using the unit (see `/examples`).

Or you can compile the unit first and copy the generated `PascalWeb.o` and `PascalWeb.ppu` files into your unit path.

I wrote the library using only `.inc` files and a `.pp` file to avoid setting up a package manager.

Any automation needed to test/compile the code is inside the `Makefile`.

## OpenSSL

Since this library makes use of the OpenSSL unit provided by FPC, you will need to download the OpenSSL 1.0.2 binaries and place them in your project build directory.

You can find those binaries in [the IndySockets git repository](https://github.com/IndySockets/OpenSSL-Binaries).
