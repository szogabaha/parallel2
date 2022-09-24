#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "benchmark.hpp"
#include "sorted_list.hpp"
#include "list_cg_mutex.hpp"
#include "list_fg_mutex.hpp"
#include "list_cg_tatas.hpp"
#include "list_fg_tatas.hpp"
#include "list_fg_clh.hpp"

static const int DATA_VALUE_RANGE_MIN = 0;
static const int DATA_VALUE_RANGE_MAX = 256;
static const int DATA_PREFILL = 512;

template <typename List>
void read(List &l, int random)
{
	/* read operations: 100% count */
	l.count(random % DATA_VALUE_RANGE_MAX);
}

template <typename List>
void update(List &l, int random)
{
	/* update operations: 50% insert, 50% remove */
	auto choice = (random % (2 * DATA_VALUE_RANGE_MAX)) / DATA_VALUE_RANGE_MAX;
	if (choice == 0)
	{
		l.insert(random % DATA_VALUE_RANGE_MAX);
	}
	else
	{
		l.remove(random % DATA_VALUE_RANGE_MAX);
	}
}

template <typename List>
void mixed(List &l, int random)
{
	/* mixed operations: 6.25% update, 93.75% count */
	auto choice = (random % (32 * DATA_VALUE_RANGE_MAX)) / DATA_VALUE_RANGE_MAX;
	if (choice == 0)
	{
		l.insert(random % DATA_VALUE_RANGE_MAX);
	}
	else if (choice == 1)
	{
		l.remove(random % DATA_VALUE_RANGE_MAX);
	}
	else
	{
		l.count(random % DATA_VALUE_RANGE_MAX);
	}
}

void sorted_list_benchmark(int threadcnt, std::uniform_int_distribution<int> uniform_dist, std::mt19937 engine)
{
	if(threadcnt > 1)
	{
		printf("[Skipped] non-thread-safe benchmark. Thread count > 1\n");
		return;
	}
	std::string identifier("non-thread-safe:");
	{
		sorted_list<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}

		benchmark(threadcnt, identifier + "read", [&l1](int random)
				  { read(l1, random); });
		benchmark(threadcnt, identifier + "update", [&l1](int random)
				  { update(l1, random); });
	}
	{
		/* start with fresh list: update test left list in random size */
		sorted_list<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}
		benchmark(threadcnt, identifier + "mixed", [&l1](int random)
				  { mixed(l1, random); });
	}
}

void coarse_grained_mutex_benchmark(int threadcnt, std::uniform_int_distribution<int> uniform_dist, std::mt19937 engine)
{
	std::string identifier("coarse grained mutex:");
	{
		list_cg_mutex<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}

		benchmark(threadcnt, identifier + "read", [&l1](int random)
				  { read(l1, random); });
		benchmark(threadcnt, identifier + "update", [&l1](int random)
				  { update(l1, random); });
	}
	{
		/* start with fresh list: update test left list in random size */
		list_cg_mutex<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}
		benchmark(threadcnt, identifier + "mixed", [&l1](int random)
				  { mixed(l1, random); });
	}
}

void fine_grained_mutex_benchmark(int threadcnt, std::uniform_int_distribution<int> uniform_dist, std::mt19937 engine)
{
	std::string identifier("fine grained mutex:");
	{
		list_fg_mutex<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}

		benchmark(threadcnt, identifier + "read", [&l1](int random)
				  { read(l1, random); });
		benchmark(threadcnt, identifier + "update", [&l1](int random)
				  { update(l1, random); });
	}
	{
		/* start with fresh list: update test left list in random size */
		list_fg_mutex<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}
		benchmark(threadcnt, identifier + "mixed", [&l1](int random)
				  { mixed(l1, random); });
	}
}

void coarse_grained_tatas_benchmark(int threadcnt, std::uniform_int_distribution<int> uniform_dist, std::mt19937 engine)
{
	std::string identifier("coarse grained TATAS lock:");
	{
		list_cg_tatas<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}

		benchmark(threadcnt, identifier + "read", [&l1](int random)
				  { read(l1, random); });
		benchmark(threadcnt, identifier + "update", [&l1](int random)
				  { update(l1, random); });
	}
	{
		/* start with fresh list: update test left list in random size */
		list_cg_tatas<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}
		benchmark(threadcnt, identifier + "mixed", [&l1](int random)
				  { mixed(l1, random); });
	}
}

void fine_grained_tatas_benchmark(int threadcnt, std::uniform_int_distribution<int> uniform_dist, std::mt19937 engine)
{
	using namespace TATAS;
	std::string identifier("fine grained TATAS lock:");
	{
		list_fg_tatas<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}

		benchmark(threadcnt, identifier + "read", [&l1](int random)
				  { read(l1, random); });
		benchmark(threadcnt, identifier + "update", [&l1](int random)
				  { update(l1, random); });
	}
	{
		/* start with fresh list: update test left list in random size */
		list_fg_tatas<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}
		benchmark(threadcnt, identifier + "mixed", [&l1](int random)
				  { mixed(l1, random); });
	}
}

void fine_grained_clh_benchmark(int threadcnt, std::uniform_int_distribution<int> uniform_dist, std::mt19937 engine)
{
	using namespace CLH;
	std::string identifier("fine grained CLH lock:");
	{
		list_fg_clh<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}

		benchmark(threadcnt, identifier + "read", [&l1](int random)
				  { read(l1, random); });
		benchmark(threadcnt, identifier + "update", [&l1](int random)
				  { update(l1, random); });
	}
	{
		/* start with fresh list: update test left list in random size */
		list_fg_clh<int> l1;
		/* prefill list with 1024 elements */
		for (int i = 0; i < DATA_PREFILL; i++)
		{
			l1.insert(uniform_dist(engine));
		}
		benchmark(threadcnt, identifier + "mixed", [&l1](int random)
				  { mixed(l1, random); });
	}
}

int main(int argc, char *argv[])
{
	/* get number of threads from command line */
	if (argc < 2)
	{
		std::cerr << u8"Please specify number of worker threads: " << argv[0] << u8" <number>\n";
		std::exit(EXIT_FAILURE);
	}
	std::istringstream ss(argv[1]);
	int threadcnt;
	if (!(ss >> threadcnt))
	{
		std::cerr << u8"Invalid number of threads '" << argv[1] << u8"'\n";
		std::exit(EXIT_FAILURE);
	}
	/* set up random number generator */
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution<int> uniform_dist(DATA_VALUE_RANGE_MIN, DATA_VALUE_RANGE_MAX);

	/* Benchmarks */
	printf("======================================================\n");
	sorted_list_benchmark(threadcnt, uniform_dist, engine);
	printf("======================================================\n");
	coarse_grained_mutex_benchmark(threadcnt, uniform_dist, engine);
	printf("======================================================\n");
	fine_grained_mutex_benchmark(threadcnt, uniform_dist, engine);
	printf("======================================================\n");
	coarse_grained_tatas_benchmark(threadcnt, uniform_dist, engine);
	printf("======================================================\n");
	fine_grained_tatas_benchmark(threadcnt, uniform_dist, engine);
	printf("======================================================\n");
	fine_grained_clh_benchmark(threadcnt, uniform_dist, engine);
	return EXIT_SUCCESS;
}
