# MiniGit: Git-like Version Control System

## Overview
MiniGit is a simplified version control system inspired by Git. It supports commits, branching, merging, and file difference tracking while demonstrating the core data structures and algorithms used in modern source control systems.

The project models commit history as a Directed Acyclic Graph (DAG), stores file snapshots using hashing, verifies content integrity through Merkle Trees, and computes file differences using the Longest Common Subsequence (LCS) algorithm.

## Features
- Create and store commits
- Branch creation and management
- Merge branches
- Commit history traversal
- File diff generation using LCS
- Content hashing for version tracking
- Merkle Tree based integrity verification
- DAG-based commit graph

## Data Structures & Algorithms
- **Merkle Tree** -> Snapshot integrity verification
- **DAG (Directed Acyclic Graph)** -> Commit history representation
- **Hashing** -> Commit and file identification
- **LCS Algorithm** -> Diff computation
- **Trees & Graphs** -> Branch and merge operations

## Compilation
g++ minigit.cpp -o minigit  
./minigit

## Example Commands
commit "Initial commit"  
branch feature-login  
checkout feature-login  
commit "Added login page"  
merge feature-login  
log  
diff file_v1.txt file_v2.txt

## Applications
- Source Code Management
- Document Version Tracking
- Backup Systems
- Distributed Version Control Systems

## Future Enhancements
- Distributed repositories
- Remote push/pull support
- Three-way merge
- Conflict resolution
- Persistent storage
- Compression and pack files

## Author
Varnika Shukla
