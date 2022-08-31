class AddListToStock < ActiveRecord::Migration[7.0]
  def change
    add_column :stocks, :StockList, :string
    add_reference :stocks, :stock_list, null: true, foreign_key: true
  end
end
