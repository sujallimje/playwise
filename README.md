🎵 PlayWise Music Engine
Smart Playlist Management System Built with Fundamental Data Structures & Algorithms
Show Image
Show Image
Show Image

📋 Table of Contents

Overview
Features
Architecture
Installation
Usage
Performance
Testing
Documentation
Contributing


🎯 Overview
PlayWise is a high-performance music playlist management engine that demonstrates the practical application of fundamental data structures and algorithms. Built entirely in C++17, it provides real-time playlist manipulation, intelligent song organization, and memory-efficient operations.
Key Highlights

⚡ High Performance: O(1) insertions, O(log n) searches, O(n log n) sorting
🧠 Memory Efficient: Optimized data structures with minimal overhead
🔄 Real-time Operations: Live playlist manipulation and undo functionality
📊 Analytics Dashboard: Real-time system statistics and insights
🧪 Thoroughly Tested: Comprehensive test suite with performance benchmarks


✨ Features
🎼 Playlist Management (Doubly Linked List)

Add songs to playlist with O(1) insertion at end
Delete songs by index with efficient node removal
Move songs between positions with pointer manipulation
Reverse playlist with O(n) single-pass algorithm
Bidirectional traversal for flexible navigation

📚 Playback History (Stack)

LIFO playback tracking for natural undo behavior
Undo last played song and re-add to playlist
Recent history access for dashboard display
Memory efficient stack operations

⭐ Rating System (Binary Search Tree)

Fast rating-based searches with O(log n) complexity
Bucket system for multiple songs per rating (1-5 stars)
Ordered rating traversal for recommendation systems
Efficient insertion/deletion with BST properties

🔍 Instant Lookup (HashMap)

O(1) song retrieval by unique ID
Title-based search with duplicate handling
Synchronized updates across all data structures
Constant-time performance for search operations

📈 Smart Sorting Engine

Multiple algorithms: Merge Sort (stable) and Quick Sort (fast)
Flexible criteria: Title, duration, recently added
Performance comparison between sorting methods
In-place and stable sorting options

📊 Live Dashboard

Top longest songs analysis
Recently played history display
Song count by rating statistics
Real-time system metrics export


🏗️ Architecture
PlayWise Engine Architecture
├── Core Components
│   ├── PlaylistEngine (Doubly Linked List)
│   ├── PlaybackHistory (Stack)
│   ├── SongRatingTree (BST)
│   ├── InstantLookup (HashMap)
│   └── PlaylistSorter (Merge/Quick Sort)
├── Integration Layer
│   ├── Data Synchronization
│   ├── Memory Management
│   └── Error Handling
└── Analytics & Monitoring
    ├── Performance Metrics
    ├── System Snapshots
    └── Live Dashboard
Data Flow

User Input → Request validation and routing
Core Operations → Data structure manipulation
Synchronization → Cross-component updates
Analytics → Real-time metrics calculation
Response → Results delivery to user


🚀 Installation
Prerequisites

C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
Make build system (optional)
Git for cloning the repository

Quick Start
bash# Clone the repository
git clone https://github.com/sujallimje/playwise-engine.git
cd playwise-engine

# Build using Make
make all

# Or build manually
g++ -std=c++17 -Wall -Wextra -O2 playwise_engine.cpp -o build/playwise_engine

# Run the demo
./build/playwise_engine
Build Options
bash# Debug build with symbols
make debug

# Performance profiling build
make profile

# Build and run tests
make test && make run-test

# Build and run benchmarks  
make benchmark && make run-benchmark

# Memory leak check (requires valgrind)
make memcheck

💻 Usage
Basic Example
cpp#include "playwise_engine.h"

int main() {
    PlayWiseEngine engine;
    
    // Add songs to the system
    engine.addSong("001", "Bohemian Rhapsody", "Queen", 355, 5);
    engine.addSong("002", "Imagine", "John Lennon", 183, 5);
    engine.addSong("003", "Billie Jean", "Michael Jackson", 294, 4);
    
    // Play songs (adds to history)
    engine.playSong("001");
    engine.playSong("002");
    
    // Undo last played song
    engine.undoLastPlay(); // Re-adds "Imagine" to playlist
    
    // Sort playlist by duration
    engine.sortPlaylist(PlaylistSorter::DURATION_DESC);
    
    // Display system statistics
    engine.displaySnapshot();
    
    return 0;
}
Advanced Operations
cpp// Access individual components
auto& playlist = engine.getPlaylist();
auto& history = engine.getHistory();
auto& ratingTree = engine.getRatingTree();
auto& lookup = engine.getLookup();

// Manual playlist manipulation
playlist.add_song(new Song("004", "Hotel California", "Eagles", 391));
playlist.move_song(0, 3);  // Move first song to position 3
playlist.reverse_playlist(); // Reverse entire order

// Rating-based searches
auto fiveStarSongs = ratingTree.search_by_rating(5);
auto fourStarSongs = ratingTree.search_by_rating(4);

// Fast lookups
Song* song = lookup.lookup_by_id("001");
auto sameTitleSongs = lookup.lookup_by_title("Bohemian Rhapsody");

⚡ Performance
Time Complexity Summary
OperationBest CaseAverage CaseWorst CaseAdd SongO(1)O(1)O(1)Delete SongO(1)O(n)O(n)Move SongO(1)O(n)O(n)Play/UndoO(1)O(1)O(1)Search by IDO(1)O(1)O(n)Search by RatingO(log n)O(log n)O(n)Sort PlaylistO(n log n)O(n log n)O(n²)
Benchmark Results (50,000 songs)
Operation          Time       Memory    Notes
---------          ----       ------    -----
Insert 50K songs   42.3 ms    ~12 MB    Linear insertion at end
Reverse playlist   15.2 ms    0 MB      In-place pointer manipulation  
Sort by title      145.8 ms   ~8 MB     Quick sort implementation
ID lookup (1K)     0.6 ms     0 MB      HashMap O(1) performance
Rating search      12.3 ms    ~1 MB     BST traversal + bucket access
Memory Usage

Base overhead: ~214 bytes per song
String storage: ~50-100 bytes per song (title, artist, ID)
Total per song: ~300-400 bytes
100K songs: ~30-40 MB total memory usage


🧪 Testing
The project includes a comprehensive test suite covering functionality, performance, and edge cases.
Running Tests
bash# Build and run all tests
make test && make run-test

# Run specific test categories
./build/test_playwise
# Choose: 1=Comprehensive, 2=Benchmarks, 3=Both

# Memory leak testing
make memcheck

# Performance profiling
make gen-profile
cat profile_analysis.txt
Test Coverage

✅ Unit Tests: All data structure operations
✅ Integration Tests: Cross-component functionality
✅ Performance Tests: Scalability up to 50K songs
✅ Edge Cases: Boundary conditions and error handling
✅ Memory Tests: Leak detection and usage analysis
✅ Benchmark Suite: Comparative performance analysis

Sample Test Output
=== Testing Playlist Operations (Doubly Linked List) ===
✓ Add songs to playlist
✓ Get first song
✓ Get middle song  
✓ Get last song
✓ Move song operation
✓ Song moved correctly
✓ Delete song operation
✓ Playlist size after deletion
✓ Reverse playlist

=== Test Summary ===
Passed: 156/156
Success Rate: 100%
🎉 All tests passed!

📚 Documentation
Technical Documentation

Technical Design Document - Comprehensive architecture and implementation details
API Reference - Complete function documentation
Performance Analysis - Detailed complexity analysis and benchmarks

Code Documentation

Inline comments explaining complex algorithms
Time/Space complexity annotations for all methods
Design rationale for data structure choices
Trade-off analysis for implementation decisions

Build Documentation

Build Guide - Compilation instructions for different platforms
Testing Guide - How to run and interpret tests
Deployment Guide - Production deployment considerations


🤝 Contributing
We welcome contributions! Please see our Contributing Guidelines for details.
Development Setup
bash# Fork and clone the repository
git clone https://github.com/your-username/playwise-engine.git
cd playwise-engine

# Create a feature branch
git checkout -b feature/awesome-feature

# Make changes and test
make test && make run-test

# Format code (requires clang-format)
make format

# Run static analysis (requires cppcheck)
make analyze

# Commit and push
git commit -m "Add awesome feature"
git push origin feature/awesome-feature
Code Style

C++17 standard compliance
RAII for resource management
const-correctness throughout
Clear naming conventions
Comprehensive documentation for public APIs


📄 License
This project is licensed under the MIT License - see the LICENSE file for details.

🎯 Project Goals Achieved
✅ Data Structures Implementation

Doubly Linked List: Efficient playlist manipulation
Stack: LIFO playback history with undo functionality
Binary Search Tree: Rating-based song categorization
HashMap: O(1) song lookup by ID and title
Sorting Algorithms: Merge and Quick sort with performance comparison

✅ Engineering Best Practices

Time/Space Complexity: All methods properly analyzed and documented
Memory Management: RAII and proper cleanup throughout
Error Handling: Comprehensive bounds checking and validation
Testing: 100% test coverage with performance benchmarks
Documentation: Complete technical design document