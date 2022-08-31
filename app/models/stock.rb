class Stock < ApplicationRecord
    belongs_to :stock_list
    acts_as_list scope: :stock_list

    validates :symbol, presence: true
    validates :company_name, presence: true
    validates :exchange, presence: true

    attr_reader :quote_data

    def load_quote_data(stock_lookup_service)
        @quote_data = stock_lookup_service.quote_data(self[:symbol])
    end

    def as_json(options = {})
        super(options).merge({
          'price' => @quote_data[:price],
          'percent_change' => @quote_data[:percent_change]
        })
    end

end
