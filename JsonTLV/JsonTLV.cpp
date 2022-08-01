#include "JsonTLV.h"

#include <iostream>
#include <fstream>
#include <cstdio>

JsonTLV::JsonTLV(int indexJsonKeyMapping)
    : mIndexJsonKeyMapping(indexJsonKeyMapping)
{
    mTypeSizeMap[EType::JSON_BOOL] = sizeof(bool);
    mTypeSizeMap[EType::JSON_INT] = sizeof(int);
    mTypeSizeMap[EType::JSON_UNSIGNED_INT] = sizeof(unsigned int);
    mTypeSizeMap[EType::JSON_STRING] = 0;
}

unsigned int JsonTLV::getFileSize(std::ifstream& file) {
    const int begin = file.tellg();
    file.seekg(0, std::ios::end);
    const unsigned int end = file.tellg();
    const unsigned int fileSize = (end-begin);
    file.seekg(0, std::ios::beg);
    return fileSize;
}

void JsonTLV::decodeTLVFile(const std::string& tlvFilePath) {
    std::ifstream tlvFile(tlvFilePath, std::ios::in | std::ios::binary);
    if (!tlvFile) {
       std::cout << "decodeTVLFile: " << tlvFilePath
                 << ": Cannot open binary file !!!" << std::endl;
       return;
    }

    std::ofstream outputFile(tlvFilePath + ".txt", std::ios::out | std::ofstream::trunc);
    if (!outputFile) {
       std::cout << "decodeTVLFile:" << tlvFilePath + ".txt"
                 << ": Cannot open txt file!" << std::endl;
       return;
    }

    const int fileSize = getFileSize(tlvFile);

    assert(fileSize != 0 && "TLV File is empty !!!");

    int currentPos = 0;

    while(currentPos < fileSize) {
        char keyChar[mTypeSizeMap[EType::JSON_UNSIGNED_INT]];
        tlvFile.read(keyChar, mTypeSizeMap[EType::JSON_UNSIGNED_INT]);
        currentPos += mTypeSizeMap[EType::JSON_UNSIGNED_INT];
        outputFile << *((unsigned int*)(keyChar)) << " : ";

        char typeChar;
        tlvFile.read(&typeChar, 1);
        currentPos += 1;

        switch ((EType)(typeChar)) {
        case EType::JSON_BOOL: {
            char value;
            tlvFile.read(&value, 1);
            currentPos += 1;
            outputFile << (bool)(value) << ", ";
            break;
        }
        case EType::JSON_UNSIGNED_INT: {
            char value[mTypeSizeMap[EType::JSON_UNSIGNED_INT]];
            tlvFile.read(value, mTypeSizeMap[EType::JSON_UNSIGNED_INT]);
            currentPos += mTypeSizeMap[EType::JSON_UNSIGNED_INT];
            outputFile << *((unsigned int*)(value)) << ", ";
            break;
        }
        case EType::JSON_INT: {
            char value[mTypeSizeMap[EType::JSON_INT]];
            tlvFile.read(value, mTypeSizeMap[EType::JSON_INT]);
            currentPos += mTypeSizeMap[EType::JSON_INT];
            outputFile << *((int*)(value)) << ", ";
            break;
        }
        case EType::JSON_STRING: {
            char sizeChar[mTypeSizeMap[EType::JSON_UNSIGNED_INT]];
            tlvFile.read(sizeChar, mTypeSizeMap[EType::JSON_UNSIGNED_INT]);
            currentPos += mTypeSizeMap[EType::JSON_UNSIGNED_INT];

            int size = *((unsigned int*)(sizeChar));
            char value[size];
            tlvFile.read(value, size);
            currentPos += size;
            value[size] = '\0';
            outputFile << value << ", ";
            break;
        }
        default:
            std::cout << "decodeTVLFile: " << (EType)(typeChar)
                      << ": Unsupported Json type !!!" << std::endl;
            break;
        }
    }
    outputFile.close();
    tlvFile.close();
}

void JsonTLV::writeStringToBinaryFile(const std::string &value, std::ofstream& file) {
    char type = EType::JSON_STRING;
    file.write((char *)(&type), 1);
    const unsigned int length = value.length();
    file.write((char *)(&length), mTypeSizeMap[EType::JSON_UNSIGNED_INT]);
    file.write(value.c_str(), length);
}

template<typename T>
void JsonTLV::writeToBinaryFile(T value, std::ofstream& file, EType type) {
    file.write((char *)(&type), 1);
    file.write((char *)(&value),mTypeSizeMap[type]);
}

void JsonTLV::encodeJsonKeyMap(const std::string& tlvFilePath) {
    std::ofstream tlvFile(tlvFilePath, std::ios::out | std::ios::binary | std::ios_base::app);

    if (!tlvFile) {
       std::cout << "encodeJsonKeyMap:" << tlvFilePath
                 << ": Can not open binary file !!!" << std::endl;
       return;
    }

    for (auto& el: mJsonKeyMap) {
        tlvFile.write((char *)(&el.second),  mTypeSizeMap[EType::JSON_UNSIGNED_INT]);
        writeStringToBinaryFile(el.first, tlvFile);
    }
    tlvFile.close();
}

void JsonTLV::encodeJsonValue(const std::string& tlvFilePath,
                             const json& jsonLine,
                             const std::string &keyStr,
                             json::value_t type) {
    std::ofstream tlvFile(tlvFilePath, std::ios::out | std::ios::binary | std::ios_base::app);

    if (!tlvFile) {
       std::cout << "encodeJsonLine:" << tlvFilePath
                 << ": Can not open binary file!" << std::endl;
       return;
    }

    mJsonKeyMap[keyStr] = mIndexJsonKeyMapping++;
    tlvFile.write((char *)(&mJsonKeyMap[keyStr]),  mTypeSizeMap[EType::JSON_UNSIGNED_INT]);

    switch (type) {
    case json::value_t::boolean:
        writeToBinaryFile((bool)jsonLine[keyStr],
                          tlvFile, EType::JSON_BOOL);
        break;
    case json::value_t::number_unsigned:
        writeToBinaryFile((unsigned int)jsonLine[keyStr],
                          tlvFile, EType::JSON_UNSIGNED_INT);
        break;
    case json::value_t::number_integer:
        writeToBinaryFile((int)jsonLine[keyStr],
                          tlvFile, EType::JSON_INT);
        break;
    case json::value_t::string:
        writeStringToBinaryFile((std::string)jsonLine[keyStr],
                                tlvFile);
        break;
    default:
        std::cout << "encodeJsonLine: Unsupported Json type !!!" << std::endl;
        break;
    }
    tlvFile.close();

}

void JsonTLV::encodeJsonFile(const std::string& jsonFilePath, const std::string& tlvFilePath) {
    std::ifstream jsonFile;
    jsonFile.open(jsonFilePath, std::ios::in);

    if (jsonFile.is_open()) {
        assert(getFileSize(jsonFile) != 0 && "Json File is empty!!!");
        std::string line;
        std::string tlvFilePathUpdated = tlvFilePath;

        if (tlvFilePath.empty()) {
            tlvFilePathUpdated = jsonFilePath;
            tlvFilePathUpdated.replace(tlvFilePathUpdated.size() - 4,
                                       tlvFilePathUpdated.size() - 1, ".tlv");
        }

        std::remove(tlvFilePathUpdated.c_str());

        std::string keyTlvFilePath = tlvFilePathUpdated;
        keyTlvFilePath.replace(keyTlvFilePath.size() - 4,
                               keyTlvFilePath.size() - 1, "_key.tlv");
        std::remove(keyTlvFilePath.c_str());

        while (getline(jsonFile, line)) {
            json jsonLine = json::parse(line);
            for (auto& el: jsonLine.items())
                encodeJsonValue(tlvFilePathUpdated, jsonLine, el.key(), el.value().type());
        }

        encodeJsonKeyMap(keyTlvFilePath);

        jsonFile.close();
        return;

    }
    std::cout << "encodeJsonFile: " << jsonFilePath
              << ": Could not open json record file!" << std::endl;
}
