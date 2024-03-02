#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "PeakFinder.h"

std::vector<std::vector<float>> read_data_from_file(const std::string &filepath, int &num_channels)
{
  std::ifstream file(filepath);
  if (!file.is_open())
  {
    std::cerr << "Error opening file: " << filepath << std::endl;
    return {};
  }

  std::string line;

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
  int num_channels = 0;
  auto data = read_data_from_file("../data/EEG Eye State.txt", num_channels);

  std::vector<int> eeg_peaks;

  for (int i = 0; i < num_channels; i++)
  {
    std::vector<float> row = data[i];
    std::vector<int> out;

    PeakFinder::findPeaks(row, out, false);

    if (i == num_channels - 1)
    {
      std::sort(eeg_peaks.begin(), eeg_peaks.end());
      std::vector<int>::iterator individual_peaks;
      individual_peaks = std::unique(eeg_peaks.begin(), eeg_peaks.end());
      eeg_peaks.resize(std::distance(eeg_peaks.begin(), individual_peaks));
      for (std::vector<int>::iterator it = eeg_peaks.begin(); it != eeg_peaks.end(); ++it)
        std::cout << *it << ' ';
      std::cout << out.size();
    }
    else
    {
      for (size_t i = 0; i < out.size(); ++i)
        eeg_peaks.push_back(out[i]);
    }
  }

  return 0;
}
