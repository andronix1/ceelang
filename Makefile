OBJS = build/obj/main.o \
       build/obj/tasks.o \
       build/obj/core/arr.o \
       build/obj/core/slice.o \
       build/obj/core/str.o \
       build/obj/base/messages.o \
       build/obj/base/result.o \
       build/obj/tokenizer/readers/reader.o \
       build/obj/tokenizer/readers/uint.o \
       build/obj/tokenizer/readers/ident.o \
       build/obj/tokenizer/tokens.o \
       build/obj/tokenizer/tokenize.o \
       build/obj/tokenizer/task.o

link: build-obj
	gcc $(OBJS) -o build/cee

build-obj: clean $(OBJS)

build/obj/%.o: src/%.c
	@mkdir -p `dirname $@`
	gcc -c $^ -o $@ -I src

clean:
	rm -Rf build
