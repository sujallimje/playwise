#include <iostream>
#include <cassert>
#include <vector>
#include <chrono>
#include <string>
#include <memory>
#include<bits/stdc++.h>
// Include the main PlayWise engine
// Note: In a real project, this would be split into header files


/**
 * Simple Test Framework
 */
class TestFramework {
private:
    static int total_tests;
    static int passed_tests;
    static std::string current_suite;

public:
    static void begin_suite(const std::string& suite_name) {
        current_suite = suite_name;
        std::cout << "\n=== Testing " << suite_name << " ===" << std::endl;
    }

    static void test(const std::string& test_name, bool condition) {
        total_tests++;
        if (condition) {
            passed_tests++;
            std::cout << ". " << test_name << std::endl;
        } else {
            std::cout << "! " << test_name << " FAILED" << std::endl;
        }
    }

    static void summary() {
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Passed: " << passed_tests << "/" << total_tests << std::endl;
        std::cout << "Success Rate: " << (100.0 * passed_tests / total_tests) << "%" << std::endl;
        
        if (passed_tests == total_tests) {
            std::cout << " All tests passed!" << std::endl;
        } else {
            std::cout << " Some tests failed." << std::endl;
        }
    }
};

int TestFramework::total_tests = 0;
int TestFramework::passed_tests = 0;
std::string TestFramework::current_suite = "";

/**
 * Mock Song class for testing (simplified version)
 */
class TestSong {
public:
    std::string id, title, artist;
    int duration, rating;
    
    TestSong(const std::string& id, const std::string& title, 
             const std::string& artist, int duration, int rating = 0)
        : id(id), title(title), artist(artist), duration(duration), rating(rating) {}
    
    bool operator==(const TestSong& other) const {
        return id == other.id;
    }
};

/**
 * Test Doubly Linked List Implementation
 */
class TestNode {
public:
    TestSong* song;
    TestNode* next;
    TestNode* prev;
    
    TestNode(TestSong* s) : song(s), next(nullptr), prev(nullptr) {}
};

class TestPlaylist {
private:
    TestNode* head;
    TestNode* tail;
    int size;
    
public:
    TestPlaylist() : head(nullptr), tail(nullptr), size(0) {}
    
    ~TestPlaylist() {
        clear();
    }
    
    void add_song(TestSong* song) {
        TestNode* newNode = new TestNode(song);
        
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
    
    bool delete_song(int index) {
        if (index < 0 || index >= size) return false;
        
        TestNode* current = getNodeAt(index);
        if (!current) return false;
        
        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }
        
        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev;
        }
        
        delete current;
        size--;
        return true;
    }
    
    bool move_song(int from_index, int to_index) {
        if (from_index < 0 || from_index >= size || 
            to_index < 0 || to_index >= size || 
            from_index == to_index) return false;
        
        TestNode* node = getNodeAt(from_index);
        if (!node) return false;
        
        // Remove from current position
        if (node->prev) node->prev->next = node->next;
        else head = node->next;
        
        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;
        
        // Insert at new position
        if (to_index == 0) {
            node->prev = nullptr;
            node->next = head;
            if (head) head->prev = node;
            head = node;
        } else {
            TestNode* target = getNodeAt(to_index);
            if (target) {
                node->next = target;
                node->prev = target->prev;
                if (target->prev) target->prev->next = node;
                target->prev = node;
            }
        }
        
        return true;
    }
    
    void reverse() {
        if (size <= 1) return;
        
        TestNode* current = head;
        TestNode* temp = nullptr;
        
        while (current) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }
        
        temp = head;
        head = tail;
        tail = temp;
    }
    
    TestSong* get_song_at(int index) {
        TestNode* node = getNodeAt(index);
        return node ? node->song : nullptr;
    }
    
    int get_size() const { return size; }
    
private:
    TestNode* getNodeAt(int index) {
        if (index < 0 || index >= size) return nullptr;
        
        TestNode* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    }
    
    void clear() {
        while (head) {
            TestNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }
};

/**
 * Performance Testing Utilities
 */
class PerformanceTest {
public:
    template<typename Func>
    static double measureTime(Func&& func, const std::string& operation_name) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double time_ms = duration.count() / 1000.0;
        
        std::cout << operation_name << " took: " << time_ms << " ms" << std::endl;
        return time_ms;
    }
};

/**
 * Test Functions
 */
void test_playlist_operations() {
    TestFramework::begin_suite("Playlist Operations (Doubly Linked List)");
    
    TestPlaylist playlist;
    
    // Create test songs
    TestSong song1("001", "Song 1", "Artist 1", 180);
    TestSong song2("002", "Song 2", "Artist 2", 240);
    TestSong song3("003", "Song 3", "Artist 3", 200);
    TestSong song4("004", "Song 4", "Artist 4", 220);
    
    // Test adding songs
    playlist.add_song(&song1);
    playlist.add_song(&song2);
    playlist.add_song(&song3);
    TestFramework::test("Add songs to playlist", playlist.get_size() == 3);
    
    // Test getting songs by index
    TestFramework::test("Get first song", playlist.get_song_at(0) == &song1);
    TestFramework::test("Get middle song", playlist.get_song_at(1) == &song2);
    TestFramework::test("Get last song", playlist.get_song_at(2) == &song3);
    
    // Test moving songs
    bool move_success = playlist.move_song(0, 2); // Move first to last position
    TestFramework::test("Move song operation", move_success);
    TestFramework::test("Song moved correctly", playlist.get_song_at(2) == &song1);
    
    // Test deleting songs
    bool delete_success = playlist.delete_song(1); // Delete middle song
    TestFramework::test("Delete song operation", delete_success);
    TestFramework::test("Playlist size after deletion", playlist.get_size() == 2);
    
    // Test reversing playlist
    playlist.add_song(&song4); // Add another song
    playlist.reverse();
    TestFramework::test("Reverse playlist", playlist.get_song_at(0) == &song4);
    
    // Test boundary conditions
    TestFramework::test("Invalid index access", playlist.get_song_at(10) == nullptr);
    TestFramework::test("Invalid delete", !playlist.delete_song(-1));
    TestFramework::test("Invalid move", !playlist.move_song(-1, 0));
}

void test_stack_operations() {
    TestFramework::begin_suite("Playback History (Stack)");
    
    std::stack<TestSong*> history;
    
    TestSong song1("001", "Song 1", "Artist 1", 180);
    TestSong song2("002", "Song 2", "Artist 2", 240);
    TestSong song3("003", "Song 3", "Artist 3", 200);
    
    // Test pushing songs
    history.push(&song1);
    history.push(&song2);
    history.push(&song3);
    TestFramework::test("Stack size after pushes", history.size() == 3);
    
    // Test LIFO behavior
    TestSong* last_played = history.top();
    TestFramework::test("Last played song (LIFO)", last_played == &song3);
    
    // Test popping
    history.pop();
    TestFramework::test("Stack size after pop", history.size() == 2);
    TestFramework::test("New top after pop", history.top() == &song2);
    
    // Test empty stack
    history.pop();
    history.pop();
    TestFramework::test("Empty stack", history.empty());
}

void test_sorting_algorithms() {
    TestFramework::begin_suite("Sorting Algorithms");
    
    // Create test data
    std::vector<TestSong*> songs;
    songs.push_back(new TestSong("003", "Charlie", "Artist C", 300));
    songs.push_back(new TestSong("001", "Alpha", "Artist A", 100));
    songs.push_back(new TestSong("004", "Delta", "Artist D", 400));
    songs.push_back(new TestSong("002", "Beta", "Artist B", 200));
    
    // Test sorting by title
    std::vector<TestSong*> title_sorted = songs;
    std::sort(title_sorted.begin(), title_sorted.end(), 
              [](TestSong* a, TestSong* b) { return a->title < b->title; });
    
    TestFramework::test("Sort by title - first", title_sorted[0]->title == "Alpha");
    TestFramework::test("Sort by title - last", title_sorted[3]->title == "Delta");
    
    // Test sorting by duration
    std::vector<TestSong*> duration_sorted = songs;
    std::sort(duration_sorted.begin(), duration_sorted.end(),
              [](TestSong* a, TestSong* b) { return a->duration < b->duration; });
    
    TestFramework::test("Sort by duration - shortest", duration_sorted[0]->duration == 100);
    TestFramework::test("Sort by duration - longest", duration_sorted[3]->duration == 400);
    
    // Cleanup
    for (TestSong* song : songs) {
        delete song;
    }
}

void test_hash_map_operations() {
    TestFramework::begin_suite("HashMap Lookup Operations");
    
    std::unordered_map<std::string, TestSong*> id_map;
    std::unordered_map<std::string, std::vector<TestSong*>> title_map;
    
    TestSong song1("001", "Song A", "Artist 1", 180);
    TestSong song2("002", "Song B", "Artist 2", 240);
    TestSong song3("003", "Song A", "Artist 3", 200); // Same title as song1
    
    // Test insertions
    id_map[song1.id] = &song1;
    id_map[song2.id] = &song2;
    id_map[song3.id] = &song3;
    
    title_map[song1.title].push_back(&song1);
    title_map[song2.title].push_back(&song2);
    title_map[song3.title].push_back(&song3); // Same title as song1
    
    // Test ID lookups (should be O(1))
    TestFramework::test("Lookup by ID - found", id_map["001"] == &song1);
    TestFramework::test("Lookup by ID - not found", id_map.find("999") == id_map.end());
    
    // Test title lookups (handling duplicates)
    TestFramework::test("Title lookup - unique title", title_map["Song B"].size() == 1);
    TestFramework::test("Title lookup - duplicate title", title_map["Song A"].size() == 2);
    TestFramework::test("Title lookup - contains both songs", 
                       std::find(title_map["Song A"].begin(), title_map["Song A"].end(), &song1) != title_map["Song A"].end() &&
                       std::find(title_map["Song A"].begin(), title_map["Song A"].end(), &song3) != title_map["Song A"].end());
}

void test_binary_search_tree() {
    TestFramework::begin_suite("Binary Search Tree (Rating System)");
    
    // Simple BST node for testing
    struct TestRatingNode {
        int rating;
        std::vector<TestSong*> songs;
        TestRatingNode* left;
        TestRatingNode* right;
        
        TestRatingNode(int r) : rating(r), left(nullptr), right(nullptr) {}
    };
    
    // Simple BST operations for testing
    auto insertRating = [](TestRatingNode*& root, TestSong* song, int rating) {
        std::function<TestRatingNode*(TestRatingNode*, TestSong*, int)> insertHelper;
        insertHelper = [&](TestRatingNode* node, TestSong* song, int rating) -> TestRatingNode* {
            if (!node) {
                TestRatingNode* newNode = new TestRatingNode(rating);
                newNode->songs.push_back(song);
                return newNode;
            }
            
            if (rating == node->rating) {
                node->songs.push_back(song);
            } else if (rating < node->rating) {
                node->left = insertHelper(node->left, song, rating);
            } else {
                node->right = insertHelper(node->right, song, rating);
            }
            return node;
        };
        root = insertHelper(root, song, rating);
    };
    
    std::function<TestRatingNode*(TestRatingNode*, int)> searchRating;
    searchRating = [&](TestRatingNode* root, int rating) -> TestRatingNode* {
        if (!root || root->rating == rating) return root;
        
        if (rating < root->rating) {
            return searchRating(root->left, rating);
        }
        return searchRating(root->right, rating);
    };
    
    TestRatingNode* root = nullptr;
    
    TestSong song1("001", "Song 1", "Artist 1", 180, 5);
    TestSong song2("002", "Song 2", "Artist 2", 240, 3);
    TestSong song3("003", "Song 3", "Artist 3", 200, 5); // Same rating as song1
    TestSong song4("004", "Song 4", "Artist 4", 220, 4);
    
    // Test insertions
    insertRating(root, &song1, 5);
    insertRating(root, &song2, 3);
    insertRating(root, &song3, 5);
    insertRating(root, &song4, 4);
    
    TestFramework::test("BST root created", root != nullptr);
    TestFramework::test("BST root rating", root->rating == 5);
    
    // Test searches
    TestRatingNode* rating5Node = searchRating(root, 5);
    TestFramework::test("Search rating 5 - found", rating5Node != nullptr);
    TestFramework::test("Search rating 5 - correct bucket size", rating5Node->songs.size() == 2);
    
    TestRatingNode* rating3Node = searchRating(root, 3);
    TestFramework::test("Search rating 3 - found", rating3Node != nullptr);
    TestFramework::test("Search rating 3 - correct song", rating3Node->songs[0] == &song2);
    
    TestRatingNode* rating1Node = searchRating(root, 1);
    TestFramework::test("Search rating 1 - not found", rating1Node == nullptr);
    
    // Cleanup (simplified - in real implementation would need proper tree deletion)
}

void test_performance_characteristics() {
    TestFramework::begin_suite("Performance Characteristics");
    
    const int LARGE_SIZE = 10000;
    
    // Test linked list performance
    {
        TestPlaylist large_playlist;
        std::vector<std::unique_ptr<TestSong>> songs;
        
        // Generate test songs
        for (int i = 0; i < LARGE_SIZE; i++) {
            songs.push_back(std::make_unique<TestSong>(
                "ID" + std::to_string(i),
                "Title" + std::to_string(i),
                "Artist" + std::to_string(i % 100), // 100 different artists
                180 + (i % 300) // Duration between 180-480 seconds
            ));
        }
        
        // Test insertion performance
        double insert_time = PerformanceTest::measureTime([&]() {
            for (auto& song : songs) {
                large_playlist.add_song(song.get());
            }
        }, "Insert " + std::to_string(LARGE_SIZE) + " songs");
        
        TestFramework::test("Large playlist created", large_playlist.get_size() == LARGE_SIZE);
        TestFramework::test("Insert performance reasonable", insert_time < 100.0); // Less than 100ms
        
        // Test reverse performance
        double reverse_time = PerformanceTest::measureTime([&]() {
            large_playlist.reverse();
        }, "Reverse " + std::to_string(LARGE_SIZE) + " songs");
        
        TestFramework::test("Reverse performance reasonable", reverse_time < 50.0); // Less than 50ms
    }
    
    // Test HashMap performance
    {
        std::unordered_map<std::string, TestSong*> large_map;
        std::vector<std::unique_ptr<TestSong>> songs;
        
        // Generate test songs
        for (int i = 0; i < LARGE_SIZE; i++) {
            songs.push_back(std::make_unique<TestSong>(
                "ID" + std::to_string(i),
                "Title" + std::to_string(i),
                "Artist" + std::to_string(i % 100),
                180 + (i % 300)
            ));
        }
        
        // Test insertion performance
        double map_insert_time = PerformanceTest::measureTime([&]() {
            for (auto& song : songs) {
                large_map[song->id] = song.get();
            }
        }, "HashMap insert " + std::to_string(LARGE_SIZE) + " songs");
        
        TestFramework::test("HashMap insertion performance", map_insert_time < 20.0); // Less than 20ms
        
        // Test lookup performance
        double lookup_time = PerformanceTest::measureTime([&]() {
            for (int i = 0; i < 1000; i++) {
                std::string id = "ID" + std::to_string(i);
                volatile auto result = large_map[id]; // volatile to prevent optimization
                (void)result; // Suppress unused variable warning
            }
        }, "HashMap lookup 1000 songs");
        
        TestFramework::test("HashMap lookup performance", lookup_time < 1.0); // Less than 1ms
    }
    
    // Test sorting performance
    {
        std::vector<std::unique_ptr<TestSong>> songs;
        
        // Generate random test data
        for (int i = 0; i < LARGE_SIZE; i++) {
            songs.push_back(std::make_unique<TestSong>(
                "ID" + std::to_string(i),
                "Title" + std::to_string(rand() % 1000), // Random titles for sorting
                "Artist" + std::to_string(rand() % 100),
                180 + (rand() % 300)
            ));
        }
        
        // Create vector of pointers for sorting
        std::vector<TestSong*> song_ptrs;
        for (auto& song : songs) {
            song_ptrs.push_back(song.get());
        }
        
        // Test std::sort performance (baseline)
        auto sort_copy = song_ptrs;
        double std_sort_time = PerformanceTest::measureTime([&]() {
            std::sort(sort_copy.begin(), sort_copy.end(),
                     [](TestSong* a, TestSong* b) { return a->title < b->title; });
        }, "std::sort " + std::to_string(LARGE_SIZE) + " songs");
        
        TestFramework::test("Sorting performance reasonable", std_sort_time < 100.0); // Less than 100ms
        
        // Verify sorting correctness
        bool is_sorted = std::is_sorted(sort_copy.begin(), sort_copy.end(),
                                       [](TestSong* a, TestSong* b) { return a->title < b->title; });
        TestFramework::test("Sorting correctness", is_sorted);
    }
}

void test_memory_management() {
    TestFramework::begin_suite("Memory Management");
    
    // Test for memory leaks in playlist operations
    {
        TestPlaylist* playlist = new TestPlaylist();
        
        // Add songs and perform operations
        for (int i = 0; i < 100; i++) {
            TestSong* song = new TestSong("ID" + std::to_string(i), 
                                         "Title" + std::to_string(i), 
                                         "Artist", 180);
            playlist->add_song(song);
        }
        
        // Delete some songs
        for (int i = 0; i < 50; i++) {
            playlist->delete_song(0);
        }
        
        TestFramework::test("Playlist operations completed", playlist->get_size() == 50);
        
        delete playlist;
        // Note: In a real application, we'd also need to delete the TestSong objects
        // This is a simplified test focusing on data structure memory management
    }
    
    // Test stack memory management
    {
        std::stack<TestSong*>* history = new std::stack<TestSong*>();
        
        for (int i = 0; i < 100; i++) {
            TestSong* song = new TestSong("ID" + std::to_string(i), "Title", "Artist", 180);
            history->push(song);
        }
        
        TestFramework::test("Stack operations completed", history->size() == 100);
        
        // Clear stack
        while (!history->empty()) {
            history->pop();
        }
        
        TestFramework::test("Stack cleared", history->empty());
        
        delete history;
    }
}

void test_edge_cases() {
    TestFramework::begin_suite("Edge Cases and Boundary Conditions");
    
    // Test empty playlist operations
    TestPlaylist empty_playlist;
    TestFramework::test("Empty playlist size", empty_playlist.get_size() == 0);
    TestFramework::test("Delete from empty playlist", !empty_playlist.delete_song(0));
    TestFramework::test("Move in empty playlist", !empty_playlist.move_song(0, 1));
    TestFramework::test("Get from empty playlist", empty_playlist.get_song_at(0) == nullptr);
    
    // Test single element playlist
    TestPlaylist single_playlist;
    TestSong single_song("001", "Only Song", "Only Artist", 180);
    single_playlist.add_song(&single_song);
    
    TestFramework::test("Single element playlist size", single_playlist.get_size() == 1);
    TestFramework::test("Reverse single element", single_playlist.get_song_at(0) == &single_song);
    single_playlist.reverse();
    TestFramework::test("Single element after reverse", single_playlist.get_song_at(0) == &single_song);
    
    // Test boundary indices
    TestPlaylist boundary_playlist;
    TestSong songs[3] = {
        TestSong("001", "Song 1", "Artist 1", 180),
        TestSong("002", "Song 2", "Artist 2", 240),
        TestSong("003", "Song 3", "Artist 3", 200)
    };
    
    for (int i = 0; i < 3; i++) {
        boundary_playlist.add_song(&songs[i]);
    }
    
    // Test negative indices
    TestFramework::test("Negative index access", boundary_playlist.get_song_at(-1) == nullptr);
    TestFramework::test("Negative index delete", !boundary_playlist.delete_song(-1));
    TestFramework::test("Negative index move", !boundary_playlist.move_song(-1, 0));
    
    // Test out of bounds indices
    TestFramework::test("Out of bounds access", boundary_playlist.get_song_at(10) == nullptr);
    TestFramework::test("Out of bounds delete", !boundary_playlist.delete_song(10));
    TestFramework::test("Out of bounds move", !boundary_playlist.move_song(0, 10));
    
    // Test same index move
    TestFramework::test("Same index move", !boundary_playlist.move_song(1, 1));
}

void run_comprehensive_tests() {
    std::cout << "=== PlayWise Music Engine - Comprehensive Test Suite ===\n" << std::endl;
    
    // Run all test suites
    test_playlist_operations();
    test_stack_operations();
    test_sorting_algorithms();
    test_hash_map_operations();
    test_binary_search_tree();
    test_performance_characteristics();
    test_memory_management();
    test_edge_cases();
    
    // Print final summary
    TestFramework::summary();
}

/**
 * Benchmark Tests
 */
void run_benchmarks() {
    std::cout << "\n=== Performance Benchmarks ===\n" << std::endl;
    
    const std::vector<int> sizes = {1000, 5000, 10000, 25000, 50000};
    
    std::cout << "Benchmark Results:" << std::endl;
    std::cout << "Size\tInsert(ms)\tReverse(ms)\tSort(ms)\tLookup(micros)" << std::endl;
    std::cout << "----\t----------\t----------\t--------\t----------" << std::endl;
    
    for (int size : sizes) {
        // Playlist operations
        TestPlaylist playlist;
        std::vector<std::unique_ptr<TestSong>> songs;
        
        for (int i = 0; i < size; i++) {
            songs.push_back(std::make_unique<TestSong>(
                "ID" + std::to_string(i),
                "Title" + std::to_string(rand() % 1000),
                "Artist" + std::to_string(rand() % 100),
                180 + (rand() % 300)
            ));
        }
        
        // Measure insertion
        auto start = std::chrono::high_resolution_clock::now();
        for (auto& song : songs) {
            playlist.add_song(song.get());
        }
        auto end = std::chrono::high_resolution_clock::now();
        double insert_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        
        // Measure reverse
        start = std::chrono::high_resolution_clock::now();
        playlist.reverse();
        end = std::chrono::high_resolution_clock::now();
        double reverse_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        
        // Measure sorting
        std::vector<TestSong*> song_ptrs;
        for (auto& song : songs) {
            song_ptrs.push_back(song.get());
        }
        
        start = std::chrono::high_resolution_clock::now();
        std::sort(song_ptrs.begin(), song_ptrs.end(),
                 [](TestSong* a, TestSong* b) { return a->title < b->title; });
        end = std::chrono::high_resolution_clock::now();
        double sort_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
        
        // Measure HashMap lookup
        std::unordered_map<std::string, TestSong*> lookup_map;
        for (auto& song : songs) {
            lookup_map[song->id] = song.get();
        }
        
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; i++) {
            std::string id = "ID" + std::to_string(i);
            volatile auto result = lookup_map[id];
            (void)result;
        }
        end = std::chrono::high_resolution_clock::now();
        double lookup_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 100.0 / 1000.0;
        
        std::cout << size << "\t" << std::fixed << std::setprecision(2) 
                  << insert_time << "\t\t" << reverse_time << "\t\t" 
                  << sort_time << "\t\t" << lookup_time << std::endl;
    }
    
    std::cout << "\nBenchmark completed! " << std::endl;
}

int main() {
    // Seed random number generator for consistent but varied test data
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::cout << "Choose test mode:" << std::endl;
    std::cout << "1. Comprehensive Tests" << std::endl;
    std::cout << "2. Performance Benchmarks" << std::endl;
    std::cout << "3. Both" << std::endl;
    std::cout << "Enter choice (1-3): ";
    
    int choice;
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            run_comprehensive_tests();
            break;
        case 2:
            run_benchmarks();
            break;
        case 3:
            run_comprehensive_tests();
            run_benchmarks();
            break;
        default:
            std::cout << "Invalid choice. Running comprehensive tests..." << std::endl;
            run_comprehensive_tests();
    }
    
    return 0;
}