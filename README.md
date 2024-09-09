# zen-kernel-OS

## Description
- [RUS_README](https://github.com/Junsious/zen-kernel-OS/blob/main/README_ru.md)

  
`zen-kernel-OS` is an educational project aimed at creating an operating system from scratch. The project includes a kernel written in both C and Rust, with basic functionality for text output and command input support. This project is intended to provide hands-on experience in operating system development, cross-compilation, and low-level programming.

## Building and Running

### Requirements

- **Cross-compiler**: `i386-elf-gcc` and `i386-elf-as`
- **ISO Image Creation Tool**: `grub-mkrescue`
- **Emulator**: `QEMU`

### Build

1. **Clone the repository**:

    ```bash
    git clone https://github.com/yourusername/zen-kernel-OS.git
    cd zen-kernel-OS
    ```

2. **Build the project**:

    ```bash
    make
    ```

3. **Run the ISO image with QEMU**:

    ```bash
    qemu-system-i386 -cdrom ZenOS.iso
    ```

### Project Structure

- **`kernel.c`** — Kernel source code in C.
- **`boot.asm`** — Assembler code for the bootloader.
- **`linker.ld`** — Linker script used for building the kernel.
- **`grub.cfg`** — GRUB configuration file for loading the kernel.
- **`Makefile`** — Build automation file.

## Development and Contributing

If you would like to contribute to the project, follow these steps:

1. **Create a branch**:

    ```bash
    git checkout -b feature-branch
    ```

2. **Make your changes** and test them.

3. **Commit your changes**:

    ```bash
    git commit -am 'Added new feature'
    ```

4. **Push your changes**:

    ```bash
    git push origin feature-branch
    ```

5. **Create a Pull Request** on GitHub, describing your changes and their purpose.

## License

This project is licensed under the GNU General Public License v3.0 (GPL-3.0). See [LICENSE](LICENSE) for more details.

## Contact

If you have any questions or suggestions, you can reach out to the project maintainer through GitHub or via email: your-email@example.com.
