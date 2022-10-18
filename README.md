# SRecord

The SRecord package is a collection of powerful tools for manipulating EPROM load files.

"I wrote SRecord because when I was looking for programs to manipulate EPROM load files, I could not find very many. The ones that I could find only did a few of the things I needed. SRecord is written in C++ and polymorphism is used to provide the file format flexibility and arbitrary filter chaining. Adding more file formats and filters is relatively simple, see the Reference Manual." -- Peter Miller (original author of SRecord)

The SRecord package understands a number of file formats:

* **Ascii-Hex****: The ascii-hex format is understood for both reading and writing. (Also known as the ascii-space-hex format.)
* **ASM**: It is possible, for output only, to produce a serices of DB statements containing the data. This can be useful for embedding data into assembler programs. This format cannot be read. Atmel Generic This format is produced by the Atmel AVR assembler. It is understood for both reading and writing.
* **BASIC**: It is possible, for output only, to produce a serices of DATA statements cntaining the data. This can be useful for embedding data into BASIC programs. This format cannot be read.
* **BinaryBinary**: files can both be read and written.
* **B-Record**: Files in Freescale Dragonball bootstrap b-record format can be read and written.
* **C**: It is also possible to write a C array declaration which contains the data. This can be useful when you want to embed download data into C programs. This format cannot be read.
* **COE**: The Xilinx Coefficient File Format (.coe) is understood for output only.
* **Cosmac**: The RCA Cosmac Elf format is understood for both reading and writing.
* **DEC Binary**: The DEC Binary (XXDP) format is understood for both reading and writing.
* **Elektor Monitor (EMON52)**: The EMON52 format is understood for both reading and writing.
* **Fairchild Fairbug**: The Fairchild Fairbug format is understood for both reading and writing.
* **Formatted Binary**: The Formatted Binary format is understood for both reading and writing.
* **Four Packed Code (FPC)**: The FPC format is understood for both reading and writing.
* **Hexdump**: It is possible to get a simple hexdump as output.
* **HP64000 Absolute**: The HP64000 Absolute is understood for reading only.
* **IDT/sim**: The IDT/sim binary file format is understood for both reading and writing.
* **Intel**: The Intel hexadecimal format is understood for both reading and writing. (Also known as the Intel MCS-86 Object format.)
* **Intel AOMF**: The Intel Absolute Object Module Format (AOMF) is understood for both reading and writing.
* **Intel 16**:  The Intel hexadecimal 16 format is understood for both reading and writing. (Also known as the INHX16 file format.)
* **LSI Logic Fast Load**: The LSI Logic Fast Load format is understood for both reading and writing. Logisim The Logisim format is understood for both reading and writing. See srec_logisim(5) for more information.
* **Memory Initialization Format**: The Memory Initialization Format (.mem) by Lattice Semiconductor is understood for writing only.
* **MIF**: The Memory Initialization File format by Altera is supported for both reading and writing.
* **MOS Technology**: The MOS Technology hexadecimal format is understood for both reading and writing.
* **MIPS-Flash**: The MIPS Flash file format is supported for both reading and writing.
* **Motorola S-Record**: The Motorola hexadecimal S-Record format is understood for both reading and writing. (Also known as the Exorciser, Exormacs or Exormax format.)
* **MsBin**: The Windows CE Binary Image Data Format is supported both for reading and writing.
* **Needham**: The Needham Electronics ASCII file format is understood for both reading and writing.
* **OS65V**: The Ohio Scientific hexadecimal format is understood for both reading and writing.
* **PPB**: The Stag Prom Programmer binary format is understood for both reading and writing.
* **PPX**: The Stag Prom Programmer hexadecimal format is understood for both reading and writing.
* **Signetics**: The Signetics format is understood for both reading and writing.
SPASM The SPASM format is used by a variety of PIC programmers; it is understood for both reading and writing.
* **Spectrum**: The Spectrum format is understood for both reading and writing.
* **Tektronix (Extended)**: The Tektronix hexadecimal format and the Tektronix Extended hexadecimal format are both understood for both reading and writing.
* **Texas Instruments Tagged**: The Texas Instruments Tagged format is understood for both reading and writing (both 8 and 16 bit). Also known as the TI-tagged or TI-SDSMAC format.
* **Texas Instruments ti-txt**: The TI-TXT format is understood for reading and writing. This format is used with the bootstrap loader of the Texas Instruments MSP430 family of processors.
* **TRS-80**: The Radio Shack TRS-80 object file format is understood for reading and writing.
* **VHDL**: It is possible to write VHDL file. This is only supported for output.
* **Verilog VMEM**: It is possible to write a Verilog VMEM file suitable for loading with $readmemh(). This format is supported for reading and writing.
* **Wilson**: The Wilson format is understood for both reading and writing. This mystery format was added for a mysterious type of EPROM writer.

Full documentation and binary packages can be found at the [SRecord website](https://srecord.sourceforge.net/)