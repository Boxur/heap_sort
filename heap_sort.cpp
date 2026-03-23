#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

void heapify(std::vector<int>& vec, int n, int i)
{
	while (true)
	{
		int largest = i;
		int left = 2 * i + 1;
		int right = 2 * i + 2;

		if (left < n && vec[left] > vec[largest])
			largest = left;

		if (right < n && vec[right] > vec[largest])
			largest = right;

		if (largest == i)
			break;

		std::swap(vec[i], vec[largest]);
		i = largest;
	}
}

void heapSort(std::vector<int>& vec)
{
	int n = static_cast<int>(vec.size());

	for (int i = n / 2 - 1; i >= 0; --i)
		heapify(vec, n, i);

	for (int i = n - 1; i > 0; --i)
	{
		std::swap(vec[0], vec[i]);
		heapify(vec, i, 0);
	}
}

int partition(std::vector<int>& vec, int low, int high)
{
	int pivot = vec[high];
	int i = low - 1;

	for (int j = low; j < high; ++j)
	{
		if (vec[j] <= pivot)
		{
			++i;
			std::swap(vec[i], vec[j]);
		}
	}

	std::swap(vec[i + 1], vec[high]);
	return i + 1;
}

void quickSortHelper(std::vector<int>& vec, int low, int high)
{
	if (low < high)
	{
		int pi = partition(vec, low, high);
		quickSortHelper(vec, low, pi - 1);
		quickSortHelper(vec, pi + 1, high);
	}
}

void quickSort(std::vector<int>& vec)
{
	if (vec.empty())
		return;
	quickSortHelper(vec, 0, static_cast<int>(vec.size()) - 1);
}

std::vector<int> generateRandomVector(int n, int min = 0, int max = 1000)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(min, max);

	std::vector<int> vec(n);
	for (int& val : vec)
		val = dist(rng);

	return vec;
}

int main()
{
	constexpr int repetitions = 5;
	for (int n = 10000; n <= 100000; n += 10000)
	{
		double heapTotal = 0.0;
		double quickTotal = 0.0;

		for (int r = 0; r < repetitions; ++r)
		{
			auto data = generateRandomVector(n);
			auto vec1 = data;
			auto vec2 = data;

			auto start = std::chrono::high_resolution_clock::now();
			heapSort(vec1);
			auto end = std::chrono::high_resolution_clock::now();
			heapTotal += std::chrono::duration<double, std::milli>(end - start).count();

			start = std::chrono::high_resolution_clock::now();
			quickSort(vec2);
			end = std::chrono::high_resolution_clock::now();
			quickTotal += std::chrono::duration<double, std::milli>(end - start).count();
		}

		std::cout << std::setw(6) << n
		          << std::setw(8) << std::fixed << std::setprecision(3) << heapTotal / repetitions
		          << std::setw(8) << std::fixed << std::setprecision(3) << quickTotal / repetitions
		          << std::endl;
	}
}