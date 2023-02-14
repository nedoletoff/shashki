Реализация русских шашек

Сборка:
установить все из requirements.txt

cd ../model  

gcc -c c_code/coordinates_list.c -o c_code/coordinates_list.o

gcc -c c_code/board.c -o c_code/board.o

gcc -c c_code/ai.c -o c_code/ai.o

gcc -shared -o board.dll c_code/board.o c_code/coordinates_list.o c_code/ai.o

cd ..

Запуск 
из shashki запустить python view/game.py

