all: main


# To Change for your OS
# For linux
cc = gcc 
# For windows
#cc = i686-w64-mingw32-gcc  


lib = -LSDL3/lib 
include = -ISDL3/include

obj = target/obj/

# Compile Libraries
$(obj)Parser.o: Parser/Parser.c Parser/Parser.h
	$(cc)  -Wall -c -o $@ $<

$(obj)BigNumberManager.o: BigNumberManager/BigNumberManager.c BigNumberManager/BigNumberManager.h
	$(cc) -Wall -c -o $@ $<

$(obj)Hachage.o: Hachage/Hachage.c Hachage/Hachage.h
	$(cc) -Wall -c -o $@ $<

# Compile project files
$(obj)ButtonLC.o: src/ButtonLC.c src/ButtonLC.h
	$(cc) -Wall -c -o $@ $< $(lib) $(include)

$(obj)TextLabelLC.o: src/TextLabelLC.c src/TextLabelLC.h
	$(cc) -Wall -c -o $@ $< $(lib) $(include)

$(obj)WindowLC.o: src/WindowLC.c src/WindowLC.h
	$(cc) -Wall -c -o $@ $< $(lib) $(include)

$(obj)GameStruct.o: src/GameStruct.c src/GameStruct.h
	$(cc) -Wall -c -o $@ $< $(lib) $(include)

$(obj)CodeInterpretor.o: src/CodeInterpretor.c src/CodeInterpretor.h
	$(cc) -Wall -c -o $@ $< $(lib) $(include)

$(obj)Render.o: src/Render.c src/Render.h
	$(cc) -Wall -c -o $@ $< $(lib) $(include)


# Create and compile the main
$(obj)GamePackages.o: $(obj)GameStruct.o $(obj)CodeInterpretor.o $(obj)WindowLC.o $(obj)TextLabelLC.o $(obj)ButtonLC.o $(obj)Render.o
	$(cc) -r -o $@ $^

main: main.c $(obj)Parser.o  $(obj)Hachage.o $(obj)BigNumberManager.o $(obj)GamePackages.o
	$(cc) -Wall -o target/$@ $^ -lSDL3 -lSDL3_ttf -lSDL3_image $(lib) $(include)


# For execute juste open the main or main.exe

# Compile Running && Tests commands (configured for linux because I use linux)
run: $(obj)GameStruct.o $(obj)CodeInterpretor.o $(obj)WindowLC.o $(obj)TextLabelLC.o $(obj)ButtonLC.o $(obj)Render.o main
	./target/main

runVerif: main
	valgrind --suppressions=SDL3_Errors.supp --leak-check=full --show-leak-kinds=all ./target/main

runGenSupp: main
	valgrind --gen-suppressions=all --leak-check=full --show-leak-kinds=all ./target/main 2> valgrind_output.log
	sed -n '/^{/,/^}/p' valgrind_output.log > SDL3_Errors.supp
	rm valgrind_output.log

clean:
	rm target/obj/*
	rm target/main
	rm target/main.exe
