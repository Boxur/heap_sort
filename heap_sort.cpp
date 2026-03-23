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

void merge(std::vector<int>& vec, int left, int mid, int right)
{
	int n1 = mid - left + 1;
	int n2 = right - mid;

	std::vector<int> leftVec(n1);
	std::vector<int> rightVec(n2);

	for (int i = 0; i < n1; ++i)
		leftVec[i] = vec[left + i];
	for (int j = 0; j < n2; ++j)
		rightVec[j] = vec[mid + 1 + j];

	int i = 0, j = 0, k = left;

	while (i < n1 && j < n2)
	{
		if (leftVec[i] <= rightVec[j])
			vec[k++] = leftVec[i++];
		else
			vec[k++] = rightVec[j++];
	}

	while (i < n1)
		vec[k++] = leftVec[i++];

	while (j < n2)
		vec[k++] = rightVec[j++];
}

void mergeSortHelper(std::vector<int>& vec, int left, int right)
{
	if (left < right)
	{
		int mid = left + (right - left) / 2;
		mergeSortHelper(vec, left, mid);
		mergeSortHelper(vec, mid + 1, right);
		merge(vec, left, mid, right);
	}
}

void mergeSort(std::vector<int>& vec)
{
	if (vec.empty())
		return;
	mergeSortHelper(vec, 0, static_cast<int>(vec.size()) - 1);
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
	for (int n = 10000; n <= 1000000; n += 10000)
	{
		double heapTotal = 0.0;
		double mergeTotal = 0.0;

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
			mergeSort(vec2);
			end = std::chrono::high_resolution_clock::now();
			mergeTotal += std::chrono::duration<double, std::milli>(end - start).count();
		}

		std::cout << std::setw(6) << n
		          << std::setw(8) << std::fixed << std::setprecision(3) << heapTotal / repetitions
		          << std::setw(8) << std::fixed << std::setprecision(3) << mergeTotal / repetitions
		          << std::endl;
	}
}