#include "comms.hpp"
#include <stdexcept>
#include <iomanip>

void petrov::knowbase::reg_comm(std::istream& in, std::ostream& ou)
{
  std::string s;
  while (in >> s)
  {
    if (s == "note")
    {
      if (!note(in))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "line")
    {
      if (!line(in))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "show")
    {
      if (!show(in, ou))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "drop")
    {
      if (!drop(in))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "link")
    {
      if (!link(in))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "halt")
    {
      if (!halt(in))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "mind")
    {
      if (!mind(in, ou))
      {
       ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "expired")
    {
      if (!expired(in, ou))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else if (s == "refresh")
    {
      if (!refresh(in))
      {
        ou << "<INVALID COMMAND>\n";
      }
    }
    else
    {
      ou << "<INVALID COMMAND>\n";
    }
  }
}

bool petrov::knowbase::note(std::istream& in)
{
  std::string s;
  in >> s;
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
  in >> s1 >> std::quoted(s2);
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it = storage.find(s1);
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
  in >> s1;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator lineee = storage.find(s1);
  if (lineee == storage.end())
  {
    return 0;
  }
  for (size_t i = 0; i < lineee->second->lines.size(); ++i)
  {
    if (i != lineee->second->lines.size() - 1)
    {
      ou << lineee->second->lines[i] << '\n';
    }
    else
    {
      ou << lineee->second->lines[i];
    }
  }
  ou << '\n';
  return 1;
}

bool petrov::knowbase::drop(std::istream& in)
{
  std::string s;
  in >> s;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it = storage.find(s);
  if (it == storage.end())
  {
    return 0;
  }
  storage.erase(s);
  return 1;
}

bool petrov::knowbase::link(std::istream& in)
{
  std::string s1, s2;
  in >> s1 >> s2;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it1 = storage.find(s1);
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it2 = storage.find(s2);
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
  in >> s1 >> s2;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it1 = storage.find(s1);
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it2 = storage.find(s2);
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
  in >> s;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it = storage.find(s);
  if (it == storage.end())
  {
    return 0;
  }
  for (size_t i = 0; i < it->second->txt.size(); ++i)
  {
    std::shared_ptr<mem> ptr = it->second->txt[i].lock();
    if (ptr)
    {
      ou << ptr->name << '\n';
    }
  }
  return 1;
}

bool petrov::knowbase::expired(std::istream& in, std::ostream& ou)
{
  std::string s;
  in >> s;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it = storage.find(s);
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
  in >> s;
  std::unordered_map<std::string, std::shared_ptr<mem>>::iterator it = storage.find(s);
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
