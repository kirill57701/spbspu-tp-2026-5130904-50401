#include "comms.hpp"

void knowbase::reg_comm(std::iostream& in, std::ostream& ou)
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
      if (!note(in, ou))
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
