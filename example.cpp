#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "PeakFinder.h"

std::vector<std::vector<float>> read_data_from_file(const std::string& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filepath << std::endl;
    return {};
  }

  std::vector<std::vector<float>> data;
  std::string line;
  while (std::getline(file, line)) {
    // Skip empty lines and comment lines
    if (line.empty() || line.front() == '@') {
      continue;
    }

    std::stringstream line_stream(line);
    std::vector<float> row;
    float value;

    // Split by comma using comma as a delimiter
    std::string token;
    while (std::getline(line_stream, token, ',')) {
      // Convert token to float and add to row
      try {
        value = std::stof(token);
      } catch (...) {
        throw std::invalid_argument("Error converting token to float");
      }
      row.push_back(value);
    }

    // Check for 15 values and throw exception if needed
    if (row.size() != 15) {
      throw std::invalid_argument("Line has unexpected number of values (expected 15)");
    }

    data.push_back(row);
  }

  return data;
}


int main()
{
	auto data = read_data_from_file("../data/EEG Eye State.txt");

	float inArr[14] = {0, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 7};

	std::vector<float> in(inArr, inArr + sizeof(inArr) / sizeof(float));
	std::vector<int> out;

	PeakFinder::findPeaks(in, out, false);

	if (out.size() == 0)
	{
		std::cout << "No peaks" << std::endl;
		return 0;
	}

	std::cout << "Maxima found:" << std::endl;

	for (size_t i = 0; i < out.size(); ++i)
		std::cout << in[out[i]] << " ";

	std::cout << std::endl;

	return 0;
}
