const Binance = require('node-binance-api');
const { Client } = require('pg');
const QueryStream = require('pg-query-stream');

const binance = new Binance().options({
    APIKEY: '',
    APISECRET: ''
});

const client = new Client({
    user: 'postgres',
    host: 'localhost',
    database: process.env.DATABASE_RESULT_HISTORY_NAME,
    password: process.env.DATABASE_PASSWORD,
    port: 5432
});

client.connect().finally(() => {
    let pairs = [];

    client.query('SELECT DISTINCT pair FROM transactions ORDER BY pair').then((res) => {
        for (let i = 0; i < res.rows.length; ++i) {
            pairs.push(res.rows[i].pair);
        }
    }).then(() => {
        run(pairs);
    });
});

async function run(pairs) {
    let totalList = [];

    for (let i = 0; i < pairs.length; ++i) {
        /*if (pairs[i] == 'NANOBTC')*/ {
            const query = new QueryStream(`SELECT * FROM transactions WHERE pair='${pairs[i]}' ORDER BY open_time`);
            const stream = client.query(query);
            let gainzList = [];
            let lastBuy = { data: {} };

            let promise = new Promise((resolve, reject) => {
                stream.on('data', (data) => {
                    computeGainz(data, gainzList, lastBuy);
                });
                stream.on('end', () => {
                    gainzList.sort((a, b) => {
                        return (a - b);
                    });
                    let total = 0;
                    for (let i = 0; i < gainzList.length; ++i) {
                        total += gainzList[i];
                        //console.log(gainzList[i]);
                    }
                    totalList.push(total);
                    console.log(pairs[i], 'total: ', total);
                    resolve();
                });
            });
            await promise;
            //console.log(`SELECT pair FROM transactions WHERE pair='${pairs[i]}'`);
        }
    }
    let total = 0;
    for (let i = 0; i < totalList.length; ++i) {
        total += totalList[i];
    }
    console.log('Grand total: ', total);
}

function computeGainz(data, gainzList, lastBuy) {
    if (data['type'] === 'SELL' && Object.keys(lastBuy.data).length !== 0) {
        let gainz = (data['close_price'] - lastBuy.data['close_price']) / lastBuy.data['close_price'] * 100;
        gainzList.push(gainz);
        //console.log(gainz, lastBuy.data['rsi']);
        lastBuy.data = {};
    }
    if (data['type'] === 'BUY') {
        lastBuy.data = data;
    }
}
