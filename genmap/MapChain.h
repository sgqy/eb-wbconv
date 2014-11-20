#pragma once

// ����ļ�����
class MapChain
{
    // <title, map_file>
    std::map<std::string, MapFile> _mmf;
public:
    MapChain();

    int get(std::string& Rslt, const std::string& Title, const std::string& Key);
    MapChain& operator += (const MapFile&);

    ~MapChain();
};

