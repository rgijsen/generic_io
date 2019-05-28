#pragma once

#include <iostream>
#include <vector>

class tokenizer
{
  std::string _delimiters;
  std::string _content;
  std::string _token;

  size_t _offset;

public:
  tokenizer(const std::string content, std::string delimiters)
      : _delimiters(delimiters), _content(content), _offset(0)
  {
  }

  std::string get_token()
  {
    return _token;
  }

  bool next_token()
  {
    auto i = _content.find_first_not_of(_delimiters, _offset);
    if (i == std::string::npos)
    {
      _offset = _content.length();
      return false;
    }

    auto j = _content.find_first_of(_delimiters, i);
    if (j == std::string::npos)
    {
      _token = _content.substr(i);
      _offset = _content.length();
    }
    else
    {
      _token = _content.substr(i, j - i);
      _offset = j;
    }

    return true;
  }

  std::vector<std::string> get_tokens()
  {
    std::vector<std::string> tokens;
    while (next_token())
      tokens.push_back(get_token());

    return tokens;
  }
};
