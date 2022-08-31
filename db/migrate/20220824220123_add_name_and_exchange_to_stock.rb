class AddNameAndExchangeToStock < ActiveRecord::Migration[7.0]
  def change
    add_column :stocks, :Stock, :string
    add_column :stocks, :company_name, :string
    add_column :stocks, :exchange, :string
  end
end
