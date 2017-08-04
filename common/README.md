# CS50 Tiny Search Engine (TSE) common library

Christina Lu, August 2017.

These modules support the TSE project and contain common code used by 
the *crawler*, *indexer*, and *querier*.

## Usage
To build `libcommon.a`, run `make`.

To clean up, run `make clean`.

## Overview

 * index - all the logic required for saving and loading index files
 * pagedir - all the logic for saving and loading crawler directories
 * word - provided module, implements NormalizeWord()
