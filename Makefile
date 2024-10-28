OBJS = build/obj/main.o \
       build/obj/tasks.o \
       build/obj/tokenizer/task.o \
       build/obj/tokenizer/tokenize.o \
       build/obj/tokenizer/char_ext.o \
       build/obj/tokenizer/builders.o \
       build/obj/lexer/task.o \
       build/obj/lexer/projects/modules/scopes/load.o \
       build/obj/lexer/projects/modules/scopes/defs/parse.o \
       build/obj/lexer/projects/modules/scopes/defs/contents/func/parse.o \
       build/obj/core/arr.o \
       build/obj/core/str.o

link: build-obj
	gcc $(OBJS) -o build/cee

build-obj: clean $(OBJS)

build/obj/%.o: src/%.c
	@mkdir -p `dirname $@`
	gcc -c $^ -o $@ -I src

clean:
	rm -Rf build
