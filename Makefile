
#在执行./main之前先在终端执行以下命令，在执行的时候 去这个路径链接libpython3.5m.so
#export LD_LIBRARY_PATH=/home/bingluo/anaconda3/envs/py3.5/lib:$LD_LIBRARY_PATH

INC=/home/bingluo/anaconda3/envs/py3.5/include/python3.5m
LIB=/home/bingluo/anaconda3/envs/py3.5/lib/

main:
	g++ -o main  main.cpp -I$(INC)/ -L$(LIB)  -lpython3.5m

clean:
	rm -rf main.o main
