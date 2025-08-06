#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <queue>
#include <deque>
#include <set>

// Forward declarations
class Song;
class PlaylistEngine;
class PlaybackHistory;
class SongRatingTree;
class InstantLookup;
class PlayWiseEngine;

/**
 * Song class - represents a single song with metadata
 */
class Song
{
public:
    std::string id;
    std::string title;
    std::string artist;
    std::string genre; // Added genre field
    int duration;      // in seconds
    int rating;        // 1-5 stars
    int play_count;    // Track how many times song has been played
    std::chrono::system_clock::time_point added_time;

    Song(const std::string &id, const std::string &title,
         const std::string &artist, int duration, int rating = 0,
         const std::string &genre = "Unknown")
        : id(id), title(title), artist(artist), genre(genre), duration(duration),
          rating(rating), play_count(0), added_time(std::chrono::system_clock::now()) {}

    std::string toString() const
    {
        return title + " by " + artist + " [" + genre + "] (" + std::to_string(duration) + "s) [" + std::to_string(play_count) + " plays]";
    }

    // Check if song is from a calming genre
    bool isCalmingGenre() const
    {
        std::string lower_genre = genre;
        std::transform(lower_genre.begin(), lower_genre.end(), lower_genre.begin(), ::tolower);
        return lower_genre == "lo-fi" || lower_genre == "jazz" ||
               lower_genre == "classical" || lower_genre == "ambient" ||
               lower_genre == "chill" || lower_genre == "lofi";
    }
};

/**
 * DoublyLinkedList Node for Playlist Engine
 */
class PlaylistNode
{
public:
    Song *song;
    PlaylistNode *next;
    PlaylistNode *prev;

    PlaylistNode(Song *s) : song(s), next(nullptr), prev(nullptr) {}
};

/**
 * Playlist Engine using Doubly Linked List
 * Time Complexity: O(n) for most operations, O(1) for add_song at end
 * Space Complexity: O(n) where n is number of songs
 */
class PlaylistEngine
{
private:
    PlaylistNode *head;
    PlaylistNode *tail;
    int size;

public:
    PlaylistEngine() : head(nullptr), tail(nullptr), size(0) {}

    ~PlaylistEngine()
    {
        clear();
    }

    /**
     * Add song to end of playlist
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    void add_song(Song *song)
    {
        PlaylistNode *newNode = new PlaylistNode(song);

        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    /**
     * Delete song at specific index
     * Time Complexity: O(n) - need to traverse to index
     * Space Complexity: O(1)
     */
    bool delete_song(int index)
    {
        if (index < 0 || index >= size)
            return false;

        PlaylistNode *current = getNodeAt(index);
        if (!current)
            return false;

        if (current->prev)
        {
            current->prev->next = current->next;
        }
        else
        {
            head = current->next;
        }

        if (current->next)
        {
            current->next->prev = current->prev;
        }
        else
        {
            tail = current->prev;
        }

        delete current;
        size--;
        return true;
    }

    /**
     * Move song from one index to another
     * Time Complexity: O(n) - need to traverse and relink
     * Space Complexity: O(1)
     */
    bool move_song(int from_index, int to_index)
    {
        if (from_index < 0 || from_index >= size ||
            to_index < 0 || to_index >= size ||
            from_index == to_index)
            return false;

        PlaylistNode *node = getNodeAt(from_index);
        if (!node)
            return false;

        // Remove node from current position
        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;

        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;

        // Insert at new position
        if (to_index == 0)
        {
            node->prev = nullptr;
            node->next = head;
            if (head)
                head->prev = node;
            head = node;
            if (!tail)
                tail = node;
        }
        else if (to_index == size - 1)
        {
            node->next = nullptr;
            node->prev = tail;
            if (tail)
                tail->next = node;
            tail = node;
        }
        else
        {
            PlaylistNode *target = getNodeAt(to_index);
            node->next = target;
            node->prev = target->prev;
            if (target->prev)
                target->prev->next = node;
            target->prev = node;
        }

        return true;
    }

    /**
     * Reverse entire playlist
     * Time Complexity: O(n)
     * Space Complexity: O(1)
     */
    void reverse_playlist()
    {
        if (size <= 1)
            return;

        PlaylistNode *current = head;
        PlaylistNode *temp = nullptr;

        // Swap next and prev for all nodes
        while (current)
        {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev; // Move to next (which is now prev)
        }

        // Swap head and tail
        temp = head;
        head = tail;
        tail = temp;
    }

    /**
     * Get all songs as vector for sorting operations
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    std::vector<Song *> getAllSongs() const
    {
        std::vector<Song *> songs;
        PlaylistNode *current = head;
        while (current)
        {
            songs.push_back(current->song);
            current = current->next;
        }
        return songs;
    }

    /**
     * Rebuild playlist from sorted vector
     * Time Complexity: O(n)
     * Space Complexity: O(1) additional
     */
    void rebuildFromVector(const std::vector<Song *> &songs)
    {
        clear();
        for (Song *song : songs)
        {
            add_song(song);
        }
    }

    void display() const
    {
        PlaylistNode *current = head;
        int index = 0;
        while (current)
        {
            std::cout << index << ": " << current->song->toString() << std::endl;
            current = current->next;
            index++;
        }
    }

    int getSize() const { return size; }

private:
    PlaylistNode *getNodeAt(int index)
    {
        if (index < 0 || index >= size)
            return nullptr;

        PlaylistNode *current;
        if (index < size / 2)
        {
            current = head;
            for (int i = 0; i < index; i++)
            {
                current = current->next;
            }
        }
        else
        {
            current = tail;
            for (int i = size - 1; i > index; i--)
            {
                current = current->prev;
            }
        }
        return current;
    }

    void clear()
    {
        while (head)
        {
            PlaylistNode *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }
};

/**
 * Playback History using Stack
 * Time Complexity: O(1) for push/pop operations
 * Space Complexity: O(n) where n is number of played songs
 */
class PlaybackHistory
{
private:
    std::stack<Song *> history;

public:
    /**
     * Add song to playback history
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    void play_song(Song *song)
    {
        history.push(song);
    }

    /**
     * Undo last played song and return it
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    Song *undo_last_play()
    {
        if (history.empty())
            return nullptr;

        Song *lastPlayed = history.top();
        history.pop();
        return lastPlayed;
    }

    /**
     * Get recently played songs (up to n)
     * Time Complexity: O(min(n, stack_size))
     * Space Complexity: O(min(n, stack_size))
     */
    std::vector<Song *> getRecentlyPlayed(int n = 5)
    {
        std::vector<Song *> recent;
        std::stack<Song *> temp = history;

        while (!temp.empty() && recent.size() < static_cast<size_t>(n))
        {
            recent.push_back(temp.top());
            temp.pop();
        }

        return recent;
    }

    bool isEmpty() const { return history.empty(); }
    int size() const { return history.size(); }
};

/**
 * BST Node for Song Rating Tree
 */
class RatingNode
{
public:
    int rating;
    std::vector<Song *> songs; // Bucket for songs with this rating
    RatingNode *left;
    RatingNode *right;

    RatingNode(int r) : rating(r), left(nullptr), right(nullptr) {}
};

/**
 * Song Rating Tree using Binary Search Tree
 * Time Complexity: O(log n) average, O(n) worst case for search/insert/delete
 * Space Complexity: O(n) where n is number of unique ratings
 */
class SongRatingTree
{
private:
    RatingNode *root;

public:
    SongRatingTree() : root(nullptr) {}

    ~SongRatingTree()
    {
        clear(root);
    }

    /**
     * Insert song with rating
     * Time Complexity: O(log n) average, O(n) worst case
     * Space Complexity: O(1) for insertion, O(log n) for recursion stack
     */
    void insert_song(Song *song, int rating)
    {
        if (rating < 1 || rating > 5)
            return; // Invalid rating
        song->rating = rating;
        root = insertHelper(root, song, rating);
    }

    /**
     * Search songs by rating
     * Time Complexity: O(log n) to find node + O(m) where m is songs with that rating
     * Space Complexity: O(m) for return vector
     */
    std::vector<Song *> search_by_rating(int rating)
    {
        RatingNode *node = searchHelper(root, rating);
        if (node)
            return node->songs;
        return std::vector<Song *>();
    }

    /**
     * Delete song by ID
     * Time Complexity: O(n * log n) in worst case (need to search all rating buckets)
     * Space Complexity: O(log n) for recursion stack
     */
    bool delete_song(const std::string &song_id)
    {
        return deleteHelper(root, song_id);
    }

    /**
     * Get song count by rating for dashboard
     * Time Complexity: O(n) where n is number of rating nodes
     * Space Complexity: O(1)
     */
    std::unordered_map<int, int> getSongCountByRating()
    {
        std::unordered_map<int, int> counts;
        countHelper(root, counts);
        return counts;
    }

private:
    RatingNode *insertHelper(RatingNode *node, Song *song, int rating)
    {
        if (!node)
        {
            RatingNode *newNode = new RatingNode(rating);
            newNode->songs.push_back(song);
            return newNode;
        }

        if (rating == node->rating)
        {
            node->songs.push_back(song);
        }
        else if (rating < node->rating)
        {
            node->left = insertHelper(node->left, song, rating);
        }
        else
        {
            node->right = insertHelper(node->right, song, rating);
        }

        return node;
    }

    RatingNode *searchHelper(RatingNode *node, int rating)
    {
        if (!node || node->rating == rating)
            return node;

        if (rating < node->rating)
        {
            return searchHelper(node->left, rating);
        }
        return searchHelper(node->right, rating);
    }

    bool deleteHelper(RatingNode *node, const std::string &song_id)
    {
        if (!node)
            return false;

        // Search in current node's songs
        auto &songs = node->songs;
        for (auto it = songs.begin(); it != songs.end(); ++it)
        {
            if ((*it)->id == song_id)
            {
                songs.erase(it);
                return true;
            }
        }

        // Search in left and right subtrees
        return deleteHelper(node->left, song_id) || deleteHelper(node->right, song_id);
    }

    void countHelper(RatingNode *node, std::unordered_map<int, int> &counts)
    {
        if (!node)
            return;

        counts[node->rating] = node->songs.size();
        countHelper(node->left, counts);
        countHelper(node->right, counts);
    }

    void clear(RatingNode *node)
    {
        if (!node)
            return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
};

/**
 * Instant Song Lookup using HashMap
 * Time Complexity: O(1) average for lookup, O(n) worst case
 * Space Complexity: O(n) where n is number of songs
 */
class InstantLookup
{
private:
    std::unordered_map<std::string, Song *> id_map;
    std::unordered_map<std::string, std::vector<Song *>> title_map;

public:
    /**
     * Add song to lookup tables
     * Time Complexity: O(1) average
     * Space Complexity: O(1)
     */
    void add_song(Song *song)
    {
        id_map[song->id] = song;
        title_map[song->title].push_back(song);
    }

    /**
     * Remove song from lookup tables
     * Time Complexity: O(1) for ID lookup + O(k) for title lookup where k is songs with same title
     * Space Complexity: O(1)
     */
    void remove_song(const std::string &song_id)
    {
        auto it = id_map.find(song_id);
        if (it != id_map.end())
        {
            Song *song = it->second;
            id_map.erase(it);

            // Remove from title map
            auto &title_songs = title_map[song->title];
            title_songs.erase(
                std::remove_if(title_songs.begin(), title_songs.end(),
                               [song_id](Song *s)
                               { return s->id == song_id; }),
                title_songs.end());

            if (title_songs.empty())
            {
                title_map.erase(song->title);
            }
        }
    }

    /**
     * Lookup song by ID
     * Time Complexity: O(1) average
     * Space Complexity: O(1)
     */
    Song *lookup_by_id(const std::string &id)
    {
        auto it = id_map.find(id);
        return (it != id_map.end()) ? it->second : nullptr;
    }

    /**
     * Lookup songs by title
     * Time Complexity: O(1) average
     * Space Complexity: O(k) where k is number of songs with that title
     */
    std::vector<Song *> lookup_by_title(const std::string &title)
    {
        auto it = title_map.find(title);
        return (it != title_map.end()) ? it->second : std::vector<Song *>();
    }
};

/**
 * Sorting utilities with different algorithms
 */
class PlaylistSorter
{
public:
    enum SortCriteria
    {
        TITLE_ASC,
        TITLE_DESC,
        DURATION_ASC,
        DURATION_DESC,
        RECENTLY_ADDED
    };

    /**
     * Merge Sort implementation
     * Time Complexity: O(n log n)
     * Space Complexity: O(n)
     */
    static void mergeSort(std::vector<Song *> &songs, SortCriteria criteria)
    {
        mergeSortHelper(songs, 0, songs.size() - 1, criteria);
    }

    /**
     * Quick Sort implementation
     * Time Complexity: O(n log n) average, O(n²) worst case
     * Space Complexity: O(log n) for recursion stack
     */
    static void quickSort(std::vector<Song *> &songs, SortCriteria criteria)
    {
        quickSortHelper(songs, 0, songs.size() - 1, criteria);
    }

private:
    static bool compare(Song *a, Song *b, SortCriteria criteria)
    {
        switch (criteria)
        {
        case TITLE_ASC:
            return a->title < b->title;
        case TITLE_DESC:
            return a->title > b->title;
        case DURATION_ASC:
            return a->duration < b->duration;
        case DURATION_DESC:
            return a->duration > b->duration;
        case RECENTLY_ADDED:
            return a->added_time > b->added_time;
        default:
            return false;
        }
    }

    static void mergeSortHelper(std::vector<Song *> &songs, int left, int right, SortCriteria criteria)
    {
        if (left >= right)
            return;

        int mid = left + (right - left) / 2;
        mergeSortHelper(songs, left, mid, criteria);
        mergeSortHelper(songs, mid + 1, right, criteria);
        merge(songs, left, mid, right, criteria);
    }

    static void merge(std::vector<Song *> &songs, int left, int mid, int right, SortCriteria criteria)
    {
        std::vector<Song *> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right)
        {
            if (compare(songs[i], songs[j], criteria))
            {
                temp[k++] = songs[i++];
            }
            else
            {
                temp[k++] = songs[j++];
            }
        }

        while (i <= mid)
            temp[k++] = songs[i++];
        while (j <= right)
            temp[k++] = songs[j++];

        for (int x = 0; x < k; x++)
        {
            songs[left + x] = temp[x];
        }
    }

    static void quickSortHelper(std::vector<Song *> &songs, int low, int high, SortCriteria criteria)
    {
        if (low < high)
        {
            int pi = partition(songs, low, high, criteria);
            quickSortHelper(songs, low, pi - 1, criteria);
            quickSortHelper(songs, pi + 1, high, criteria);
        }
    }

    static int partition(std::vector<Song *> &songs, int low, int high, SortCriteria criteria)
    {
        Song *pivot = songs[high];
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (compare(songs[j], pivot, criteria))
            {
                i++;
                std::swap(songs[i], songs[j]);
            }
        }
        std::swap(songs[i + 1], songs[high]);
        return i + 1;
    }
};

/**
 * Recently Skipped Tracker using Deque
 * Maintains a circular buffer of recently skipped songs
 * Time Complexity: O(1) for add/check operations
 * Space Complexity: O(k) where k is max_size (default 10)
 */
class RecentlySkippedTracker
{
private:
    std::deque<std::string> skipped_songs; // Song IDs of recently skipped songs
    size_t max_size;

public:
    RecentlySkippedTracker(size_t max_size = 10) : max_size(max_size) {}

    /**
     * Add a song to the recently skipped list
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    void addSkippedSong(const std::string &song_id)
    {
        // Remove if already exists to avoid duplicates
        auto it = std::find(skipped_songs.begin(), skipped_songs.end(), song_id);
        if (it != skipped_songs.end())
        {
            skipped_songs.erase(it);
        }

        // Add to front
        skipped_songs.push_front(song_id);

        // Maintain size limit using sliding window
        if (skipped_songs.size() > max_size)
        {
            skipped_songs.pop_back();
        }
    }

    /**
     * Check if a song was recently skipped
     * Time Complexity: O(k) where k is max_size
     * Space Complexity: O(1)
     */
    bool wasRecentlySkipped(const std::string &song_id) const
    {
        return std::find(skipped_songs.begin(), skipped_songs.end(), song_id) != skipped_songs.end();
    }

    /**
     * Get all recently skipped songs
     * Time Complexity: O(k)
     * Space Complexity: O(k)
     */
    std::vector<std::string> getRecentlySkipped() const
    {
        return std::vector<std::string>(skipped_songs.begin(), skipped_songs.end());
    }

    /**
     * Clear the skipped songs list
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    void clear()
    {
        skipped_songs.clear();
    }

    size_t size() const { return skipped_songs.size(); }
    bool empty() const { return skipped_songs.empty(); }
};

/**
 * Auto Replay Manager for Genre-based Mood Replay
 * Uses HashMap for play counts and Priority Queue for top-k selection
 * Time Complexity: O(log n) for play count updates, O(n log n) for top-k selection
 * Space Complexity: O(n) where n is number of songs
 */
class AutoReplayManager
{
private:
    std::unordered_map<std::string, int> play_counts; // song_id -> play count
    std::queue<Song *> replay_queue;                  // Queue for auto-replay songs
    bool auto_replay_enabled;
    int replay_cycles; // Track how many times we've replayed

public:
    AutoReplayManager() : auto_replay_enabled(true), replay_cycles(0) {}

    /**
     * Record a song play and update play count
     * Time Complexity: O(1) average
     * Space Complexity: O(1)
     */
    void recordPlay(Song *song)
    {
        if (song)
        {
            play_counts[song->id]++;
            song->play_count++;
        }
    }

    /**
     * Get top N most played songs from a list, filtered by calming genres
     * Time Complexity: O(n log n) for sorting
     * Space Complexity: O(n) for temporary storage
     */
    std::vector<Song *> getTopCalmingSongs(const std::vector<Song *> &all_songs, int top_n = 3)
    {
        // Filter calming songs and create pairs for sorting
        std::vector<std::pair<int, Song *>> calming_songs;

        for (Song *song : all_songs)
        {
            if (song && song->isCalmingGenre() && play_counts[song->id] > 0)
            {
                calming_songs.push_back({play_counts[song->id], song});
            }
        }

        // Sort by play count (descending) using priority queue concept
        std::sort(calming_songs.begin(), calming_songs.end(),
                  [](const auto &a, const auto &b)
                  {
                      if (a.first == b.first)
                      {
                          // If play counts are equal, prefer more recent songs
                          return a.second->added_time > b.second->added_time;
                      }
                      return a.first > b.first;
                  });

        // Extract top N songs
        std::vector<Song *> result;
        for (int i = 0; i < std::min(top_n, (int)calming_songs.size()); i++)
        {
            result.push_back(calming_songs[i].second);
        }

        return result;
    }

    /**
     * Setup auto-replay queue with top calming songs
     * Time Complexity: O(n log n)
     * Space Complexity: O(k) where k is number of replay songs
     */
    void setupAutoReplay(const std::vector<Song *> &all_songs)
    {
        if (!auto_replay_enabled)
            return;

        // Clear existing queue
        while (!replay_queue.empty())
        {
            replay_queue.pop();
        }

        // Get top 3 calming songs
        auto top_calming = getTopCalmingSongs(all_songs, 3);

        if (!top_calming.empty())
        {
            std::cout << "\n🎵 Auto-replay activated! Top calming songs:" << std::endl;
            for (size_t i = 0; i < top_calming.size(); i++)
            {
                replay_queue.push(top_calming[i]);
                std::cout << (i + 1) << ". " << top_calming[i]->toString() << std::endl;
            }
            replay_cycles++;
            std::cout << "Replay cycle #" << replay_cycles << " starting...\n"
                      << std::endl;
        }
    }

    /**
     * Get next song from auto-replay queue
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    Song *getNextReplaySong()
    {
        if (replay_queue.empty())
        {
            return nullptr;
        }

        Song *song = replay_queue.front();
        replay_queue.pop();

        // Re-add to queue for continuous loop
        replay_queue.push(song);

        return song;
    }

    /**
     * Get play count statistics
     * Time Complexity: O(n)
     * Space Complexity: O(n)
     */
    std::unordered_map<std::string, int> getPlayCountStats() const
    {
        return play_counts;
    }

    bool hasReplaySongs() const { return !replay_queue.empty(); }
    void enableAutoReplay(bool enable) { auto_replay_enabled = enable; }
    bool isAutoReplayEnabled() const { return auto_replay_enabled; }
    int getReplayCycles() const { return replay_cycles; }

    /**
     * Display auto-replay statistics
     */
    void displayStats() const
    {
        std::cout << "\n=== Auto-Replay Statistics ===" << std::endl;
        std::cout << "Auto-replay enabled: " << (auto_replay_enabled ? "Yes" : "No") << std::endl;
        std::cout << "Replay cycles completed: " << replay_cycles << std::endl;
        std::cout << "Songs in replay queue: " << replay_queue.size() << std::endl;

        if (!play_counts.empty())
        {
            std::cout << "\nTop played songs:" << std::endl;
            std::vector<std::pair<int, std::string>> sorted_plays;
            for (const auto &pair : play_counts)
            {
                sorted_plays.push_back({pair.second, pair.first});
            }
            std::sort(sorted_plays.rbegin(), sorted_plays.rend());

            for (size_t i = 0; i < std::min(size_t(5), sorted_plays.size()); i++)
            {
                std::cout << (i + 1) << ". Song ID: " << sorted_plays[i].second
                          << " (" << sorted_plays[i].first << " plays)" << std::endl;
            }
        }
        std::cout << "==============================\n"
                  << std::endl;
    }
};

/**
 * Main PlayWise Engine - Integrates all components
 */
class PlayWiseEngine
{
private:
    PlaylistEngine playlist;
    PlaybackHistory history;
    SongRatingTree ratingTree;
    InstantLookup lookup;
    RecentlySkippedTracker skipped_tracker; // New: Recently skipped tracker
    AutoReplayManager replay_manager;       // New: Auto replay manager
    std::vector<Song *> songDatabase;       // Owns the song objects
    Song *current_song;                     // Track currently playing song
    bool playlist_ended;                    // Track if playlist has ended

public:
    PlayWiseEngine() : current_song(nullptr), playlist_ended(false) {}

    ~PlayWiseEngine()
    {
        // Clean up songs
        for (Song *song : songDatabase)
        {
            delete song;
        }
    }

    /**
     * Add new song to the system
     * Time Complexity: O(log n) due to BST insertion
     * Space Complexity: O(1)
     */
    Song *addSong(const std::string &id, const std::string &title,
                  const std::string &artist, int duration, int rating = 0,
                  const std::string &genre = "Unknown")
    {
        Song *song = new Song(id, title, artist, duration, rating, genre);
        songDatabase.push_back(song);

        playlist.add_song(song);
        lookup.add_song(song);
        if (rating > 0)
        {
            ratingTree.insert_song(song, rating);
        }

        return song;
    }

    void playSong(const std::string &song_id)
    {
        Song *song = lookup.lookup_by_id(song_id);
        if (song)
        {
            // Check if song was recently skipped
            if (skipped_tracker.wasRecentlySkipped(song_id))
            {
                std::cout << "⚠️  Warning: This song was recently skipped. Playing anyway..." << std::endl;
            }

            current_song = song;
            history.play_song(song);
            replay_manager.recordPlay(song); // Record play count
            playlist_ended = false;

            std::cout << "🎵 Now playing: " << song->toString() << std::endl;
        }
        else
        {
            std::cout << "❌ Song not found!" << std::endl;
        }
    }

    /**
     * Skip current song and add to recently skipped tracker
     * Time Complexity: O(1)
     * Space Complexity: O(1)
     */
    void skipCurrentSong()
    {
        if (current_song)
        {
            skipped_tracker.addSkippedSong(current_song->id);
            std::cout << "⏭️  Skipped: " << current_song->toString() << std::endl;
            std::cout << "Added to recently skipped list (" << skipped_tracker.size() << "/10)" << std::endl;
            current_song = nullptr;
        }
        else
        {
            std::cout << "❌ No song is currently playing!" << std::endl;
        }
    }

    /**
     * Auto-play next song with smart selection
     * Avoids recently skipped songs unless no alternatives
     * Time Complexity: O(n) where n is playlist size
     * Space Complexity: O(1)
     */
    Song *autoPlayNext()
    {
        auto all_songs = playlist.getAllSongs();

        // First, try to find a song that wasn't recently skipped
        for (Song *song : all_songs)
        {
            if (song && !skipped_tracker.wasRecentlySkipped(song->id))
            {
                playSong(song->id);
                return song;
            }
        }

        // If all songs were skipped, check for auto-replay
        if (all_songs.empty() || checkAndHandlePlaylistEnd())
        {
            return nullptr;
        }

        // Last resort: play a recently skipped song
        if (!all_songs.empty())
        {
            std::cout << "🔄 All songs were recently skipped. Playing least recently skipped..." << std::endl;
            playSong(all_songs[0]->id);
            return all_songs[0];
        }

        return nullptr;
    }

    /**
     * Check if playlist ended and handle auto-replay for calming genres
     * Time Complexity: O(n log n) for auto-replay setup
     * Space Complexity: O(k) where k is replay queue size
     */
    bool checkAndHandlePlaylistEnd()
    {
        if (playlist.getAllSongs().empty())
        {
            playlist_ended = true;
            std::cout << "📋 Playlist ended!" << std::endl;

            if (replay_manager.isAutoReplayEnabled())
            {
                replay_manager.setupAutoReplay(songDatabase);

                Song *replay_song = replay_manager.getNextReplaySong();
                if (replay_song)
                {
                    std::cout << "🔁 Auto-replaying calming song..." << std::endl;
                    playSong(replay_song->id);
                    return false; // Continue playing
                }
            }

            std::cout << "🎵 No more songs to play. Add more songs or enable auto-replay!" << std::endl;
            return true; // Playlist truly ended
        }
        return false;
    }

    void undoLastPlay()
    {
        Song *song = history.undo_last_play();
        if (song)
        {
            playlist.add_song(song);
            std::cout << "Re-added to playlist: " << song->toString() << std::endl;
        }
    }

    void sortPlaylist(PlaylistSorter::SortCriteria criteria, bool useQuickSort = false)
    {
        auto songs = playlist.getAllSongs();

        auto start = std::chrono::high_resolution_clock::now();
        if (useQuickSort)
        {
            PlaylistSorter::quickSort(songs, criteria);
        }
        else
        {
            PlaylistSorter::mergeSort(songs, criteria);
        }
        auto end = std::chrono::high_resolution_clock::now();

        playlist.rebuildFromVector(songs);

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Sorting completed in " << duration.count() << " microseconds" << std::endl;
    }

    /**
     * System Snapshot for Dashboard
     * Time Complexity: O(n log n) due to sorting for top longest songs
     * Space Complexity: O(n)
     */
    struct SystemSnapshot
    {
        std::vector<Song *> top_longest_songs;
        std::vector<Song *> recently_played;
        std::unordered_map<int, int> rating_counts;
        int total_songs;
        int playlist_size;
    };

    SystemSnapshot export_snapshot()
    {
        SystemSnapshot snapshot;

        // Top 5 longest songs
        auto allSongs = songDatabase;
        PlaylistSorter::quickSort(allSongs, PlaylistSorter::DURATION_DESC);
        for (int i = 0; i < std::min(5, (int)allSongs.size()); i++)
        {
            snapshot.top_longest_songs.push_back(allSongs[i]);
        }

        // Recently played songs
        snapshot.recently_played = history.getRecentlyPlayed(5);

        // Song count by rating
        snapshot.rating_counts = ratingTree.getSongCountByRating();

        // General stats
        snapshot.total_songs = songDatabase.size();
        snapshot.playlist_size = playlist.getSize();

        return snapshot;
    }

    void displaySnapshot()
    {
        auto snapshot = export_snapshot();

        std::cout << "\n=== PlayWise System Snapshot ===" << std::endl;
        std::cout << "Total Songs: " << snapshot.total_songs << std::endl;
        std::cout << "Playlist Size: " << snapshot.playlist_size << std::endl;

        std::cout << "\nTop 5 Longest Songs:" << std::endl;
        for (size_t i = 0; i < snapshot.top_longest_songs.size(); i++)
        {
            std::cout << (i + 1) << ". " << snapshot.top_longest_songs[i]->toString() << std::endl;
        }

        std::cout << "\nRecently Played:" << std::endl;
        for (size_t i = 0; i < snapshot.recently_played.size(); i++)
        {
            std::cout << (i + 1) << ". " << snapshot.recently_played[i]->toString() << std::endl;
        }

        std::cout << "\nSong Count by Rating:" << std::endl;
        for (int rating = 1; rating <= 5; rating++)
        {
            int count = (snapshot.rating_counts.find(rating) != snapshot.rating_counts.end())
                            ? snapshot.rating_counts[rating]
                            : 0;
            std::cout << rating << " stars: " << count << " songs" << std::endl;
        }
        std::cout << "================================\n"
                  << std::endl;
    }

    // Accessor methods for testing
    PlaylistEngine &getPlaylist() { return playlist; }
    PlaybackHistory &getHistory() { return history; }
    SongRatingTree &getRatingTree() { return ratingTree; }
    InstantLookup &getLookup() { return lookup; }
    RecentlySkippedTracker &getSkippedTracker() { return skipped_tracker; }
    AutoReplayManager &getReplayManager() { return replay_manager; }

    /**
     * Display recently skipped songs
     */
    void displayRecentlySkipped() const
    {
        std::cout << "\n=== Recently Skipped Songs ===" << std::endl;
        if (skipped_tracker.empty())
        {
            std::cout << "No recently skipped songs." << std::endl;
        }
        else
        {
            auto skipped_ids = skipped_tracker.getRecentlySkipped();
            std::cout << "Last " << skipped_ids.size() << " skipped songs:" << std::endl;
            for (size_t i = 0; i < skipped_ids.size(); i++)
            {
                // Find song in database since lookup is const
                for (const Song *song : songDatabase)
                {
                    if (song && song->id == skipped_ids[i])
                    {
                        std::cout << (i + 1) << ". " << song->toString() << std::endl;
                        break;
                    }
                }
            }
        }
        std::cout << "=============================\n"
                  << std::endl;
    }

    /**
     * Clear recently skipped songs
     */
    void clearRecentlySkipped()
    {
        skipped_tracker.clear();
        std::cout << "✅ Recently skipped songs cleared!" << std::endl;
    }

    /**
     * Toggle auto-replay feature
     */
    void toggleAutoReplay()
    {
        bool current_state = replay_manager.isAutoReplayEnabled();
        replay_manager.enableAutoReplay(!current_state);
        std::cout << "🔄 Auto-replay " << (!current_state ? "enabled" : "disabled") << std::endl;
    }

    /**
     * Display songs by genre
     */
    void displaySongsByGenre() const
    {
        std::unordered_map<std::string, std::vector<Song *>> genre_map;

        for (Song *song : songDatabase)
        {
            genre_map[song->genre].push_back(song);
        }

        std::cout << "\n=== Songs by Genre ===" << std::endl;
        for (const auto &pair : genre_map)
        {
            std::cout << "\n📁 " << pair.first << " (" << pair.second.size() << " songs):" << std::endl;
            for (size_t i = 0; i < pair.second.size(); i++)
            {
                std::cout << "  " << (i + 1) << ". " << pair.second[i]->toString() << std::endl;
            }
        }
        std::cout << "======================\n"
                  << std::endl;
    }
};

/**
 * Interactive Menu System
 */
class InteractiveMenu
{
private:
    PlayWiseEngine engine;

public:
    void run()
    {
        std::cout << "=== PlayWise Music Engine ===" << std::endl;
        std::cout << "Welcome! Let's start by adding some sample songs...\n"
                  << std::endl;

        // Add some default songs to get started
        loadSampleSongs();

        int choice;
        do
        {
            displayMenu();
            std::cout << "Enter your choice: ";

            // Better input handling
            if (!(std::cin >> choice))
            {
                // Clear error state and skip invalid input
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input! Please enter a number." << std::endl;
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                continue;
            }
            std::cin.ignore(10000, '\n'); // Clear entire input buffer

            switch (choice)
            {
            case 1:
                addNewSong();
                break;
            case 2:
                displayPlaylist();
                break;
            case 3:
                playSongMenu();
                break;
            case 4:
                skipCurrentSongMenu();
                break;
            case 5:
                autoPlayNextMenu();
                break;
            case 6:
                undoLastPlayMenu();
                break;
            case 7:
                sortPlaylistMenu();
                break;
            case 8:
                searchSongsMenu();
                break;
            case 9:
                rateSongMenu();
                break;
            case 10:
                playlistManipulationMenu();
                break;
            case 11:
                viewHistoryMenu();
                break;
            case 12:
                recentlySkippedMenu();
                break;
            case 13:
                autoReplaySettingsMenu();
                break;
            case 14:
                engine.displaySongsByGenre();
                break;
            case 15:
                engine.displaySnapshot();
                break;
            case 0:
                std::cout << "Thank you for using PlayWise Music Engine!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice! Please try again." << std::endl;
            }

            if (choice != 0)
            {
                std::cout << "\n"; // Just add a newline without pause
            }

        } while (choice != 0);
    }

private:
    void displayMenu()
    {
        std::cout << "\n================================" << std::endl;
        std::cout << "     PLAYWISE MUSIC ENGINE" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "1.  Add New Song" << std::endl;
        std::cout << "2.  Display Playlist" << std::endl;
        std::cout << "3.  Play Song" << std::endl;
        std::cout << "4.  Skip Current Song" << std::endl;
        std::cout << "5.  Auto-Play Next" << std::endl;
        std::cout << "6.  Undo Last Play" << std::endl;
        std::cout << "7.  Sort Playlist" << std::endl;
        std::cout << "8.  Search Songs" << std::endl;
        std::cout << "9.  Rate Song" << std::endl;
        std::cout << "10. Playlist Manipulation" << std::endl;
        std::cout << "11. View Play History" << std::endl;
        std::cout << "12. Recently Skipped Songs" << std::endl;
        std::cout << "13. Auto-Replay Settings" << std::endl;
        std::cout << "14. Songs by Genre" << std::endl;
        std::cout << "15. System Dashboard" << std::endl;
        std::cout << "0.  Exit" << std::endl;
        std::cout << "================================" << std::endl;
    }

    void loadSampleSongs()
    {
        engine.addSong("001", "Bohemian Rhapsody", "Queen", 355, 5, "Rock");
        engine.addSong("002", "Imagine", "John Lennon", 183, 5, "Pop");
        engine.addSong("003", "Billie Jean", "Michael Jackson", 294, 4, "Pop");
        engine.addSong("004", "Sweet Child O' Mine", "Guns N' Roses", 356, 4, "Rock");
        engine.addSong("005", "Hotel California", "Eagles", 391, 5, "Rock");

        // Add some calming genre songs for auto-replay testing
        engine.addSong("006", "Miles Runs the Voodoo Down", "Miles Davis", 420, 4, "Jazz");
        engine.addSong("007", "Clair de Lune", "Claude Debussy", 300, 5, "Classical");
        engine.addSong("008", "Lofi Hip Hop Beat", "ChillHop Cafe", 180, 3, "Lo-Fi");
        engine.addSong("009", "Ambient Soundscape", "Brian Eno", 480, 4, "Ambient");

        std::cout << "Sample songs loaded successfully (including calming genres for auto-replay)!\n"
                  << std::endl;
    }

    void addNewSong()
    {
        std::string id, title, artist, genre;
        int duration, rating;

        std::cout << "\n--- Add New Song ---" << std::endl;
        std::cout << "Enter Song ID: ";
        std::getline(std::cin, id);

        std::cout << "Enter Song Title: ";
        std::getline(std::cin, title);

        std::cout << "Enter Artist Name: ";
        std::getline(std::cin, artist);

        std::cout << "Enter Genre: ";
        std::getline(std::cin, genre);

        std::cout << "Enter Duration (seconds): ";
        if (!(std::cin >> duration))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid duration! Using default 180 seconds." << std::endl;
            duration = 180;
        }

        std::cout << "Enter Rating (1-5, 0 for no rating): ";
        if (!(std::cin >> rating))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid rating! Using default 0." << std::endl;
            rating = 0;
        }
        std::cin.ignore(10000, '\n'); // Clear buffer for next inputs

        if (rating < 0 || rating > 5)
        {
            rating = 0;
            std::cout << "Invalid rating! Set to 0 (no rating)." << std::endl;
        }

        Song *newSong = engine.addSong(id, title, artist, duration, rating, genre);
        std::cout << "Song added successfully: " << newSong->toString() << std::endl;

        if (newSong->isCalmingGenre())
        {
            std::cout << "🎵 This is a calming genre - perfect for auto-replay!" << std::endl;
        }
    }

    void skipCurrentSongMenu()
    {
        std::cout << "\n--- Skip Current Song ---" << std::endl;
        engine.skipCurrentSong();
    }

    void autoPlayNextMenu()
    {
        std::cout << "\n--- Auto-Play Next Song ---" << std::endl;
        Song *next_song = engine.autoPlayNext();
        if (!next_song)
        {
            std::cout << "No more songs available for auto-play." << std::endl;
        }
    }

    void recentlySkippedMenu()
    {
        std::cout << "\n--- Recently Skipped Songs Management ---" << std::endl;
        std::cout << "1. View Recently Skipped Songs" << std::endl;
        std::cout << "2. Clear Recently Skipped List" << std::endl;

        int choice;
        std::cout << "Choose option: ";
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            return;
        }
        std::cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
            engine.displayRecentlySkipped();
            break;
        case 2:
            engine.clearRecentlySkipped();
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    void autoReplaySettingsMenu()
    {
        std::cout << "\n--- Auto-Replay Settings ---" << std::endl;
        std::cout << "1. Toggle Auto-Replay (Currently: "
                  << (engine.getReplayManager().isAutoReplayEnabled() ? "ON" : "OFF") << ")" << std::endl;
        std::cout << "2. View Auto-Replay Statistics" << std::endl;
        std::cout << "3. Force Setup Auto-Replay Queue" << std::endl;

        int choice;
        std::cout << "Choose option: ";
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            return;
        }
        std::cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
            engine.toggleAutoReplay();
            break;
        case 2:
            engine.getReplayManager().displayStats();
            break;
        case 3:
        {
            auto songs = engine.getPlaylist().getAllSongs();
            std::vector<Song *> all_songs;
            for (auto *song : songs)
            {
                all_songs.push_back(song);
            }
            engine.getReplayManager().setupAutoReplay(all_songs);
        }
        break;
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    void displayPlaylist()
    {
        std::cout << "\n--- Current Playlist ---" << std::endl;
        if (engine.getPlaylist().getSize() == 0)
        {
            std::cout << "Playlist is empty!" << std::endl;
        }
        else
        {
            engine.getPlaylist().display();
        }
    }

    void playSongMenu()
    {
        std::cout << "\n--- Play Song ---" << std::endl;
        displayPlaylist();

        if (engine.getPlaylist().getSize() == 0)
            return;

        std::string songId;
        std::cout << "\nEnter Song ID to play: ";
        std::getline(std::cin, songId);

        engine.playSong(songId);
    }

    void undoLastPlayMenu()
    {
        std::cout << "\n--- Undo Last Play ---" << std::endl;
        if (engine.getHistory().isEmpty())
        {
            std::cout << "No songs in play history!" << std::endl;
        }
        else
        {
            engine.undoLastPlay();
        }
    }

    void sortPlaylistMenu()
    {
        std::cout << "\n--- Sort Playlist ---" << std::endl;
        std::cout << "1. Title (A-Z)" << std::endl;
        std::cout << "2. Title (Z-A)" << std::endl;
        std::cout << "3. Duration (Shortest first)" << std::endl;
        std::cout << "4. Duration (Longest first)" << std::endl;
        std::cout << "5. Recently Added" << std::endl;

        int sortChoice;
        std::cout << "Choose sorting criteria: ";
        if (!(std::cin >> sortChoice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            return;
        }
        std::cin.ignore(10000, '\n');

        PlaylistSorter::SortCriteria criteria;
        switch (sortChoice)
        {
        case 1:
            criteria = PlaylistSorter::TITLE_ASC;
            break;
        case 2:
            criteria = PlaylistSorter::TITLE_DESC;
            break;
        case 3:
            criteria = PlaylistSorter::DURATION_ASC;
            break;
        case 4:
            criteria = PlaylistSorter::DURATION_DESC;
            break;
        case 5:
            criteria = PlaylistSorter::RECENTLY_ADDED;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            return;
        }

        char algorithmChoice;
        std::cout << "Use QuickSort? (y/n): ";
        std::cin >> algorithmChoice;
        std::cin.ignore(10000, '\n');

        bool useQuickSort = (algorithmChoice == 'y' || algorithmChoice == 'Y');
        engine.sortPlaylist(criteria, useQuickSort);

        std::cout << "\nPlaylist sorted!" << std::endl;
        engine.getPlaylist().display();
    }

    void searchSongsMenu()
    {
        std::cout << "\n--- Search Songs ---" << std::endl;
        std::cout << "1. Search by Song ID" << std::endl;
        std::cout << "2. Search by Title" << std::endl;
        std::cout << "3. Search by Rating" << std::endl;

        int searchChoice;
        std::cout << "Choose search method: ";
        if (!(std::cin >> searchChoice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            return;
        }
        std::cin.ignore(10000, '\n');

        switch (searchChoice)
        {
        case 1:
        {
            std::string id;
            std::cout << "Enter Song ID: ";
            std::getline(std::cin, id);

            Song *song = engine.getLookup().lookup_by_id(id);
            if (song)
            {
                std::cout << "Found: " << song->toString() << std::endl;
            }
            else
            {
                std::cout << "Song not found!" << std::endl;
            }
            break;
        }
        case 2:
        {
            std::string title;
            std::cout << "Enter Song Title: ";
            std::getline(std::cin, title);

            auto songs = engine.getLookup().lookup_by_title(title);
            if (songs.empty())
            {
                std::cout << "No songs found with that title!" << std::endl;
            }
            else
            {
                std::cout << "Found " << songs.size() << " song(s):" << std::endl;
                for (size_t i = 0; i < songs.size(); i++)
                {
                    std::cout << (i + 1) << ". " << songs[i]->toString() << std::endl;
                }
            }
            break;
        }
        case 3:
        {
            int rating;
            std::cout << "Enter Rating (1-5): ";
            if (!(std::cin >> rating))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input!" << std::endl;
                return;
            }
            std::cin.ignore(10000, '\n');

            auto songs = engine.getRatingTree().search_by_rating(rating);
            if (songs.empty())
            {
                std::cout << "No songs found with " << rating << " star rating!" << std::endl;
            }
            else
            {
                std::cout << "Found " << songs.size() << " song(s) with " << rating << " stars:" << std::endl;
                for (size_t i = 0; i < songs.size(); i++)
                {
                    std::cout << (i + 1) << ". " << songs[i]->toString() << std::endl;
                }
            }
            break;
        }
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    void rateSongMenu()
    {
        std::cout << "\n--- Rate Song ---" << std::endl;
        displayPlaylist();

        if (engine.getPlaylist().getSize() == 0)
            return;

        std::string songId;
        std::cout << "\nEnter Song ID to rate: ";
        std::getline(std::cin, songId);

        Song *song = engine.getLookup().lookup_by_id(songId);
        if (!song)
        {
            std::cout << "Song not found!" << std::endl;
            return;
        }

        int rating;
        std::cout << "Current rating: " << (song->rating == 0 ? "No rating" : std::to_string(song->rating)) << std::endl;
        std::cout << "Enter new rating (1-5): ";
        if (!(std::cin >> rating))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            return;
        }
        std::cin.ignore(10000, '\n');

        if (rating < 1 || rating > 5)
        {
            std::cout << "Invalid rating! Must be between 1-5." << std::endl;
            return;
        }

        engine.getRatingTree().insert_song(song, rating);
        std::cout << "Rating updated for: " << song->toString() << std::endl;
    }

    void playlistManipulationMenu()
    {
        std::cout << "\n--- Playlist Manipulation ---" << std::endl;
        std::cout << "1. Move Song" << std::endl;
        std::cout << "2. Delete Song" << std::endl;
        std::cout << "3. Reverse Playlist" << std::endl;

        int choice;
        std::cout << "Choose operation: ";
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input!" << std::endl;
            return;
        }
        std::cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1:
        {
            displayPlaylist();
            if (engine.getPlaylist().getSize() == 0)
                return;

            int fromIndex, toIndex;
            std::cout << "Enter source index: ";
            if (!(std::cin >> fromIndex))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input!" << std::endl;
                return;
            }
            std::cout << "Enter destination index: ";
            if (!(std::cin >> toIndex))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input!" << std::endl;
                return;
            }
            std::cin.ignore(10000, '\n');

            if (engine.getPlaylist().move_song(fromIndex, toIndex))
            {
                std::cout << "Song moved successfully!" << std::endl;
                engine.getPlaylist().display();
            }
            else
            {
                std::cout << "Failed to move song! Check indices." << std::endl;
            }
            break;
        }
        case 2:
        {
            displayPlaylist();
            if (engine.getPlaylist().getSize() == 0)
                return;

            int index;
            std::cout << "Enter index to delete: ";
            if (!(std::cin >> index))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input!" << std::endl;
                return;
            }
            std::cin.ignore(10000, '\n');

            if (engine.getPlaylist().delete_song(index))
            {
                std::cout << "Song deleted successfully!" << std::endl;
                engine.getPlaylist().display();
            }
            else
            {
                std::cout << "Failed to delete song! Check index." << std::endl;
            }
            break;
        }
        case 3:
        {
            engine.getPlaylist().reverse_playlist();
            std::cout << "Playlist reversed!" << std::endl;
            engine.getPlaylist().display();
            break;
        }
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    }

    void viewHistoryMenu()
    {
        std::cout << "\n--- Play History ---" << std::endl;

        if (engine.getHistory().isEmpty())
        {
            std::cout << "No play history available!" << std::endl;
            return;
        }

        std::cout << "Total songs played: " << engine.getHistory().size() << std::endl;

        auto recentSongs = engine.getHistory().getRecentlyPlayed(10);
        std::cout << "\nRecently played songs:" << std::endl;
        for (size_t i = 0; i < recentSongs.size(); i++)
        {
            std::cout << (i + 1) << ". " << recentSongs[i]->toString() << std::endl;
        }
    }
};

/**
 * Demo and Testing Function (kept for backward compatibility)
 */
void runDemo()
{
    PlayWiseEngine engine;

    std::cout << "=== PlayWise Music Engine Demo ===" << std::endl;

    // Add some sample songs with genres
    engine.addSong("001", "Bohemian Rhapsody", "Queen", 355, 5, "Rock");
    engine.addSong("002", "Imagine", "John Lennon", 183, 5, "Pop");
    engine.addSong("003", "Billie Jean", "Michael Jackson", 294, 4, "Pop");
    engine.addSong("004", "Sweet Child O' Mine", "Guns N' Roses", 356, 4, "Rock");
    engine.addSong("005", "Hotel California", "Eagles", 391, 5, "Rock");
    engine.addSong("006", "Miles Runs the Voodoo Down", "Miles Davis", 420, 5, "Jazz");
    engine.addSong("007", "Clair de Lune", "Claude Debussy", 300, 5, "Classical");
    engine.addSong("008", "Lofi Hip Hop Beat", "ChillHop Cafe", 180, 3, "Lo-Fi");

    std::cout << "\nInitial Playlist:" << std::endl;
    engine.getPlaylist().display();

    // Test playback history and new features
    std::cout << "\n--- Testing Playback and New Features ---" << std::endl;
    engine.playSong("001");
    engine.playSong("006"); // Jazz song
    engine.playSong("007"); // Classical song
    engine.playSong("008"); // Lo-Fi song

    std::cout << "\n--- Testing Skip Feature ---" << std::endl;
    engine.skipCurrentSong();

    std::cout << "\n--- Testing Auto-Play Next ---" << std::endl;
    engine.autoPlayNext();

    std::cout << "\n--- Display Recently Skipped ---" << std::endl;
    engine.displayRecentlySkipped();

    std::cout << "\n--- Testing Auto-Replay Manager ---" << std::endl;
    engine.getReplayManager().displayStats();

    // Test sorting
    std::cout << "\n--- Testing Sorting ---" << std::endl;
    engine.sortPlaylist(PlaylistSorter::DURATION_DESC);
    engine.getPlaylist().display();

    // Display songs by genre
    engine.displaySongsByGenre();

    // Display system snapshot
    engine.displaySnapshot();
}

int main()
{
    std::cout << "=== Welcome to PlayWise Music Engine ===" << std::endl;
    std::cout << "Choose mode:" << std::endl;
    std::cout << "1. Interactive Mode (Recommended)" << std::endl;
    std::cout << "2. Demo Mode" << std::endl;
    std::cout << "Enter choice (1 or 2): ";

    int mode;
    if (!(std::cin >> mode))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input! Running interactive mode by default..." << std::endl;
        mode = 1;
    }
    else
    {
        std::cin.ignore(10000, '\n'); // Clear input buffer
    }

    if (mode == 1)
    {
        InteractiveMenu menu;
        menu.run();
    }
    else if (mode == 2)
    {
        runDemo();
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
    }
    else
    {
        std::cout << "Invalid choice! Running interactive mode by default..." << std::endl;
        InteractiveMenu menu;
        menu.run();
    }

    return 0;
}