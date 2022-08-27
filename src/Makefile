myprintf: myprintf.o	# $@:目标文件; %^:所有依赖文件; $<:第一个依赖文件
	gcc $< -o $@

myprintf.o: myprintf.c
	gcc -c $< -o $@

clean:
	rm *.o myprintf -rf
