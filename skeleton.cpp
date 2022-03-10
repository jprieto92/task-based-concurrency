/*
 * @file 	skeleton.cpp
 * @authors	Saul Alonso Monsalve (100303517), Javier Prieto Cepeda (100307011)
 * @brief 	First assignment's stub
 */

#include <vector>
#include <future>
#include <chrono>
#include <iostream>
#include <list>
#include "skeleton.hpp"

using namespace std;

/* Function headers */

template<typename T> 
int match(const vector<vector<T>>& chunk, const list<T>& pattern); 
 
template<typename T> 
int reduce(const vector<T>& chunk);

/*
 *	Match function
 */
template<typename T>
int match(const vector<vector<T>>& chunk, const list<T>& pattern)
{
	// Result with the number of coincidences
	int result;

	// Coincidences vector
	vector<bool> coincidences;

	// Iterate over chunk
	for(auto it = chunk.begin(); it != chunk.end(); ++it){
		size_t count = 0;
		auto it_pattern = pattern.begin();

		// Iterate over the vector and the pattern
		for(auto it_chunk = (*it).begin(); it_chunk != (*it).end(); ++it_chunk, ++it_pattern)
			if(*it_chunk == *it_pattern) count++;
		
		// If number of coincidences == pattern size --> true, else --> false
		if(count == pattern.size())
			coincidences.push_back(true);
		else
			coincidences.push_back(false);
	}

	// Call reduce function in order to get a result
	result = reduce<bool>(coincidences);
	
	return result;
}

/*
 *	Reduce function
 */
template<typename T>
int reduce(const vector<T>& chunk)
{
	int result = 0;	// Number of coincidences

	// Iterate and accumulate coincidences
	for(auto it = chunk.begin(); it != chunk.end(); ++it){
		result += (int)(*it);			
	}

	return result;
}

/*
 *	Spawn task
 */
template<typename T>
future<int> spawn_task(const vector<vector<T>>& chunk, const list<T>& pattern){
	// Create packaged_task
	packaged_task<int(vector<vector<T>>, list<T>)> pt(move(match<T>));
	
	// Get future
	auto res(pt.get_future());

	// Create thread
	thread t(move(pt), chunk, pattern);

	// Make thread detached
	t.detach();

	// Return future
	return res;
}

/*
 *	Main
 */
int main ()
{
	// Elements vector
	vector<vector<float>> elements(NUM_ELEMENTS);

	// Partial results
	vector<int> partial_results;

	int result = 0;

	float R1;
	float R2;

	// Future
	vector<future<int>> futures_match;

	// Initialize pattern to match
	list<float> pattern;
	pattern.push_back(Pattern1);
	pattern.push_back(Pattern2);	

	// Fill elements vector	
	for (int i=0; i<NUM_ELEMENTS; i++)
	{
		R1 = rand() % 10;
		R2 = rand() % 10;
		elements[i] = {R1, R2};
	}

	// Start time
	auto begin = std::chrono::high_resolution_clock::now();

	// Compute number of chunks (mind cases in which the number of chunks is not an integer)
	int num_chunks = NUM_ELEMENTS / CHUNK_SIZE;

	// N/M concurrent tasks
	for (int i=0; i<num_chunks; i++)
	{
		// Grab the chunk
		vector<vector<float>> chunk(&elements[i*CHUNK_SIZE], &elements[(i+1)*CHUNK_SIZE]);

		// Store future
		futures_match.push_back(spawn_task(chunk, pattern));
	}

	// Get tasks results
	for(int i=0; i<num_chunks; i++){
		// Get future
		partial_results.push_back(futures_match[i].get());
	}

	// Get result
	result = reduce<int>(partial_results);

	// Print the result
	cout << "Result: (" << result << ")" << endl;

	// End time
	auto end = std::chrono::high_resolution_clock::now();
  	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << "ns" << std::endl;

	return 0;
}



