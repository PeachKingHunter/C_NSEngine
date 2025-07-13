all: main

obj = obj/

# Compile Libraries
$(obj)Parser.o: Parser/Parser.c Parser/Parser.h
	gcc -Wall -c -o $@ $<

$(obj)BigNumberManager.o: BigNumberManager/BigNumberManager.c BigNumberManager/BigNumberManager.h
	gcc -Wall -c -o $@ $<


$(obj)Hachage.o: Hachage/Hachage.c Hachage/Hachage.h
	gcc -Wall -c -o $@ $<


# Compile project files
$(obj)ButtonLC.o: src/ButtonLC.c src/ButtonLC.h
	gcc -Wall -c -o $@ $<

$(obj)TextLabelLC.o: src/TextLabelLC.c src/TextLabelLC.h
	gcc -Wall -c -o $@ $<

$(obj)WindowLC.o: src/WindowLC.c src/WindowLC.h
	gcc -Wall -c -o $@ $<

$(obj)GameStruct.o: src/GameStruct.c src/GameStruct.h
	gcc -Wall -c -o $@ $<

$(obj)CodeInterpretor.o: src/CodeInterpretor.c src/CodeInterpretor.h
	gcc -Wall -c -o $@ $<

$(obj)Render.o: src/Render.c src/Render.h
	gcc -Wall -c -o $@ $<


# Create and compile the main
$(obj)GamePackages.o: $(obj)GameStruct.o $(obj)CodeInterpretor.o $(obj)WindowLC.o $(obj)TextLabelLC.o $(obj)ButtonLC.o $(obj)Render.o
	ld -r -o $@ $^

main: main.c $(obj)Parser.o  $(obj)Hachage.o $(obj)BigNumberManager.o $(obj)GamePackages.o
	gcc -Wall -o $@ $^ -lSDL3 -lSDL3_ttf


# Running commands
run: $(obj)GameStruct.o $(obj)CodeInterpretor.o $(obj)WindowLC.o $(obj)TextLabelLC.o $(obj)ButtonLC.o $(obj)Render.o main
	./main

runVerif: main
	valgrind --suppressions=SDL3_Errors.supp --leak-check=full --show-leak-kinds=all ./main

runGenSupp: main
	valgrind --gen-suppressions=all --leak-check=full --show-leak-kinds=all ./main 2> valgrind_output.log
	sed -n '/^{/,/^}/p' valgrind_output.log > SDL3_Errors.supp

clean:
	rm obj/*
