
#pragma once

class WBItem
{
    int _index;
    std::string _dict;
    std::string _title;
    std::string _honbun;
public:
    void convert(const MapChain& MapPack);
    void push(char* Line);
    void pull(std::string& Dict, std::string& Title, std::string& Honbun) const;
    int index() const;
};

class WBFile
{
    std::map<int, WBItem> _wbf;
public:
    WBFile();
    WBFile(const wchar_t* InFile);

    void convert(const MapChain& MapPack);

    void scan(const wchar_t* InFile);
    void print(const wchar_t* OutFile);
};
