#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <thread>
#include <mutex>


#define NUM_THREADS 8


std::vector<bool> prime;
std::vector<unsigned int> res;
std::mutex vect_lock;

void sieve(unsigned int n) {
    prime.resize(n + 1, true);
    prime[0] = false;
    prime[1] = false;

    for (int i = 2; i * i <= n; i++) {
        if (prime[i])
            for (int j = i * i; j <= n; j += i) {
                prime[j] = false;
            }
    }
}

// std::vector<unsigned int> findPrimes(unsigned int start, unsigned int end) {
//     std::vector<unsigned int> result;
//     for (int i = start; i <= end; i++) {
//         if (prime[i]) {
            
//             result.push_back(i); // writing to vector not thread safe, must use mutex
            
//         }
//     }
 
//     return result;
// }

void findPrimes(unsigned int start, unsigned int end) {
    
    for (int i = start; i <= end; i++) {
        
        if (prime[i]) {
            vect_lock.lock();
            res.push_back(i); // writing to vector not thread safe, must use mutex
            vect_lock.unlock();
        }
    }
}

void splitWorkIntoThreads(unsigned int start, unsigned int end) {
    unsigned int diff = (end + 1) - start;
    unsigned int thread_range = diff / NUM_THREADS;
    int remainder = diff % NUM_THREADS;
    unsigned int new_end = 0;
    unsigned int new_start = 0;
    
    std::vector<std::thread> thread_vect;
    for (int i = 0; i < NUM_THREADS; i++) {
        new_end = new_start + thread_range + (remainder ? 1 : 0) - 1;
        thread_vect.emplace_back(findPrimes, new_start, new_end);
        new_start = new_end + 1;
        if (remainder)
            remainder--;
    }
    
    for (auto& th : thread_vect) {
        th.join();
    }
}

int main() {
    int time_start = clock();
    
    unsigned int n = 100000000;
    
    // serial computation
    sieve(n);
    
    // parallel computation
    splitWorkIntoThreads(2, n);
    
    unsigned long long int sum = 0;
    
    for (auto i : res) {
        //printf("%d\n", n);   
        sum += i;
    }
    
    int time_end = clock();
    
    FILE * fptr = fopen("primes.txt", "w");
    if (fptr == NULL) {
        printf("error opening file.\n");
        return -1;
    }   
    
    unsigned long int num_prime = res.size();
    fprintf(fptr, "%lfs %lu %llu\n", (time_end - time_start) / double(CLOCKS_PER_SEC), num_prime , sum);
    //printf("%lfs %lu %llu\n", (time_end - time_start) / double(CLOCKS_PER_SEC), num_prime , sum);
    for (unsigned int i = num_prime - 10; i < num_prime; i++) {
        fprintf(fptr, "%lu ", res[i]);   
        //printf("%lu ", res[i]);
    }
    
    fclose(fptr);
    return 0;
}