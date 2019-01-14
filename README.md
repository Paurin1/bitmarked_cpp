# Bitmarked C++

The repository contains C++ files with classes and methods capable of handling Bitmarket API's requests.

The project is based on Bitmarket API documentation https://www.bitmarket.net/ .

Public API documentation is available here: https://www.bitmarket.net/docs.php?file=api_public.html

Private API can be reached here: https://github.com/bitmarket-net/api

# Remember! Get familiar with Bitmarket API's official documentation as it provides information about limitations! Breaking rules may result in closing your account.

## Usage
- *include* internal directory structure is crucial
- include in your project **BitmarketPublic.h** or **BitmarketPrivate.h** (depending on your needs)
- add **PythonNet.cpp**, **BitmarketPublic.cpp** and **BitmarketPrivate.cpp** into your project's makefile
- in **PythonNet.cpp** file set value of *python_path* to fit your system settings
- compile your project with at least C++11
```cpp
#include <iostream>
#include "BitmarketPublic.h"

void checkRate()
{
    BitmarketPublic bitPub;
    auto ret = bitPub.ticker();
    
    std::cout << "Last rate: " << ret->last;
}
```
```cpp
#include <iostream>
#include "BitmarketPrivate.h"

void checkHistory()
{
    BitmarketPrivate bitPrv;
    bpp.key_private = "xxx";
    bpp.key_public = "xxx";
    auto ret = *bitPrv.history("EUR", 1000, 0);
    
    std::cout << "success: " << ret["success"].get<bool>() <<std::endl;
    std::cout << "limit:"
              << "\n\tused: " << ret["limit"]["used"].get<int>()
              << "\n\tallowed: " << ret["limit"]["allowed"].get<int>()
              << "\n\texpires: " << ret["limit"]["expires"].get<int>()
              << std::endl;

    std::cout << "data:"
              << "\n\ttotal: " << ret["data"]["total"].get<int>()
              << "\n\tcount: " << ret["data"]["count"].get<int>()
              << "\n\tresults: " << std::endl;

    for (auto el : ret["data"]["results"].array())
    {
      std::cout << "#" << el["id"].get<int>()
                << "\tamount=" << el["amount"].get<int>()
                << "\trate=" << el["amount"].get<int>()
                << "\tcommission=" << el["commission"].get<int>()
                << std::endl;
    }
}
```

## Project structure

In *include* directory:
  - PythonNet.h
  - PythonNet.cpp
  - BitmarketPublic.h
  - BitmarketPublic.cpp
  - BitmarketPrivate.h
  - BitmarketPrivate.cpp
  - PublicApiDataStructures.h
  
*PythonNet* is a temporary solution for handling HTTPS connection. It allows this project to be compiled and run on Windows as well as on Linux using the same source code.

*BitmarketPublic* is a class containing methods to handle public Bitmarket API. In the near future it will be propably rewritten to return data in *BitmarketPrivate* style.

*BitmarketPrivate* is a class with methods to handle private Bitmarket API. Every method returns data stored in a **nlohmann::json** class.

*PublicApiDataStructures* contains definitions of structs that represent data returned by public API. It's going to be removed in the near future.

More detailed descriptions are available in comments included in each file and in Bitmarket API documentation.

## Third party tools:
- [Nlohmann's JSON for Modern C++](https://github.com/nlohmann/json) to parse response from the API
- [Part of Bitcoin Core](https://github.com/bitcoin/bitcoin) to generate HMAC SHA512, some files were modified to fit in
