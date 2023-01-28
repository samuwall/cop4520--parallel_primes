# cop4520--parallel_primes        ( Samu Wallace )


compilation/execution: 
g++ threadsieveprime.cpp -o threadsieveprime.exe 
threadsieveprime

output will show in primes.txt

# report 

the "threadsieveprime.cpp" program first uses a sieve to mark all primes within a bool vector. 

then, it splits the work of finding these primes into 8 threads evenly through divvying up the total work by the number of threads
and having each run the findPrimes function.

with 8 threads and a range of 10^8, the program runs in ~3.8s, while in single-threaded mode it runs in about ~4s.

there are more performance gains to be made through also parallelizing the sieve algorithm, but the deadline is nearing and I
have found this program to be a good introduction into multithreading. 
