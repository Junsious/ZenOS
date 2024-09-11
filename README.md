# zen-kernel-OS
![2](https://github.com/user-attachments/assets/47cff0b6-14a5-44d6-9dda-248524830bcb)

[![Junsious's github activity graph](https://github-readme-activity-graph.vercel.app/graph?username=[junsious](https://github.com/Junsious/zen-kernel-OS)&theme=rogue&custom_title=Junsious&hide_border=true)](https://github.com/ashutosh00710/github-readme-activity-graph)
## Description
- [RUS_README](https://github.com/Junsious/zen-kernel-OS/blob/main/README_ru.md)

  
- `zen-kernel-OS` is an educational project aimed at creating an operating system from scratch. The project includes a kernel written in both C and Rust, with basic functionality for text output and command input support. This project is intended to provide hands-on experience in operating system development, cross-compilation, and low-level programming. The project will also develop.

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
- **`keyboard_driver.c`** — Driver for your keyboard on C.

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

If you have any questions or suggestions, you can reach out to the project maintainer through GitHub or via email: alexandrd542@gmail.com
