# VirtualDisk

An in-memory filesystem, simulating a Unix-like directory structure with a terminal interface. Built as a low-level systems project exploring tree-based data structures, smart pointer memory management, and type-safe error handling.

---

## Why am I building this?

This project exists because ForgeDB needs more than just a storage engine.

Block_storage_engine handles the low-level stuff: blocks, inodes, bitmaps, the raw disk
layout. But before data ever hits disk, something needs to manage the filesystem layer
above it. The in-memory tree, the working directory, path traversal, the actual
`mkdir /assets/textures` kind of logic. That's what VirtualDisk is.

Think of it this way: when you type a path into your terminal, your OS isn't going
straight to disk for every single operation. There's a layer in memory representing where
you are, what exists, and how things are connected. VirtualDisk builds that layer from
scratch.

It's a fully navigable filesystem that lives entirely in RAM. No disk reads, no I/O.
Just a tree of files and folders you can create, remove, navigate, and traverse using
real Unix-style commands. It even has its own terminal interface so you can actually
use it, not just test it programmatically.

The reason I'm building it as a separate project instead of just embedding it into BSE
is that it forced me to get the design right in isolation. No block layout to worry
about, no serialization concerns. Just: what does a filesystem actually look like as a
data structure? How do you handle paths cleanly? How do you make every failure
visible without drowning in exceptions?

Once VirtualDisk and block_storage_engine are integrated, that combined layer becomes
the foundation ForgeDB sits on. VirtualDisk handles the in-memory tree and path
semantics. BSE handles persistence. ForgeDB exposes the whole thing as one clean
embeddable library.

This is the middle piece.


## Overview

VirtualDisk creates a fully navigable filesystem in memory — no disk I/O involved. It supports creating, listing, removing, and navigating files and folders through a command-line terminal interface, mimicking the behavior of a real Unix shell.

The project is built around three core ideas:

- **Entity hierarchy** — `FileSystemEntity` as an abstract base class with `virtualFile` and `virtualFolder` as concrete subclasses, connected via a tree using `shared_ptr` and `weak_ptr` to avoid reference cycles.
- **Type-safe error handling** — a custom `Result<T>` template replaces traditional exception handling and sentinel return values, making all failure paths explicit and compiler-enforced.
- **Terminal interface** — a command parser that tokenizes user input and dispatches to the appropriate filesystem operation, closely modeled after real Unix shell behavior.

---

## Features

- Create and remove files and folders (`mkdir`, `touch`, `rm`)
- Navigate the directory tree (`cd`) with support for relative paths, `..`, `.`, and `~`
- List directory contents (`ls`) for single or multiple directories
- Path traversal with full support for nested paths (e.g. `mkdir a/b/c`)
- Input validation with descriptive error messages and suggestions
- File type registration and validation (txt, json, png, mp3, and more)
- Timestamps on all entities (`createdAt`, `modifiedAt`)
- Custom `Result<T>` error handling — zero exceptions in the core filesystem logic

---

## Project Structure

```
virtualDisk/
├── include/
│   ├── error/
│   │   ├── error.h          # FSError struct
│   │   └── result.h         # Result<T> template and Result<void> specialization
│   ├── files.h              # FileSystemEntity, virtualFile, virtualFolder, FileSystem
│   ├── validation.h         # Validator — input and syntax checking
│   ├── display.h            # Terminal display functions
│   └── utils.h              # Utility functions (parseInputs, joinStrings)
├── src/
│   ├── main.cpp             # Entry point, command loop, command dispatch
│   ├── files.cpp            # FileSystem and entity method implementations
│   ├── validation.cpp       # Validator method implementations
│   ├── display.cpp          # Display function implementations
│   └── utils.cpp            # Utility function implementations
├── bin/                     # Compiled binary output (generated)
└── Makefile
```

---

## Requirements

- **C++17** or later
- **g++** (GCC)
- **make**
- Linux or macOS (uses POSIX time functions)

---

## Build & Run

**Build:**
```bash
make
```

**Run:**
```bash
./bin/virtualDisk
```

**Clean build artifacts:**
```bash
make clean
```

---

## Usage

On launch, VirtualDisk presents a terminal prompt showing your current path:

```
================================================================================
====================== VIRTUAL DISK ===========================================
Hint: Type 'help' to see a list of disk operations (mkdir, ls, cd).
--------------------------------------------------------------------------------

~/root$
```

### Commands

| Command | Alias | Description |
|---|---|---|
| `mkdir <name>` | `create-directory` | Create one or more folders |
| `ls` | `list` | List contents of current or specified directory |
| `cd <path>` | `change-directory` | Change current directory |
| `touch <name.type>` | `create-file` | Create a file with a valid file type |
| `rm <name>` | `remove` | Remove a file |
| `cat` | `concatenate` | Read file contents |
| `help` | — | Show available commands |
| `exit` | — | Exit the program |

### Path Syntax

| Token | Meaning |
|---|---|
| `.` | Current directory |
| `..` | Parent directory |
| `~` | Root directory |
| `a/b/c` | Nested path traversal |

### Examples

```bash
# Create a nested directory structure
mkdir documents
mkdir documents/work/projects

# Create files
touch notes.txt
touch documents/report.pdf

# Navigate
cd documents/work
cd ..
cd ~

# List contents
ls
ls documents
ls documents work

# Remove a file
rm notes.txt
```

### Supported File Types

```
txt  json  xml  dat  cfg  save
png  jpg  wav  ogg  mp3  mp4
lvl  map  log  tmp  pdf  zip
```

Additional types can be registered at runtime via `virtualFile::registerFileType()`.

---

## Architecture

### Entity Tree

The filesystem is represented as a tree rooted at a single `virtualFolder` named `root`. Each node is heap-allocated and managed through `shared_ptr`. Parent pointers use `weak_ptr` to break reference cycles — without this, nodes would never be freed since parent and child would hold each other alive indefinitely.

```
root (virtualFolder)
├── documents (virtualFolder)
│   ├── report.pdf (virtualFile)
│   └── work (virtualFolder)
└── notes.txt (virtualFile)
```

All nodes inherit from `FileSystemEntity`, which provides the common interface: name, timestamps, parent pointer, and the `isFolder()` discriminator. `virtualFolder` adds child management. `virtualFile` adds content and type.

### Result\<T\> Error Handling

The project uses a custom `Result<T>` type rather than exceptions. Every operation that can fail returns either a value wrapped in `Ok` or a structured `FSError` wrapped in `Err`:

```cpp
Result<shared_ptr<virtualFolder>> virtualFolder::createFolder(string name);
Result<void> virtualFolder::removeFile(shared_ptr<virtualFile> file);
Result<shared_ptr<virtualFolder>> FileSystem::traverseTree_S(string& path);
```

Callers are forced to check results before using values:

```cpp
auto result = destination->createFolder(folderName);
if (result.isErr()) {
    displayError(result.unwrapErr().message, result.unwrapErr().suggestion);
} else {
    auto folder = result.unwrap();
}
```

This pattern makes all failure paths visible at the call site with zero silent failures. The `Result<void>` specialization handles operations that succeed or fail but return no value.

### Path Traversal

`FileSystem::traverseTree()` walks a tokenized path string segment by segment, starting from the current working directory. It handles `.`, `..`, `~`, and named folder steps — returning `Err` at the first segment that fails to resolve. `traverseTree_S` is a convenience wrapper that locks the resulting `weak_ptr` into a `shared_ptr`.

### Validator

`Validator` is a stateless class with all static methods. It handles input syntax checking before operations reach the filesystem. Each method returns `Result<void>` — on success carries nothing, on failure carries the error message and a suggestion for the user. Validation is always performed at the command dispatch layer, not inside filesystem methods.

---

## Planned

- Persist filesystem state to disk via integration with [BlockStorageEngine](https://github.com/etcoder-642/block_storage_engine) — a separate Unix-like block storage project implementing superblocks, inodes, and bitmaps
- implement additional commands like `cat`, `mv`, `cp`, `rmdir`
- File content editing

---

## What I Learned Building This

- Smart pointer ownership patterns — when to use `shared_ptr` vs `weak_ptr` and why mixing them incorrectly causes either memory leaks or dangling pointers
- Template specialization — implementing `Result<void>` as a full specialization because `void` cannot be stored as a value type
- Namespace design — keeping all project code under `namespace vfs` to prevent collisions with `std` and future integrated libraries

---

## Author

[Manasseh Samuel](https://github.com/etcoder-642)
Aspiring Systems Engineer.