# SRecord

The SRecord package is a collection of powerful tools for manipulating EPROM load files.

> I wrote SRecord because when I was looking for programs to manipulate EPROM load files, I could not find very many.
> The ones that I could find only did a few of the things I needed. SRecord is written in C++ and polymorphism is
> used to provide the file format flexibility and arbitrary filter chaining. Adding more file formats and filters is
> relatively simple, see the Reference Manual.
> -- *Peter Miller (original author of SRecord)*

The SRecord package understands a number of file formats:

* **Ascii-Hex**: input/output; also known as **ascii-space-hex**
* **ASM**: output only; for embedding data into *Assembler* code
* **Atmel Generic**: input/output; used by *Atmel AVR assembler*
* **BASIC**: output only; for embedding data into *BASIC* code
* **BinaryBinary**: input/output
* **B-Record**: input/output; also known as **Freescale Dragonball bootstrap b-record**
* **C**: output only; for embedding data into *C* code
* **COE**: output only; Extension `.coe`; also known as **Xilinx Coefficient File** format
* **Cosmac**: input/output; also known as **RCA Cosmac Elf** format
* **DEC Binary (XXDP)**: input/output
* **Efinix hex/bit**: input;
* **Elektor Monitor (EMON52)**: input/output
* **Fairchild Fairbug**: input/output
* **Formatted Binary**: input/output
* **Four Packed Code (FPC)**: input/output
* **Hexdump**: output only; a simple hexdump
* **HP64000 Absolute**: input only
* **IDT/sim**: input/output
* **Intel**: input/output; also known as **Intel MCS-86 Object** format
* **Intel Absolute Object Module Format (AOMF)**: input/output
* **Intel 16 (INHX16)**: input/output; also known as **Intel hexadecimal 16**
* **LSI Logic Fast Load**: input/output
* **Logisim**: input/output
* **Memory Initialization Format**: output only; Extension `.mem`; used by *Lattice Semiconductor*
* **MIF**: input/output; used by *Altera* **Memory Initialization File** format
* **MOS Technology**: input/output
* **MIPS-Flash**: input/output
* **Motorola S-Record**: input/output; also known as the **Exorciser**, **Exormacs** or **Exormax**
* **MsBin**: input/output; also known as **Windows CE Binary Image Data** format
* **Needham**: input/output; also known as **Needham Electronics ASCII file** format
* **OS65V**: input/output; also known as **Ohio Scientific hexadecimal** format
* **PPB**: input/output, also known as **Stag Prom Programmer binary**
* **PPX**: input/output, also known as **Stag Prom Programmer hexadecimal**
* **Signetics**: input/output
* **SPASM**: input/output; used by a variety of *PIC* programmers
* **Spectrum**: input/output
* **Tektronix**: input/output
* **Tektronix Extended**: input/output
* **Texas Instruments Tagged**: input/output (both 8 and 16 bit); also known as **TI-tagged** or **TI-SDSMAC**
* **Texas Instruments ti-txt**: input/output; used by bootstrap loader of *TI MSP430*
* **TRS-80**: input/output; used by *The Radio Shack*
* **VHDL**: output only
* **VHDL textio**: output only; suitable for loading with `textio.read()` into a bit_vector
* **Verilog VMEM**: input/output; suitable for loading with `$readmemh()`
* **Wilson**: input/output; mysterious type of EPROM writer

Full documentation and binary packages can be found at the [SRecord website](https://srecord.sourceforge.net/)
