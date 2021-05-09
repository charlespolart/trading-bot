# **Trading bot**

Trading bot is using binapi : https://github.com/niXman/binapi<br>
<br>**Dependencies :**
```console
~$ sudo apt install libboost-dev
~$ sudo apt install libboost-all-dev
~$ sudo apt install libssl-dev
~$ sudo apt install rapidjson-dev
~$
~$ wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -
~$ echo "deb [arch=amd64] http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" | sudo tee /etc/apt/sources.list.d/pgdg.list
~$ sudo apt update
~$ sudo apt install libpq-dev
~$ sudo apt install postgresql
~$ git clone https://github.com/jtv/libpqxx
~$ cd libpqxx
~$ git checkout 7.4.1
~$ ./configure --disable-documentation
~$ make
~$ sudo make install
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
<br>**Dump results database :**<br>
```console
~$ sudo -Hiu postgres pg_dump -h localhost $DATABASE_RESULT_NAME > $DATABASE_RESULT_NAME"_dump"
```
<br>**Restore results database :**<br>
```console
~$ sudo -Hiu postgres psql -h localhost $DATABASE_RESULT_NAME < $DATABASE_RESULT_NAME"_dump"
```
