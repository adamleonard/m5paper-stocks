class StocksController < ApplicationController
  def index
    stock_lookup_service = StockLookupService.new
    
    @stocks = stock_list.stocks
    
    @stocks.map { |stock|
      Thread.new do
        stock.load_quote_data(stock_lookup_service)
      end
    }.each(&:join)

    lookup_time = Time.new
    
    respond_to do |format|
      last_updated_string = "Last updated at #{lookup_time.strftime("%I:%M %p (%b %d)")}"
      format.json { render json: { stocks: @stocks, last_updated: last_updated_string } }
      format.html
    end
  end
  
  def new
    @stock = Stock.new
  end
  
  def create
    stock_lookup_service = StockLookupService.new
    
    @stock = Stock.new(stock_params)
    @stock.stock_list = stock_list
    
    company_profile = stock_lookup_service.company_profile(stock_params['symbol'])
    if company_profile.name
      @stock.company_name = company_profile.name
      @stock.exchange = company_profile.exchange
    else
      @stock.company_name = "ETF"
      @stock.exchange = "Unknown"
    end
    
    if @stock.save
      redirect_to action: 'index'
    else
      render :new, status: :unprocessable_entity
    end
  end

  def destroy
    @stock = Stock.find(params[:id])
    @stock.destroy

    redirect_to root_path, status: :see_other
  end

  def move_to_top
    @stock = Stock.find(params[:id])
    @stock.move_to_top
    if @stock.save
      redirect_to action: 'index'
    end
  end
  
  private
  def stock_params
    params.require(:stock).permit(:symbol)
  end

  private
  def stock_list
    stock_list = StockList.first
    if not stock_list
      stock_list = StockList.new(name: 'Default List')
      stock_list.save
    end
    stock_list
  end

end
