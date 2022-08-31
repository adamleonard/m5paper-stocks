Rails.application.routes.draw do
  root "stocks#index"

  resources :stocks do
    member do
      get 'move_to_top'
    end
  end

end
