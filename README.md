# **Trading bot**

Trading bot is using binapi : https://github.com/niXman/binapi<br>
<br>**Dependencies :**
```console
~$ sudo apt install libboost-dev
~$ sudo apt install libboost-all-dev
~$ sudo apt install libssl-dev
~$ sudo apt install rapidjson-dev
~$
~$ git clone https://github.com/mysql/mysql-connector-cpp.git
~$ cd mysql-connector-cpp
~$ git checkout 8.0
~$ cmake . -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release
~$ sudo cmake --build . --target install
~$ sudo mv /usr/include/mysql-cppconn-8/* /usr/include && sudo rm -rf /usr/include/mysql-cppconn-8
```

<br>**Build steps :**
```console
~$ git clone git@github.com:charlespolart/trading-bot.git
~$ cd trading-bot
~$ cmake .
~$ make
~$ ./build/trading-bot
```
<br>**Users definition :**<br>
The default path is `/etc/trading-bot/person.json`<br>
It can be overwrited by setting the environment variable ***"PERSON_FILE_TRADING_BOT=[path]"***
```json
{
    "person": [
        {
            "name": "",
            "apiKey": "API_KEY",
            "apiSecret": "API_SECRET",
            "btcAmount": "0.0",
            "enable": true
        },
        {
            "name": "",
            "apiKey": "API_KEY",
            "apiSecret": "API_SECRET",
            "btcAmount": "0.0",
            "enable": true
        }
    ]
}
```
