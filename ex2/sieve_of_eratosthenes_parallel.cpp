#include <iostream>
#include <cmath>
#include <pthread.h>
#include <cstring>
//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
#include <chrono>
using namespace std::chrono;

class Natural {
    private:            
        int value;       
        bool marked; 
    public:
        Natural(int v = 0) {
            value = v;
            marked = false;
        }
        int getValue() {
            return value;
        }
        int getSquared() {
            return value * value;
        }
        void mark() {
            marked = true;
        }
        bool isMarked() {
            return marked;
        }
};

template<class T> class Array {
    private:
        T* array;
        size_t len;
    
    public:
        Array() {
            array = nullptr;
            len = 0;
        }

        Array(T* a, size_t s) {
            array = a;
            len = s;
        }

        size_t size() {
            return len;
        }
        
        T& operator[](const int index);
};

template <class T> T& Array<T>::operator[] (const int index) {
        return array[index];
}

Array<Natural> create_array(size_t min, size_t last_number) {
    size_t size = min == 0 ? last_number + 1 : last_number - min + 1;
    Natural* array = new Natural[size];
    for(size_t i = 0; i < size; i++){
        array[i] = Natural(i+min);
    }

    if (min == 0){
        array[0].mark();
        array[1].mark();
    } 

    return Array<Natural>(array, size);
}

Array<Natural> create_array(size_t max) {
    return create_array(0, max);
}

class Chunk {
    public:
        Array<Natural> numbers;
        Array<int> seeds;

        Chunk() {}

        Chunk(int min, size_t size, Array<int> seeds) {
            numbers = create_array(min, min + size - 1);
            this->seeds = seeds;
        }

        int size() {
            return numbers.size();
        }

        void print_unmarked(){
            for(size_t i = 0; i < numbers.size(); i++) {
                if(!numbers[i].isMarked()){
                    std::cout << numbers[i].getValue() << " ";
                }
            }
        }
};

Natural getNextUnmarked(Natural n, Array<Natural> array) {
    for(size_t i = n.getValue() + 1; i < array.size(); i++ ) {
        if(!array[i].isMarked()) {
            return array[i];
        }
    }
    return n;
}

Array<int> get_seeds(int max) {
    int max_squared = (int) floor(sqrt(max));
    int size = max_squared + 1;

    Array<Natural> numbers_until_squared = create_array(max_squared);
    Natural smallest_unmarked = Natural(2);
    int num_of_seeds = 0;
    while (smallest_unmarked.getSquared() <= max){
        num_of_seeds ++;
        for(int i = smallest_unmarked.getSquared(); i < size; i+=smallest_unmarked.getValue()){
            numbers_until_squared[i].mark();
        }
        Natural next_smallest = getNextUnmarked(smallest_unmarked, numbers_until_squared);
        if (next_smallest.getValue() != smallest_unmarked.getValue()) smallest_unmarked = next_smallest;
        else break;
    }

    int* calculated_seeds = new int[num_of_seeds];
    int cnt = 0;
    for(int i=0; i< size; i++) {
        if(!numbers_until_squared[i].isMarked()) calculated_seeds[cnt++] = numbers_until_squared[i].getValue();
    }
    return Array<int>(calculated_seeds, num_of_seeds);
}

Chunk* create_chunks(Array<Natural> array, int thread_num, Array<int> seeds) {
    int partial_size = array.size() / thread_num;
    
    Chunk* chunks = new Chunk[thread_num];
    for(int i = 0; i<thread_num -1; i++){
        Chunk newChunk = Chunk(array[0].getValue() + i * partial_size, partial_size, seeds);
        chunks[i] = newChunk;
    }

    int last_partition_size = partial_size + (array.size() % thread_num);
    chunks[thread_num -1] = Chunk(array[0].getValue() + (thread_num - 1) * partial_size, last_partition_size, seeds);
    return chunks;
}

void* mark_primes_in_chunk(void* args) {
    Chunk* chunk = (Chunk*) args;

    for(size_t seed_id = 0; seed_id < chunk -> seeds.size(); seed_id++){
        for(int i = 0; i < chunk -> size(); i ++){
            if(chunk ->numbers[i].getValue() % chunk ->seeds[seed_id] == 0) {
                chunk ->numbers[i].mark();
            }
        }
    }
    pthread_exit(0);
}

int main(int argc, char** argv) {
    int max = std::stoi(argv[1]);
    int number_of_threads = std::stoi(argv[2]);

    auto start = high_resolution_clock::now();
    Array<int> seeds = get_seeds(max);
    int min = (int) floor(sqrt(max)) + 1;
    
    Array<Natural> remaining = create_array(min, max);
    
    Chunk* chunks = create_chunks(remaining, number_of_threads, seeds);
    pthread_t* p_threads = (pthread_t*)malloc(sizeof(pthread_t) * number_of_threads);
    pthread_attr_t attr; 
	pthread_attr_init(&attr);

    for(int i = 0; i< number_of_threads; i++) {
        pthread_create(&p_threads[i], &attr, mark_primes_in_chunk, (void *)&chunks[i]);
    }
    for (int i = 0; i < number_of_threads; i++) { 
		if (pthread_join(p_threads[i], NULL));
    }
    auto stop = high_resolution_clock::now();


    if(argc == 4 && strcmp(argv[3],"--time") == 0) {
        std::cout << duration_cast<microseconds>(stop - start).count(); //µs
    } else {
        for (size_t i = 0; i< seeds.size(); i++){
            std::cout<<seeds[i] << " ";
        }

        for (int i = 0; i < number_of_threads; i++) {
            chunks[i].print_unmarked();
        }
    }

    free(p_threads);
}
