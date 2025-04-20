Small library for sending and receiving HTTP messages for Free Pascal.

Features:

- Does not raise exceptions on 40x and 50x status codes, so you can read the response content
- Only uses portable built-in FreePascal units
- Compiles to a single unit
- Support for HTTP and HTTPS

## How to integrate

To start using the library, you can simply copy the contents of the `src/` directory into your project source code and start using the unit (see `/examples`).

Or you can compile the unit first and copy the generated `PascalWeb.o` and `PascalWeb.ppu` files into your unit path.

I wrote the library using only `.inc` files and a `.pp` file to avoid setting up a package manager.

The `Makefile` is used for automate tasks only.

> [!NOTE]
> Since this library makes use of the OpenSSL unit provided by FPC, you will need to download the OpenSSL 1.0.2 binaries and place them in your project build directory.
>
> You can find those binaries in [the IndySockets git repository](https://github.com/IndySockets/OpenSSL-Binaries).

