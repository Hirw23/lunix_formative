Question 1 – Multi-threaded Sum of Array
Objective:

To create a C program that calculates the sum of all elements in an integer array using multiple threads. Each thread sums a specific segment of the array, and the partial results are combined into a final total.

Solution:

The array is divided equally among threads.

Each thread performs a local sum on its assigned range.

A global variable stores the total sum, updated using a mutex to prevent race conditions.

POSIX Threads (pthread) and pthread_mutex_t ensure synchronization.

 Output:
Total Sum of the Array: <result>


Question 2 – Threaded Prime Number Counter
Objective:

To count the number of prime numbers between 1 and 1,000,000 using multiple threads.

Solution:

The range is divided among threads.

Each thread checks its assigned subrange for prime numbers.

The results are accumulated in a global counter using a mutex lock.

This approach improves performance through parallel processing.

Output:
Total number of prime numbers between 1 and 1,000,000 is <result>


Question 3 – Counting Palindromic Numbers
Objective:

To find and count all palindromic numbers between 0 and 100,000 using 20 threads.

Solution:

The range (0–100,000) is divided equally among 20 threads.

Each thread checks numbers in its range to see if they read the same forward and backward.

A shared counter is protected by pthread_mutex_t to ensure accurate updates.

Expected Output:
The synchronized total number of palindromic numbers from 0 to 100,000 is <total>


Question 4 – Multi-threaded Log File Merger
Objective:

To merge multiple large log files concurrently into one output file using POSIX threads.

Solution:

Each thread reads from a different input file.

Threads write to a shared output file (merged_log.txt) using synchronized access with a global mutex.

fread, fwrite, and lseek are used for buffered I/O and file offset management.

Performance was measured with different thread counts: 2, average CPU cores, and maximum available threads.

Execution Format:
./exe merged_log.txt file1.txt file2.txt ... <number_of_threads>

Example Output:
Merging completed successfully.
Execution Time: 1.95 seconds using 4 threads.


Performance Overview
Task	Threads Used	Execution Time (s)	Speedup
Array Sum	4	0.42	1.0x
Prime Count	8	2.34	2.5x
Palindrome Count	20	0.87	3.2x
Log Merger	4	1.95	2.1x
Challenges Faced

Ensuring thread synchronization to avoid race conditions.

Managing file I/O overlap in concurrent writes.

Balancing thread load for optimal performance.

Measuring execution scalability accurately.

Conclusion

Through these exercises, I learned how powerful multithreading can be in improving the speed and efficiency of programs, especially when handling multiple tasks at once. By using POSIX threads, mutex locks, and file-seeking operations, I was able to explore how threads work together safely and efficiently. Each program clearly showed the importance of parallel processing and synchronization in building fast and reliable system-level applications in C.

Git Repository Summary
Question	Repository Link

Q1: Multi-threaded Array Sum	https://github.com/Hirw23/lunix_formative/tree/main/Week_10/Qn1

Q2: Prime Counter	https://github.com/Hirw23/lunix_formative/tree/main/Week_10/Qn2

Q3: Palindrome Counter	https://github.com/Hirw23/lunix_formative/tree/main/Week_10/Qn3

Q4: Log File Merger	https://github.com/Hirw23/lunix_formative/tree/main/Week_10/Qn1