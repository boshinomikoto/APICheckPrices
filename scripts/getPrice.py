# getPrice.py
import requests
import json
import os

url = "https://api.binance.com/api/v3/ticker/24hr"
symbols = {
    "BTC": "BTCUSDT",
    "ETH": "ETHUSDT",
    "BNB": "BNBUSDT",
    "TON": "TONUSDT",
    "XRP": "XRPUSDT",
    "SOL": "SOLUSDT",
    "DOGE": "DOGEUSDT",
    "TRUMP": "TRUMPUSDT",
    "ADA": "ADAUSDT",
    "LTC": "LTCUSDT",
    "AVAX": "AVAXUSDT",
    "LINK": "LINKUSDT",
    "UNI": "UNIUSDT",
    "DOT": "DOTUSDT",
    "HBAR": "HBARUSDT",
    "XLM": "XLMUSDT",
    "SUI": "SUIUSDT",
    "PEPE": "PEPEUSDT",
    "BONK": "BONKUSDT"
}
result = {}

for short_name, full_symbol in symbols.items():
    try:
        response = requests.get(url, params={"symbol": full_symbol})
        if response.status_code == 200:
            data = response.json()
            price = data["lastPrice"]
            change = data["priceChange"]
            change_percent = data["priceChangePercent"]
            result[short_name] = [{
                "symbol": "USDT",
                "price": price,
                "change": change,
                "changePercent": change_percent
            }]
        else:
            print(f"Ошибка запроса для {full_symbol}: {response.status_code}")
    except Exception as e:
        print(f"Ошибка при запросе {full_symbol}: {e}")

with open("D:/APIBINANCE/apiBinanceBot/apiBinanceBot/scripts/getPrice.json", "w", encoding="utf-8") as f:
    json.dump(result, f, indent=4)
