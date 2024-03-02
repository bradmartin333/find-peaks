#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "PeakFinder.h"

std::vector<std::vector<float>> read_data_from_file(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "Error opening file: " << filepath << std::endl;
    return {};
  }

  std::string line;
  int num_channels = 0;

  while (std::getline(file, line))
  {
    // Skip empty lines and comment lines
    if (line.empty() || line.front() == '@')
    {
      if (line.find("ATTRIBUTE") != std::string::npos)
        num_channels++;
      if (line == "@DATA")
        break;
      continue;
    }

    if (num_channels == 0)
      throw std::invalid_argument("No channels found");
  }

  std::vector<std::vector<float>> data(num_channels);

  while (std::getline(file, line))
  {
    std::stringstream line_stream(line);
    std::vector<float> row;
    float value;

    // Split by comma using comma as a delimiter
    std::string token;
    int col = 0;
    while (std::getline(line_stream, token, ','))
    {
      // Convert token to float and add to row
      try
      {
        value = std::stof(token);
        data[col++].push_back(value);
      }
      catch (...)
      {
        throw std::invalid_argument("Error converting token to float");
      }
    }
  }

  return data;
}

int main()
{
  auto data = read_data_from_file("../data/EEG Eye State.txt");

  for (const std::vector<float> &row : data)
  {
    std::vector<int> out;

    PeakFinder::findPeaks(row, out, false);

    if (out.size() == 0)
    {
      std::cout << "No peaks" << std::endl;
      continue;
    }

    std::cout << "Maxima found:" << std::endl;

    for (size_t i = 0; i < out.size(); ++i)
      std::cout << row[out[i]] << " ";

    std::cout << std::endl;
  }

  return 0;
}
