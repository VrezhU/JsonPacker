#include <iostream>
#include <string>

#include "JsonTLV/JsonTLV.h"

/**
 * @brief main the start pint
 * @param argc count of arguments passed to app
 * @param argv arguments list passed to app. Need json(required) and TLV(optional) files in argument list.
 * @return
 */
int main(int argc, char** argv)
{
    std::string jsonFilePath = "";
    std::string tlvFilePath = "";

    try {
        if (argc == 1) {
            throw std::logic_error("Incorrect count of input arguments !!!");
        } else if (argc == 2) {
            jsonFilePath = argv[1];
            tlvFilePath = argv[1];
            tlvFilePath.replace(tlvFilePath.size() - 4, tlvFilePath.size() - 1, ".tlv");
        } else if (argc == 3) {
            jsonFilePath = argv[1];
            tlvFilePath = argv[2];
        }
    } catch(const std::exception &ex) {
        std::cout << ex.what() << std::endl;
        return 0;
    }
    std::cout << "json(input) file: " << jsonFilePath << std::endl;
    std::cout << "TLV(output) file: " << tlvFilePath << std::endl;

    JsonTLV jsonTLV;
    jsonTLV.encodeJsonFile(jsonFilePath, tlvFilePath);
    jsonTLV.decodeTLVFile(tlvFilePath);

    std::string keyTlvFilePath = jsonFilePath;
    keyTlvFilePath.replace(keyTlvFilePath.size() - 4, keyTlvFilePath.size() - 1, "_key.tlv");
    std::cout << "TLV KeyMap(output) file: " << keyTlvFilePath << std::endl;
    jsonTLV.decodeTLVFile(keyTlvFilePath);

    return 0;
}
