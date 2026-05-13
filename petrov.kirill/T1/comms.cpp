#include "comms.hpp"
#include <stdexcept>
#include <iomanip>

void petrov::knowbase::reg_comm(std::istream& in, std::ostream& ou)
{
  std::string s;
  while (in >> s)
  {
    bool res = 1;
    if (s == "note")
    {
      res = note(in);
    }
    else if (s == "line")
    {
      res = line(in);
    }
    else if (s == "show")
    {
      res = show(in, ou);
    }
    else if (s == "drop")
    {
      res = drop(in);
    }
    else if (s == "link")
    {
      res = link(in);
    }
    else if (s == "halt")
    {
      res = halt(in);
    }
    else if (s == "mind")
    {
      res = mind(in, ou);
    }
    else if (s == "expired")
    {
      res = expired(in, ou);
    }
    else if (s == "refresh")
    {
      res = refresh(in);
    }
    else
    {
      continue;
    }

    if (!res)
    {
      ou << "<INVALID COMMAND>\n";
    }
  }
}

bool petrov::knowbase::note(std::istream& in)
{
  std::string s;
  if (!(in >> s))
  {
    return 0;
  }
  if (storage.find(s) != storage.end())
  {
    return 0;
  }
  storage[s] = std::make_shared<mem>(s);
  return 1;
}

bool petrov::knowbase::line(std::istream& in)
{
  std::string s1, s2;
  if (!(in >> s1 >> std::quoted(s2)))
  {
    return 0;
  }
  auto it = storage.find(s1);
  if (it == storage.end())
  {
    return 0;
  }
  it->second->lines.push_back(s2);
  return 1;
}

bool petrov::knowbase::show(std::istream& in, std::ostream& ou)
{
  std::string s1;
  if (!(in >> s1))
  {
    return 0;
  }
  auto it = storage.find(s1);
  if (it == storage.end())
  {
    return 0;
  }
  if (it->second->lines.empty())
  {
    ou << '\n';
  }
  for (size_t i = 0; i < it->second->lines.size(); ++i)
  {
    ou << it->second->lines[i] << '\n';
  }
  return 1;
}

bool petrov::knowbase::drop(std::istream& in)
{
  std::string s;
  if (!(in >> s))
  {
    return 0;
  }
  auto it = storage.find(s);
  if (it == storage.end())
  {
    return 0;
  }
  storage.erase(it);
  return 1;
}

bool petrov::knowbase::link(std::istream& in)
{
  std::string s1, s2;
  if (!(in >> s1 >> s2))
  {
    return 0;
  }
  auto it1 = storage.find(s1);
  auto it2 = storage.find(s2);
  if (it1 == storage.end() || it2 == storage.end())
  {
    return 0;
  }
  for (size_t i = 0; i < it1->second->txt.size(); ++i)
  {
    if (it1->second->txt[i].lock() == it2->second)
    {
      return 0;
    }
  }
  it1->second->txt.push_back(it2->second);
  return 1;
}

bool petrov::knowbase::halt(std::istream& in)
{
  std::string s1, s2;
  if (!(in >> s1 >> s2))
  {
    return 0;
  }
  auto it1 = storage.find(s1);
  auto it2 = storage.find(s2);
  if (it1 == storage.end() || it2 == storage.end())
  {
    return 0;
  }
  for (size_t i = 0; i < it1->second->txt.size(); ++i)
  {
    if (it1->second->txt[i].lock() == it2->second)
    {
      it1->second->txt.erase(it1->second->txt.begin() + i);
      return 1;
    }
  }
  return 0;
}

bool petrov::knowbase::mind(std::istream& in, std::ostream& ou)
{
  std::string s;
  if (!(in >> s))
  {
    return 0;
  }
  auto it = storage.find(s);
  if (it == storage.end())
  {
    return 0;
  }

  bool has_valid = false;
  for (size_t i = 0; i < it->second->txt.size(); ++i)
  {
    if (auto ptr = it->second->txt[i].lock())
    {
      ou << ptr->name << '\n';
      has_valid = true;
    }
  }
  if (!has_valid)
  {
    ou << '\n';
  }
  return 1;
}

bool petrov::knowbase::expired(std::istream& in, std::ostream& ou)
{
  std::string s;
  if (!(in >> s))
  {
    return 0;
  }
  auto it = storage.find(s);
  if (it == storage.end())
  {
    return 0;
  }
  size_t count = 0;
  for (size_t i = 0; i < it->second->txt.size(); ++i)
  {
    if (it->second->txt[i].expired())
    {
      count++;
    }
  }
  ou << count << '\n';
  return 1;
}

bool petrov::knowbase::refresh(std::istream& in)
{
  std::string s;
  if (!(in >> s))
  {
    return 0;
  }
  auto it = storage.find(s);
  if (it == storage.end())
  {
    return 0;
  }
  std::vector<std::weak_ptr<mem>> clean;
  for (size_t i = 0; i < it->second->txt.size(); ++i)
  {
    if (!it->second->txt[i].expired())
    {
      clean.push_back(it->second->txt[i]);
    }
  }
  it->second->txt = clean;
  return 1;
}
