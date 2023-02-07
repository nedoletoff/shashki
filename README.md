Реализация русских шашек

Сборка:
установить все из requirements.txt

cd ../model  
gcc -c c_code/ai.c  
gcc -shared -o ai.dll c_code/ai.o  

gcc -o c_code/coordinates_list.c
c_code/board.c c_code/boards_list.c
gcc -shared -o board.dll c_code/board.o
c_code/board_list.o c_code/coordinates_list.o

Запуск 
из shashki запустить python view/game.py

