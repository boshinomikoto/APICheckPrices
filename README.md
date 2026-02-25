## Execution Flow

1. The C++ application acts as the main controller.
2. It executes Python scripts responsible for external API communication.
3. The Python layer performs HTTP requests to:
   * Binance REST API (cryptocurrency prices)
   * Bank API (currency exchange rates)
4. Retrieved data is serialized into JSON files.
5. The C++ layer parses the generated JSON files using nlohmann::json.
6. The application formats and displays:
   * Real-time cryptocurrency prices
   * 24h price change (absolute and percentage)
   * Bank exchange rates
7. A scheduler component ensures that:
   * Cryptocurrency data is refreshed continuously.
   * Bank exchange rates are updated every 2 hours to respect rate limit


## GCC
```
g++ -std=c++17 -I"D:\APIBINANCE\apiBinanceBot\apiBinanceBot\include" - way to include
-I"C:\Users\Asus TUF\AppData\Local\Programs\Python\Python312\include" - way to Python312/include
-L"C:\Users\Asus TUF\AppData\Local\Programs\Python\Python312\libs" - way to Python312/libs 
"D:\APIBINANCE\apiBinanceBot\apiBinanceBot\src\Source.cpp" -lpython312 -o
"D:\APIBINANCE\apiBinanceBot\apiBinanceBot\apiBinanceBot.exe"

./apiBinanceBot
```

![Program](imag/164135.png)

