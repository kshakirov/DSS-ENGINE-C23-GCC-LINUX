# DSS Storage: File Table, Block Mapping, and Associative Layer

## Status

Current project: **DSS-ENGINE-C23-GCC-LINUX**  
Current working direction: connect the logical file model to the physical block storage model before moving to chunking / CDC.

The current block-storage approximation already has the conceptual chain:

```text
BlockHash
    ↓
HashIndex
    ↓
BlockAddress
```

The next step is to introduce a file-level table above the block table, while keeping the future associative filesystem as a separate higher layer.

## 1. Main architectural principle

Do **not** make a file point directly to physical block addresses.

The intended layered model is:

```text
Associative layer
query / predicates / properties
            ↓
          FileId
            ↓
File layer
FileId / FileIndex
            ↓
ordered BlockIndex[]
            ↓
Block layer
BlockHash → BlockIndex → BlockAddress
            ↓
Physical storage
BlockAddress → bytes
```

A file is a logical object composed of an ordered sequence of block references. A physical address belongs to the block-storage layer, not to the semantic identity of the file.

## 2. First approximation of the file table

Until DSS gets its own hash-table implementation, keep the same experimental scheme already used for blocks:

- one array used for search;
- one parallel array used for metadata;
- the common array index connects them.

Proposed first approximation:

```c
static BlockHash fileNameHashTable[MAX_FILES];

typedef struct {
    char filename[64];
    size_t total_size;
    size_t chunk_count;
    HashIndex block_indices[MAX_CHUNKS_PER_FILE];
} FileEntry;

static FileEntry fileMetadataTable[MAX_FILES];
```

Semantics:

```text
fileNameHashTable[i]
        ↕
fileMetadataTable[i]
```

The shared index `i` is the relation between the search structure and the metadata structure.

Search path:

```text
filename
    ↓ hash
fileNameHashTable
    ↓
FileIndex
    ↓
fileMetadataTable[FileIndex]
    ↓
ordered block_indices[]
```

Each stored block index then resolves through the existing block layer:

```text
BlockIndex
    ↓
blockAddressTable[BlockIndex]
```

## 3. Separate index types

Even if both indices are physically represented by the same integer type, keep their meanings distinct.

For example:

```c
typedef int32_t HashIndex;
typedef int32_t FileIndex;
```

At the current stage:

- `HashIndex` identifies an entry in the block-search/block-address structure;
- `FileIndex` identifies an entry in the file-search/file-metadata structure.

This distinction is semantically useful even if both lower to `int32_t`.

Do not collapse the two concepts merely because their machine representation is currently identical.

## 4. Why two parallel file arrays

The current model intentionally keeps:

```text
search structure ≠ metadata structure
```

Today:

```text
fileNameHashTable[]   — search approximation
fileMetadataTable[]   — logical file data
```

Later the search mechanism can be replaced by a real DSS hash table without changing the basic meaning of `FileEntry`.

Thus the experiment isolates two concerns:

1. how an object is found;
2. what data the object contains once found.

## 5. Filename is temporary

For the current approximation, `filename` may remain inside `FileEntry`.

However, this is **not** the final DSS filesystem ontology.

The long-term idea is:

```text
object identity ≠ pathname
```

A future file/object should be discoverable through associations and predicates such as:

```text
name = foo.txt
type = text
project = DSS
author = ...
tag = ...
```

Therefore a pathname/name should eventually become only one association among many.

The final direction is:

```text
query / predicate set
        ↓
      FileId
```

rather than:

```text
path
  ↓
file
```

The current filename hash is only a first practical lookup mechanism.

## 6. File as a logical object

For the first approximation, treat a file as:

> an ordered sequence of references to blocks plus file-level metadata.

Minimal model:

```text
FileEntry
    total_size
    chunk_count
    block_indices[]
```

The order of `block_indices[]` reconstructs the logical order of file content.

This is enough for the next stage.

Do not introduce logical offsets, extents, graphs, trees, or other structures unless an experiment demonstrates that they are needed.

## 7. Deduplication consequence

Once files store block indices rather than physical addresses, deduplication becomes natural.

Example:

```text
File A -> [3, 8, 12]
File B -> [4, 8, 15]
```

Both files refer to block `8`.

The block exists physically once:

```text
BlockHash(8)
    ↓
BlockIndex 8
    ↓
BlockAddress
```

Thus logical file structure and physical block identity remain separate.

## 8. Immediate implementation task

Do not start CDC/Rabin yet.

First complete the minimal file/block relation.

### Step A — File index

Introduce a file index type and file counters as needed.

Conceptually:

```c
typedef int32_t FileIndex;
```

### Step B — Search helpers

Create file-table equivalents of the block-table experiment:

```text
find_file_index(...)
insert_file_index(...)
```

The implementation may initially remain linear. The goal is semantics, not hash-table optimization.

### Step C — Metadata synchronization

When a new file-search entry is created, the metadata entry at the same index must refer to the same logical file.

Invariant:

```text
fileNameHashTable[i]
and
fileMetadataTable[i]
```

must always describe the same file object.

### Step D — Block association

A `FileEntry` must be able to store the ordered `HashIndex` values of its blocks.

At this stage, a simple fixed-capacity array is acceptable.

## 9. Minimal invariants to test

### File lookup invariant

For the same filename/hash:

```text
first lookup  -> FileIndex i
second lookup -> FileIndex i
```

No duplicate logical file entry should be created.

### Parallel-table invariant

For every valid `FileIndex i`:

```text
fileNameHashTable[i]
```

and

```text
fileMetadataTable[i]
```

must refer to the same logical file.

### Block-reference invariant

If:

```text
fileMetadataTable[i].block_indices[j] = k
```

then `k` must be a valid block index in the block-storage layer.

### Dedup invariant

If two file chunks have the same block content and therefore the same block identity, both file entries must be allowed to contain the same block index.

## 10. What comes after this

Only after the file table and block-address relation are stable, move to content-defined chunking.

Planned chain:

```text
bytes
  ↓
GF(2) / polynomial model
  ↓
Rabin fingerprint
  ↓
rolling hash
  ↓
CDC
  ↓
chunk boundaries
  ↓
content hash
  ↓
BlockIndex
  ↓
BlockAddress
```

Then the file layer becomes:

```text
FileEntry
    ↓
ordered chunk/block indices
```

The chunker should feed an already-working block-storage abstraction rather than forcing storage and chunking to be invented simultaneously.

## 11. Research significance: module boundaries

The next review of module organization is not merely code cleanup.

For DSS, module boundaries have scientific relevance because the project is also part of the broader **Philosophy of Computation** investigation.

Questions to examine explicitly:

- What computational objects exist at each level?
- Which state belongs to which object?
- What is observable through an interface?
- What is merely a physical representation?
- When are two machine representations implementations of the same logical object?
- Which relations are semantic and which are accidental consequences of the current C layout?
- When should representation be hidden?
- When is exposing representation useful for a research experiment?

The intended distinction is currently:

```text
File object
    ≠
Block object
    ≠
Physical address
    ≠
Search structure
```

Even if all four are temporarily implemented with arrays and integers.

That distinction must remain visible in the code and experiments.

## 12. C23 / GCC / Linux research rule

The project targets:

```text
C23 + GCC + Linux
```

When ISO C23 and practical GCC/Linux facilities differ, do not hide the difference.

Where useful:

1. show the strictly ISO C23 form;
2. show the GCC/Linux form;
3. document explicitly that both forms are retained for research and educational comparison.

Example already relevant to the project:

```text
ISO C include guards
vs.
#pragma once
```

The same principle applies to other GCC/Linux-specific mechanisms used deliberately by DSS.

## 13. Current stopping point

The immediate target is:

```text
filename/hash
    ↓
FileIndex
    ↓
FileEntry
    ↓
ordered BlockIndex[]
    ↓
BlockHash / BlockIndex
    ↓
BlockAddress
```

Once this relation is working and tested, begin the chunking/Rabin experiment.

Do not implement the final associative filesystem layer yet.

Keep it as the architectural target above the file table:

```text
associations / predicates / queries
                ↓
              FileId
```

The filename-based table is only the first approximation on the path toward that model.
