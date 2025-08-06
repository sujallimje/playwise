# PlayWise Music Engine - Interactive Features

## Overview

The PlayWise Music Engine now supports fully interactive operation where users can control all functionality through a menu-driven interface.

## How to Run

```bash
cd d:/_cpp/technicia/playwise
./build/playwise_engine
```

## Available Modes

1. **Interactive Mode** (Recommended) - Full menu-driven interface
2. **Demo Mode** - Original automated demonstration

## Interactive Features

### 1. Add New Song

- Enter custom song details including ID, title, artist, duration, and rating
- Songs are automatically added to the playlist and lookup system

### 2. Display Playlist

- View all songs currently in the playlist with their index numbers
- Shows song details including title, artist, and duration

### 3. Play Song

- Select any song from the playlist by entering its ID
- Adds the song to play history for tracking

### 4. Undo Last Play

- Remove the most recently played song from history
- Re-adds it back to the playlist

### 5. Sort Playlist

Choose from multiple sorting options:

- **Title (A-Z)** - Alphabetical by song title
- **Title (Z-A)** - Reverse alphabetical by song title
- **Duration (Shortest first)** - Sort by duration ascending
- **Duration (Longest first)** - Sort by duration descending
- **Recently Added** - Sort by when songs were added

Algorithm choices:

- **Merge Sort** - Guaranteed O(n log n) performance
- **Quick Sort** - Average O(n log n), faster in practice

### 6. Search Songs

Three search methods available:

- **By Song ID** - Find a specific song using its unique ID
- **By Title** - Find all songs with matching title
- **By Rating** - Find all songs with a specific star rating (1-5)

### 7. Rate Song

- Select any song by ID and assign a rating from 1-5 stars
- Updates the song's rating and adds it to the rating tree for fast searches

### 8. Playlist Manipulation

Advanced playlist operations:

- **Move Song** - Change the position of a song in the playlist
- **Delete Song** - Remove a song from the playlist by index
- **Reverse Playlist** - Reverse the entire playlist order

### 9. View Play History

- See all recently played songs (up to 10 most recent)
- Shows total number of songs played
- Tracks playback history using stack data structure

### 10. System Dashboard

Comprehensive system overview:

- **Total Songs** - Number of songs in the database
- **Playlist Size** - Current number of songs in playlist
- **Top 5 Longest Songs** - Songs sorted by duration
- **Recently Played** - Last few songs from history
- **Rating Statistics** - Count of songs by star rating (1-5)

## Data Structures Used

### Doubly Linked List (Playlist)

- **Operations**: Add, delete, move, reverse songs
- **Time Complexity**: O(1) for add, O(n) for search/delete/move
- **Benefits**: Efficient insertion/deletion, bidirectional traversal

### Stack (Play History)

- **Operations**: Push played songs, pop for undo
- **Time Complexity**: O(1) for all operations
- **Benefits**: Natural LIFO behavior for undo functionality

### Binary Search Tree (Rating System)

- **Operations**: Insert rated songs, search by rating
- **Time Complexity**: O(log n) average for insert/search
- **Benefits**: Fast rating-based queries, sorted rating access

### Hash Map (Instant Lookup)

- **Operations**: Store songs by ID and title for fast access
- **Time Complexity**: O(1) average for lookup
- **Benefits**: Instant song retrieval by ID or title

### Sorting Algorithms

- **Merge Sort**: Stable, guaranteed O(n log n) performance
- **Quick Sort**: In-place, average O(n log n), faster practical performance

## Sample Usage Flow

1. Start application and choose Interactive Mode (1)
2. View initial playlist with sample songs (Option 2)
3. Add your own songs (Option 1)
4. Play some songs (Option 3)
5. Sort playlist by your preference (Option 5)
6. Search for specific songs (Option 6)
7. Rate your favorite songs (Option 7)
8. View system statistics (Option 10)

## Technical Features

- **Memory Management**: Proper cleanup of dynamically allocated songs
- **Error Handling**: Input validation and graceful error messages
- **Performance Timing**: Sort operations show execution time
- **Type Safety**: Fixed signed/unsigned comparison warnings
- **User-Friendly**: Clear menus and helpful prompts

The interactive system makes all the powerful data structure operations accessible through an intuitive interface!
