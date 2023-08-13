#include "GameScript.hpp"   // Подключение класса игры

int main(){
    Game MainGame;          // Главный класс
    MainGame.onlineInit();  // Инициализация онлайн движка
    MainGame.sinit();       // Инициализация окна
    MainGame.run();         // Запуск программы
}