#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class metaData
{
private:
    string name;
    long long size;
    string type;
    string createdAt;
    string modifiedAt;

public:
    metaData(string n, long long s, string t, string c, string m)
      : name(n), size(s), type(t), createdAt(c), modifiedAt(m) {}
};

class virtualFile
{
private:
    string name;
    string content;
    metaData info;
public:
    virtualFile(string name, string cont, string metaName, long long s, string t, string c, string m)
       : name(name), content(cont), info(metaName, s, t, c, m) {}
};

#endif