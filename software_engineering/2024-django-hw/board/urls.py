from django.urls import path, include
import board.views as views

urlpatterns = [
    path('startup', views.startup),
    path('login', views.login),
    path('boards', views.boards),
    path('boards/<index>',views.boards_index),
    path('user/<userName>',views.user_username)

]
