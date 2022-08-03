# JsonPacker
## Json encoder

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

JsonPacker is a sample encoder for json records which compresses stream to disk in the TLV binary format.
Functionality:
- Encoder Json record file
- Decoder TLV binary file

## Build Dependences

- [cmake](https://trendoceans.com/how-to-install-cmake-on-debian-10-11/) minimum version 3.5
- [nlohmann_json](https://github.com/nlohmann/json) minimum version 3.2.0

## Build Steps
- Setup build dependences
- clone JsonPacker from current repositorys
- run the following:
```sh
cd JsonPacker
mkdir build
cd build
cmake ..
make
```
## Usage
After build run the following:
```sh
./JsonPacker <Json record file path(reuired)> <TLV file path(optional)>
```
If you do not pass second argumnet(TLV file path) it will create `<json file name>.tlv` binary file in the json record file directory. And will create `<json file name>_key.tlv` binary file for the key map which contains mapped keys (string: int).

## Requirements
Json record file will contain records in JSON format. Each record is separated by a new line (LF).
The file will only contain simple types, complex and nested data structures will not be present (i.e.map and array).
- ex. json_record.txt
> { "key2":"value", "key2":42, "key3":true}
> {"sadsf":"dsewtew", "dsre":3221, "sdfds":"dsfewew"}

## TLV binary File format
> if value type is string                     : key | type | size | value
> otherwise (bool, int, unsigned int)         : key | type | value
