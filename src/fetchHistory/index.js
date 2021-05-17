const Binance = require('node-binance-api');
const mysql = require('mysql');

const START_TIME = 1575072000000;
const END_TIME = 1616931900000;
const INTERVAL = "1h";

const binance = new Binance().options({
    APIKEY: '',
    APISECRET: ''
});
const connection = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: process.env.HISTORY_DATABASE_PASSWORD,
    database: "history"
});

let pairs = [];

console.log("Connecting to database");
connection.connect(function (err) {
    if (err) throw err;
    fetchCoins();
});

function fetchCoins() {
    console.log("Fetching coins");
    binance.exchangeInfo((error, data) => {
        if (error) console.error(error);
        for (let symbol of data.symbols) {
            if (symbol.quoteAsset == 'BTC') {
                pairs.push(symbol.symbol);
            }
        }
        pairs.sort();
        /*let i = 0;
        for (i = 0; i < pairs.length && pairs[i] !== 'XVGBTC'; ++i);
        pairs.splice(0, i);*/
        fillDatabase();
    });
}

async function fillDatabase() {
    const now = Date.now();
    for (let i = 0; i < pairs.length; ++i) {
        console.log("Adding " + pairs[i]);
        await candlesticks(pairs[i], START_TIME, END_TIME);
    }
    connection.end(function (err) {
        if (err) throw err;
    });
}

async function candlesticks(pair, startTime, endTime) {
    let ticks = [];
    do {
        ticks = await binance.candlesticks(pair, INTERVAL, false, { limit: 1000, startTime: startTime, endTime: endTime });
        if (ticks.length != 0) {
            for (let i = 0; i < ticks.length; ++i) {
                let values = "'" + pair + "', '" + INTERVAL + "', " + ticks[i][0] + ", '" + ticks[i][1] + "', '" + ticks[i][2] + "', '" + ticks[i][3] + "', '" + ticks[i][4] + "', '" + ticks[i][5] + "', " + ticks[i][6] + ", '" + ticks[i][7] + "', " + ticks[i][8] + ", '" + ticks[i][9] + "', '" + ticks[i][10] + "'";
                let query = new Promise((resolve, reject) => {
                    connection.query("INSERT INTO candles (`pair`, `interval`, `open_time`, `open_price`, `high_price`, `low_price`, `close_price`, `volume`, `close_time`, `quote_volume`, `trade_count`, `taker_buy_volume`, `taker_buy_quote_volume`) VALUES (" + values + ");", (err, res) => {
                        if (err) {
                            return (reject(err.message));
                        }
                        resolve();
                    });
                });
                await query;
            }
            startTime = ticks[ticks.length - 1][6] + 1;
        }
    } while (ticks.length == 1000)
}
