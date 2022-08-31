class StockList < ApplicationRecord
    has_many :stocks, -> { order("position ASC") }
end
