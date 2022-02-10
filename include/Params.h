#ifndef PARAMS_H
#define PARAMS_H

#define DEFAULT_PERSON_FILE_PATH "/etc/trading_bot/person.json"
#define DEFAULT_PAIR_FILE_PATH "/etc/trading_bot/pair.json"
#define DEFAULT_LOG_FOLDER_PATH "/etc/trading_bot/log"

#define HISTORY false
#define HISTORY_START 1617448194000
#define HISTORY_END 1617448194000

#define DOWN1_INTERVAL "30m"
#define MAIN_INTERVAL "1h"
#define UP1_INTERVAL "2h"
#define UP2_INTERVAL "4h"
#define UP3_INTERVAL "6h"

/* EMA CROSS BUY */
#define EMA_SHORT_BUY 50
#define EMA_LONG_BUY 200

/* RSI */
#define RSI_LENGTH 14
#define RSI_MIN_BUY 50

/* ATR */
#define ATR_LENGTH 14

#endif // PARAMS_H
