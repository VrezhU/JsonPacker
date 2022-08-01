#ifndef JSONTLV_H
#define JSONTLV_H

#include <map>
#include <unordered_map>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

/**
 * @brief The EType enum used for define supported json types for JsonTLV
 */
enum EType {
    JSON_BOOL,
    JSON_INT,
    JSON_UNSIGNED_INT,
    JSON_STRING
};

/**
 * @brief The JsonTLV class used for encode/decode json/binary format files into binary/json
 */
class JsonTLV {

public:

    /** Create JsonTLV object
     * @brief Constructor for JsonTLV calss
     * @param indexJsonKeyMapping defines the start index of key mapping
     */
    JsonTLV(int indexJsonKeyMapping = 1);

    /**
     * @brief Decode tlv binary file into readable format and save into the txt file
     * @param tlvFilePath the path of TVL decoded binary file
     */
    void decodeTLVFile(const std::string& tlvFilePath);

    /**
     * @brief Encode json record file into tlv binary file.
     * Each json record is separated by a new line.
     * The json record file will only contain simple types, complex and nested data structures will not be present.
     * @param jsonFilePath the path of json record file
     * @param tlvFilePath the path of tlv binary file
     */
    void encodeJsonFile(const std::string& jsonFilePath, const std::string& tlvFilePath = "");

    /**
     * @brief Calculate the size of file
     * @param file the file which size will be calculated
     * @return size of file
     */
    unsigned int getFileSize(std::ifstream& file);

private:
    /**
     * @brief mJsonKeyMap the key map which contains mapped keys with the format: <string: int>
     */
    std::unordered_map<std::string, int> mJsonKeyMap;

    /**
     * @brief mTypeSizeMap the map which contains the size of json supported types
     */
    std::map<EType, unsigned int> mTypeSizeMap;

    /**
     * @brief mIndexJsonKeyMapping the start index of key mapping
     */
    unsigned int mIndexJsonKeyMapping;

    /**
     * @brief encode the value of json record into the binary tlv format
     * TLV file format:
     * if type is string                  : key | type | size | value
     * otherwise (bool, int, unsigned int) : key | type | value
     *
     * @param tlvFilePath the path of TLV file
     * @param jsonLine the line which contains json value
     * @param keyStr the key of json value
     * @param type the type of json value
     */
    void encodeJsonValue(const std::string& tlvFilePath, const json& jsonLine, const std::string &keyStr, json::value_t type);

    /**
     * @brief encode preserved json key map into TLV binary file
     * TLV file format: key | type | size | value
     *
     * @param tlvFilePath the path of TLV file
     */
    void encodeJsonKeyMap(const std::string& tlvFilePath);

    /**
     * @brief Generic function for writing json values intoTLV binary file
     * json value types should be: bool, int or unsigned int.
     * TLV file format: key | type | value
     *
     * @param value
     * @param file the TLV file
     * @param type the type of value
     */
    template<typename T>
    void writeToBinaryFile(T value, std::ofstream& file, EType type);

    /**
     * @brief writing json string values intoTLV binary file
     * json value types should be: bool, int or unsigned int
     * TLV file format: key | type | size | value
     *
     * @param value
     * @param file the TLV file
     */
    void writeStringToBinaryFile(const std::string &value, std::ofstream& file);

};

#endif // JSONTLV_H
