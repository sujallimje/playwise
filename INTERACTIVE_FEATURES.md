# PlayWise Music Engine - Interactive Features

## Overview

The PlayWise Music Engine now supports fully interactive operation where users can control all functionality through a menu-driven interface.

## How to Run

```bash
cd \\path\\
./build/playwise_engine
```

## Available Modes

1. **Interactive Mode** (Recommended) - Full menu-driven interface
2. **Demo Mode** - Original automated demonstration

## Interactive Features

### 1. Add New Song

- Enter custom song details including ID, title, artist, duration, rating, and genre
- Songs are automatically added to the playlist and lookup system
- Genre information enables mood-based auto-replay functionality

### 2. Display Playlist

- View all songs currently in the playlist with their index numbers
- Shows comprehensive song details including title, artist, genre, duration, and play count
- Enhanced display format with genre categorization and playback statistics

### 3. Play Song

- Select any song from the playlist by entering its ID
- Adds the song to play history for tracking
- Increments play count for auto-replay intelligence
- Triggers mood-based auto-replay for calming genres

### 4. Skip Current Song

- Skip the currently playing song instantly
- Adds skipped song to Recently Skipped Tracker (circular buffer)
- Maintains chronological order of last 10 skipped songs
- Integrates with auto-play system for seamless experience

### 5. Auto-Play Next

- Intelligent next song selection based on play history
- Prioritizes calming genres (Classical, Jazz, Ambient, Lo-Fi) for auto-replay
- Uses play count statistics to make smart replay decisions
- Seamless integration with mood-based replay system

### 6. Undo Last Play

- Remove the most recently played song from history
- Re-adds it back to the playlist

### 7. Sort Playlist

Choose from multiple sorting options:

- **Title (A-Z)** - Alphabetical by song title
- **Title (Z-A)** - Reverse alphabetical by song title
- **Duration (Shortest first)** - Sort by duration ascending
- **Duration (Longest first)** - Sort by duration descending
- **Recently Added** - Sort by when songs were added

Algorithm choices:

- **Merge Sort** - Guaranteed O(n log n) performance
- **Quick Sort** - Average O(n log n), faster in practice

### 8. Search Songs

Three search methods available:

- **By Song ID** - Find a specific song using its unique ID
- **By Title** - Find all songs with matching title
- **By Rating** - Find all songs with a specific star rating (1-5)

### 9. Rate Song

- Select any song by ID and assign a rating from 1-5 stars
- Updates the song's rating and adds it to the rating tree for fast searches

### 10. Playlist Manipulation

Advanced playlist operations:

- **Move Song** - Change the position of a song in the playlist
- **Delete Song** - Remove a song from the playlist by index
- **Reverse Playlist** - Reverse the entire playlist order

### 11. View Play History

- See all recently played songs (up to 10 most recent)
- Shows total number of songs played
- Tracks playback history using stack data structure

### 12. Recently Skipped Songs

Advanced skip tracking and management:

- **View Recently Skipped** - Display last 10 skipped songs in chronological order
- **Clear Skip History** - Reset the recently skipped tracker
- **Circular Buffer Management** - Automatic memory management (oldest entries removed when full)
- **Integration with Auto-Play** - Skip data influences next song selection

### 13. Auto-Replay Settings

Mood-based auto-replay management system:

- **View Statistics** - See replay cycles completed and songs in replay queue
- **Top Played Songs** - View most frequently played songs with play counts
- **Auto-Replay Status** - Check if mood-based auto-replay is enabled
- **Genre-Based Intelligence** - Automatic replay for calming genres (Classical, Jazz, Ambient, Lo-Fi)
- **Play Count Analytics** - Detailed statistics for replay decision making

### 14. Songs by Genre

Genre-based organization and display:

- **Categorized View** - All songs organized by their genres
- **Genre Statistics** - Count of songs in each genre category
- **Mood Identification** - Visual indication of calming vs energetic genres
- **Enhanced Navigation** - Easy browsing by musical style

### 15. System Dashboard

Comprehensive system overview:

- **Total Songs** - Number of songs in the database
- **Playlist Size** - Current number of songs in playlist
- **Top 5 Longest Songs** - Songs sorted by duration
- **Recently Played** - Last few songs from history
- **Rating Statistics** - Count of songs by star rating (1-5)
- **Genre Distribution** - Song count by genre categories
- **Auto-Replay Metrics** - Statistics from mood-based replay system

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

### Deque (Recently Skipped Tracker)

- **Operations**: Add skipped songs, maintain circular buffer, view/clear history
- **Time Complexity**: O(1) for add/remove operations
- **Benefits**: Efficient double-ended access, automatic memory management as circular buffer
- **Implementation**: Tracks last 10 skipped songs with FIFO behavior when full

### Auto-Replay HashMap (Play Count Tracking)

- **Operations**: Track play counts, identify frequently played songs, support replay decisions
- **Time Complexity**: O(1) average for count updates and lookups
- **Benefits**: Fast play count analytics, efficient replay queue management
- **Implementation**: Maps song IDs to play counts for intelligent auto-replay

### Sorting Algorithms

- **Merge Sort**: Stable, guaranteed O(n log n) performance
- **Quick Sort**: In-place, average O(n log n), faster practical performance

## Sample Usage Flow

1. Start application and choose Interactive Mode (1)
2. View initial playlist with sample songs including genres (Option 2)
3. Add your own songs with genre information (Option 1)
4. Play some songs to build play history (Option 3)
5. Test skip functionality and recently skipped tracker (Option 4)
6. Use auto-play next for intelligent song selection (Option 5)
7. Sort playlist by your preference (Option 7)
8. Search for specific songs (Option 8)
9. Rate your favorite songs (Option 9)
10. View recently skipped songs (Option 12)
11. Check auto-replay settings and statistics (Option 13)
12. Browse songs by genre (Option 14)
13. View comprehensive system dashboard (Option 15)

## Technical Features

- **Memory Management**: Proper cleanup of dynamically allocated songs
- **Error Handling**: Input validation and graceful error messages
- **Performance Timing**: Sort operations show execution time
- **Type Safety**: Fixed signed/unsigned comparison warnings
- **User-Friendly**: Clear menus and helpful prompts
- **Genre-Based Intelligence**: Mood detection for auto-replay functionality
- **Circular Buffer Management**: Efficient memory usage for skip tracking
- **Play Count Analytics**: Smart replay decisions based on listening patterns
- **Advanced Data Structure Integration**: Seamless combination of multiple data structures

## New Advanced Features

### Auto Replay Based on Genre Mood 🎭

The system intelligently identifies "calming" genres (Classical, Jazz, Ambient, Lo-Fi) and automatically replays them when playlists end, creating a continuous listening experience for relaxation and focus.

**Key Components:**

- **Genre Mood Detection**: Automatic classification of calming vs energetic genres
- **Play Count Intelligence**: Uses HashMap to track song popularity for smart replay selection
- **Queue Management**: Priority queue concepts for optimal replay ordering
- **Statistics Dashboard**: Comprehensive analytics showing replay cycles and top played songs

### Recently Skipped Tracker ⏭️

A sophisticated skip management system using a circular buffer to track user preferences and improve music recommendations.

**Key Components:**

- **Circular Buffer**: Deque-based implementation maintaining last 10 skipped songs
- **Memory Efficient**: Automatic cleanup when buffer reaches capacity
- **Chronological Tracking**: Maintains skip order for pattern analysis
- **Integration Ready**: Skip data influences auto-play and recommendation systems

The interactive system makes all the powerful data structure operations accessible through an intuitive interface!
