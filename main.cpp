
/*
 * Yasin Barkın Başaran ID: 28890
 * CS300 - TAKE HOME 4
 * */


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;
using namespace chrono;

// Function declarations
template <typename T> void heapInsert(T A[], int n);
template <typename T> T heapDelete(T A[], int n);
template <typename T> void HeapSort(T A[], int n);
template <typename T> void mySwap(T &a, T &b);
template <typename T> void InsertionSort(T A[], int n);
template <typename T> int QuickPartition(T A[], int l, int h);
template <typename T> void QuickSort(T A[], int l, int h);
template <typename T> void Merge(T A[], int l, int mid, int h);
template <typename T> void MergeSort(T A[], int l, int h);
template <typename T> int SequentialSearch(T A[], int n, const T& key);
template <typename T> int BinarySearchIterative(T A[], int n, const T& key);

// PhoneBookEntry struct
struct PhoneBookEntry {
    string name;
    string surname;
    string telephone;
    string city;

    bool operator<(const PhoneBookEntry& other) const {
        if (name == other.name) {
            return surname < other.surname;
        }
        return name < other.name;
    }

    bool operator>(const PhoneBookEntry& other) const {
        if (name == other.name) {
            return surname > other.surname;
        }
        return name > other.name;
    }

    bool operator==(const PhoneBookEntry& other) const {
        return name == other.name && surname == other.surname;
    }

    bool operator<=(const PhoneBookEntry& other) const {
        return *this < other || *this == other;
    }

    friend ostream& operator<<(ostream& os, const PhoneBookEntry& entry) {
        os << entry.name << " " << entry.surname << " " << entry.telephone << " " << entry.city;
        return os;
    }
};

// Read phone book from file
vector<PhoneBookEntry> readPhoneBook(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: File could not be opened." << endl;
        return {}; // Return an empty vector if the file cannot be opened.
    }
    vector<PhoneBookEntry> entries;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        PhoneBookEntry entry;
        if (iss >> entry.name >> entry.surname >> entry.telephone >> entry.city) {
            entries.push_back(entry);
        } else {
            cerr << "Warning: Failed to parse line '" << line << "'." << endl;
        }
    }
    return entries;
}

string getFilename() {
    cout << "Please enter the contact file name:" << endl;
    string filename;
    cin >> filename;
    return filename;
}

string getSearchQuery() {
    cout << "Please enter the word to be queried:" << endl;
    cin.ignore();  // Ignore newline left in the input stream
    string query;
    getline(cin, query);
    return query;
}

// Time measurement template function
template <typename Func, typename... Args>
long long measureTime(Func func, Args&&... args) {
    auto start = high_resolution_clock::now();
    func(forward<Args>(args)...);
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

// Swap function
template <typename T>
void mySwap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

// Insertion Sort
template <typename T>
void InsertionSort(T A[], int n) {
    int i, j;
    T x;
    for (i = 1; i < n; i++) {
        j = i - 1;
        x = A[i];
        while (j >= 0 && A[j] > x) {
            A[j + 1] = A[j];
            j--;
        }
        A[j + 1] = x;
    }
}

// Quick Sort
template <typename T>
int QuickPartition(T A[], int l, int h) {
    T pivot = A[l];
    int i = l, j = h;
    do {
        do { i++; } while (i <= h && A[i] <= pivot);
        do { j--; } while (A[j] > pivot);
        if (i < j) mySwap(A[i], A[j]);
    } while (i < j);
    mySwap(A[l], A[j]);
    return j;
}

template <typename T>
void QuickSort(T A[], int l, int h) {
    if (l < h) {
        int j = QuickPartition(A, l, h);
        QuickSort(A, l, j);
        QuickSort(A, j + 1, h);
    }
}

// Merge Sort
template <typename T>
void Merge(T A[], int l, int mid, int h) {
    int i = l, j = mid + 1, k = 0;
    vector<T> B(h - l + 1);
    while (i <= mid && j <= h) {
        if (A[i] < A[j])
            B[k++] = A[i++];
        else
            B[k++] = A[j++];
    }
    for (; i <= mid; i++)
        B[k++] = A[i];
    for (; j <= h; j++)
        B[k++] = A[j];
    for (i = l, k = 0; i <= h; i++, k++)
        A[i] = B[k];
}

template <typename T>
void MergeSort(T A[], int l, int h) {
    if (l < h) {
        int mid = (l + h) / 2;
        MergeSort(A, l, mid);
        MergeSort(A, mid + 1, h);
        Merge(A, l, mid, h);
    }
}

// Heap Sort
template <typename T>
void heapInsert(T A[], int n) {
    int i = n;
    while (i > 0 && A[i] > A[(i - 1) / 2]) {
        mySwap(A[i], A[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

template <typename T>
T heapDelete(T A[], int n) {
    T val = A[0];
    A[0] = A[n - 1];
    int i = 0, j = 1;
    while (j < n - 1) {
        if (j < n - 2 && A[j + 1] > A[j]) {
            j++;
        }
        if (A[i] < A[j]) {
            mySwap(A[i], A[j]);
            i = j;
            j = 2 * i + 1;
        } else {
            break;
        }
    }
    return val;
}

template <typename T>
void HeapSort(T A[], int n) {
    for (int i = 1; i < n; i++) {
        heapInsert(A, i);
    }
    for (int i = n - 1; i > 0; i--) {
        mySwap(A[0], A[i]);
        heapDelete(A, i);
    }
}

// Sequential Search
template <typename T>
int SequentialSearch(T A[], int n, const T& key) {
    for (int i = 0; i < n; i++) {
        if (A[i] == key) {
            return i;
        }
    }
    return -1;
}

// Binary Search
template <typename T>
int BinarySearchIterative(T A[], int n, const T& key) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (A[mid] == key) {
            return mid;
        } else if (A[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

template <typename T>
vector<T> SequentialSearchPartial(const T A[], int n, const string& query) {
    vector<T> results;
    for (int i = 0; i < n; ++i) {
        if (A[i].name.find(query) != string::npos) {
            results.push_back(A[i]);
        }
    }
    return results;
}

template <typename T>
vector<T> BinarySearchPartial(const T A[], int n, const string& query) {
    vector<T> results;
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (A[mid].name.find(query) != string::npos) {
            // Collect all matches around the found index
            int left = mid;
            while (left >= 0 && A[left].name.find(query) != string::npos) {
                results.push_back(A[left--]);
            }
            int right = mid + 1;
            while (right < n && A[right].name.find(query) != string::npos) {
                results.push_back(A[right++]);
            }
            break;
        } else if (A[mid].name < query) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return results;
}

int main() {
    string filename = getFilename();
    auto phoneBook = readPhoneBook(filename);

    string query = getSearchQuery();

    // Convert vector to array for sorting, assuming phoneBook data is read correctly
    int n = phoneBook.size();
    PhoneBookEntry* phoneBookArray = new PhoneBookEntry[n];
    for (int i = 0; i < n; i++) {
        phoneBookArray[i] = phoneBook[i];
    }

    // Create copies for each sorting algorithm
    PhoneBookEntry* phoneBookForInsertion = new PhoneBookEntry[n];
    PhoneBookEntry* phoneBookForQuick = new PhoneBookEntry[n];
    PhoneBookEntry* phoneBookForMerge = new PhoneBookEntry[n];
    PhoneBookEntry* phoneBookForHeap = new PhoneBookEntry[n];

    copy(phoneBookArray, phoneBookArray + n, phoneBookForInsertion);
    copy(phoneBookArray, phoneBookArray + n, phoneBookForQuick);
    copy(phoneBookArray, phoneBookArray + n, phoneBookForMerge);
    copy(phoneBookArray, phoneBookArray + n, phoneBookForHeap);

    // Sorting
    auto insertionTime = measureTime(InsertionSort<PhoneBookEntry>, phoneBookForInsertion, n);
    auto quickTime = measureTime(QuickSort<PhoneBookEntry>, phoneBookForQuick, 0, n - 1);
    auto mergeTime = measureTime(MergeSort<PhoneBookEntry>, phoneBookForMerge, 0, n - 1);
    auto heapTime = measureTime(HeapSort<PhoneBookEntry>, phoneBookForHeap, n);

    cout << endl;
    cout << "Sorting the vector copies" << endl;
    cout << "======================================" << endl;
    cout << "Quick Sort Time: " << quickTime << " Nanoseconds" << endl;
    cout << "Insertion Sort Time: " << insertionTime << " Nanoseconds" << endl;
    cout << "Merge Sort Time: " << mergeTime << " Nanoseconds" << endl;
    cout << "Heap Sort Time: " << heapTime << " Nanoseconds" << endl;
    cout << endl;

    // Parse query into name and surname for searching
    istringstream queryStream(query);
    PhoneBookEntry queryEntry;
    queryStream >> queryEntry.name >> queryEntry.surname;

    cout << "Searching for " << query << endl;
    cout << "======================================" << endl;

    long long sequentialSearchTime = 0;
    long long binarySearchTime = 0;

    vector<PhoneBookEntry> sequentialResults;

    // Perform Binary Search
    vector<PhoneBookEntry> binaryResults;
    if (queryEntry.surname.empty()) {
        // Perform Binary Search for all entries with the same name
        binarySearchTime = measureTime([&]() {
            binaryResults = BinarySearchPartial(phoneBookForHeap, n, queryEntry.name);
        });
    } else {
        binarySearchTime = measureTime([&]() {
            int index = BinarySearchIterative(phoneBookForHeap, n, queryEntry);
            if (index != -1) {
                binaryResults.push_back(phoneBookForHeap[index]);
            }
        });
    }

   // cout << "Search results for Binary Search:" << endl;
    if (!binaryResults.empty()) {
        for (const auto& entry : binaryResults) {
            cout << entry << endl;
        }
    } else {
        cout << query << " does NOT exist in the dataset" << endl;
    }
    cout << "Binary Search Time: " << binarySearchTime << " Nanoseconds" << endl;
    cout << endl;

    // Perform Sequential Search
    sequentialSearchTime = measureTime([&]() {
        if (queryEntry.surname.empty()) {
            sequentialResults = SequentialSearchPartial(phoneBookForInsertion, n, queryEntry.name);
        } else {
            int index = SequentialSearch(phoneBookForInsertion, n, queryEntry);
            if (index != -1) {
                sequentialResults.push_back(phoneBookForInsertion[index]);
            }
        }
    });

    cout << "Search results for Sequential Search:" << endl;
    if (!sequentialResults.empty()) {
        for (const auto& entry : sequentialResults) {
            cout << entry << endl;
        }
    } else {
        cout << query << " does NOT exist in the dataset" << endl;
    }
    cout << "Sequential Search Time: " << sequentialSearchTime << " Nanoseconds" << endl;
    cout << endl;

    // Calculate and display speedups
    double sequentialBinarySpeedup = static_cast<double>(sequentialSearchTime) / binarySearchTime;
    cout << "SpeedUp between Search Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << sequentialBinarySpeedup << endl;

    double insertionQuickSpeedup = static_cast<double>(insertionTime) / quickTime;
    double mergeQuickSpeedup = static_cast<double>(mergeTime) / quickTime;
    double heapQuickSpeedup = static_cast<double>(heapTime) / quickTime;

    cout << endl;
    cout << "SpeedUps between Sorting Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << insertionQuickSpeedup << endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << mergeQuickSpeedup << endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << heapQuickSpeedup << endl;

    // Clean up dynamically allocated memory
    delete[] phoneBookArray;
    delete[] phoneBookForInsertion;
    delete[] phoneBookForQuick;
    delete[] phoneBookForMerge;
    delete[] phoneBookForHeap;

    return 0;
}





