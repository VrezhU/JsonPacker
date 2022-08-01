#ifndef JSONTLV_H
#define JSONTLV_H

#include <map>
#include <unordered_map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

enum EType {
    JSON_BOOL,
    JSON_INT,
    JSON_UNSIGNED_INT,
    JSON_STRING
};

class JsonTLV {

public:
    JsonTLV(int indexJsonKeyMapping = 1);

    void decodeTVLFile(const std::string& tlvFilePath);
    void encodeJsonFile(const std::string& jsonFilePath, const std::string& tlvFilePath = "");
    unsigned int getFileSize(std::ifstream& file);

private:
    std::unordered_map<std::string, int> mJsonKeyMap;
    std::map<EType, unsigned int> mTypeSizeMap;
    unsigned int mIndexJsonKeyMapping;

    void encodeJsonLine(const std::string& tlvFilePath, const json& jsonLine, const std::string &keyStr, json::value_t type);
    void encodeJsonKeyMap(const std::string& tlvFilePath);
    template<typename T>
    void writeToBinaryFile(T value, std::ofstream& file, EType type);
    void writeStringToBinaryFile(const std::string &value, std::ofstream& file);
};

#endif // JSONTLV_H
