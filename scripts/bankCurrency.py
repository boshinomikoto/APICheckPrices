import requests
import json

url = "https://v6.exchangerate-api.com/v6/940e5e4d460e2506ea275188/latest/USD"

response = requests.get(url)
data = response.json()


base_currency = data.get("base_code")

conversion_rates = data.get("conversion_rates")

result = {}
for currency, rate in conversion_rates.items():
    if currency in ("UAH", "EUR", "PLN", "TRY", "AED", "JPY"):
        result[currency] = {
            "bank": base_currency,
            "value": rate
        }
with open("D:/APIBINANCE/apiBinanceBot/apiBinanceBot/scripts/getCurrency.json", "w", encoding="utf-8") as f:
    json.dump(result, f, indent=4, ensure_ascii=False)



