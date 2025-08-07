# zipx

A fast, dictionary-based text compression tool written in C. This project allows you to compress, decompress, and train custom dictionaries for efficient storage and transfer of text files.

## Features

- **Compression**: Compress text files using a dictionary-based algorithm.
- **Decompression**: Restore compressed files to their original form.
- **Dictionary Training**: Train your own dictionary on a set of files for better compression ratios.
- **Custom Dictionaries**: Use your own dictionary or the default one provided.

## Getting Started

### Prerequisites

- C++ compiler (e.g., `g++`)
- Make (optional, for using the provided `makefile`)

### Building

From the `src` directory, run:

```bash
make
```

This will build the `zipx` executable.

## Usage

All commands should be run from the `src` directory. The main executable is `zipx`.

### Basic Syntax

```bash
./zipx -D [DICT_FILE] [OPTION] [FILE]...
```

- If `-D` is omitted, the default dictionary (`default.dict`) is used.
- `[OPTION]` can be one of:
  - `-h` : Show help message
  - `-d` : Decompress files
  - `-t` : Train the dictionary on files

### Examples

- **Compress files** (using default dictionary):

  ```bash
  ./zipx file1.txt file2.txt
  ```

- **Compress files with a custom dictionary:**

  ```bash
  ./zipx -D my_dict file1.txt
  ```

- **Decompress files:**

  ```bash
  ./zipx -d file1.com file2.com
  ```

- **Decompress with a custom dictionary:**

  ```bash
  ./zipx -D my_dict -d file1.com
  ```

- **Train a dictionary on a set of files:**

  ```bash
  ./zipx -t file1.txt file2.txt
  ```

- **Train using the provided script:**

  ```bash
  ./train.sh
  ```

## Notes

- Only execute files from the `src` folder.
- Use the `train.sh` script to train the dictionary on all files in a directory.
- Compressed files typically have the `.com` extension.

## Error Handling

The program will report errors for:

- Missing input files
- Missing or invalid dictionary files
- Invalid syntax
- File open errors
- Directory errors when using `train.sh`

## License

This project is provided as-is for educational and research purposes.

---

For more details, see the source code and comments in the `src/` directory.
