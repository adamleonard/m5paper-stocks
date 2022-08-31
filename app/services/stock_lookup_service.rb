class StockLookupService

    def initialize
        FinnhubRuby.configure do |config|
            config.api_key['api_key'] = ENV.fetch('FINHUB_API_KEY')
        end
        @finnhub_client = FinnhubRuby::DefaultApi.new
    end

    def company_profile(symbol)
        @finnhub_client.company_profile2({symbol: symbol})
    end

    def quote_data(symbol)
        data = @finnhub_client.quote(symbol)
        {'price': data.c, 'percent_change': data.dp}
    end

end


          
