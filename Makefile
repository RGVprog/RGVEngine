CC = gcc
CFLAGS = -lGL -ldl -lm -lglfw -lassimp -lX11 -I include/

main.o: main.c
	$(CC) -c main.c -o bin/main.o $(CFLAGS)

glad.o: src/glad.c
	$(CC) -c src/glad.c -o bin/glad.o $(CFLAGS)

window.o: window.c
	$(CC) -c window.c -o bin/window.o $(CFLAGS)

shader.o: shader.c
	$(CC) -c shader.c -o bin/shader.o $(CFLAGS)

mesh.o: mesh.c
	$(CC) -c mesh.c -o bin/mesh.o $(CFLAGS)

rgvmath.o: rgvmath.c
	$(CC) -c rgvmath.c -o bin/rgvmath.o $(CFLAGS)

model.o: model.c
	$(CC) -c model.c -o bin/model.o $(CFLAGS)

d_array.o: src/d_array.c
	$(CC) -c src/d_array.c -o bin/d_array.o $(CFLAGS)


final: main.o glad.o window.o shader.o mesh.o rgvmath.o model.o d_array.o
	$(CC) -o prog bin/main.o bin/glad.o bin/window.o bin/shader.o bin/mesh.o bin/rgvmath.o bin/model.o bin/d_array.o $(CFLAGS)

clean: 
	rm bin/main.o bin/glad.o bin/window.o bin/shader.o bin/mesh.o bin/rgvmath.o bin/model.o bin/d_array.o
