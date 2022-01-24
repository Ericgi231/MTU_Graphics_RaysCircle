ray2: main.c stb_image_write.h
	gcc -g -Wall -Wextra -std=gnu99 -o ray2 main.c -lm

.PHONY: clean
clean:
	rm -f ray2 sphere.png