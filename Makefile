OBJS = build/obj/main.o \
       build/obj/tasks.o \
       build/obj/core/arr.o \
       build/obj/core/slice.o \
       build/obj/core/str.o \
       build/obj/base/messages.o \
       build/obj/base/result.o \
       build/obj/base/reader.o \
       build/obj/base/token_reader.o \
       build/obj/tokenizer/readers/uint.o \
       build/obj/tokenizer/readers/ident.o \
       build/obj/tokenizer/readers/str.o \
       build/obj/tokenizer/tokens.o \
       build/obj/tokenizer/tokenize.o \
       build/obj/tokenizer/task.o \
       build/obj/lexer/modules/module.o \
       build/obj/lexer/modules/scopes/load.o \
       build/obj/lexer/modules/scopes/scope.o \
       build/obj/lexer/modules/scopes/funcall/funcall.o \
       build/obj/lexer/modules/scopes/funcall/parse.o \
       build/obj/lexer/modules/scopes/exprs/expr.o \
       build/obj/lexer/modules/scopes/exprs/raw_expr.o \
       build/obj/lexer/modules/scopes/exprs/parse.o \
       build/obj/lexer/modules/scopes/exprs/readers/ident.o \
       build/obj/lexer/modules/scopes/exprs/readers/operator.o \
       build/obj/lexer/modules/scopes/exprs/readers/scope.o \
       build/obj/lexer/modules/scopes/exprs/readers/int.o \
       build/obj/lexer/modules/scopes/exprs/readers/str.o \
       build/obj/lexer/modules/scopes/exprs/readers/fun.o \
       build/obj/lexer/modules/scopes/defs/def.o \
       build/obj/lexer/modules/scopes/defs/parse.o \
       build/obj/lexer/modules/scopes/defs/contents/content.o \
       build/obj/lexer/modules/scopes/defs/contents/func/parse.o \
       build/obj/lexer/modules/scopes/defs/contents/func/args.o \
       build/obj/lexer/modules/scopes/defs/contents/func/stats/stat.o \
       build/obj/lexer/modules/scopes/defs/contents/func/stats/parse.o \
       build/obj/lexer/modules/scopes/defs/contents/func/stats/readers/return.o \
       build/obj/lexer/modules/scopes/defs/contents/func/stats/readers/funcall.o \
       build/obj/lexer/task.o

link: build-obj
	gcc $(OBJS) -o build/cee

build-obj: clean $(OBJS)

build/obj/%.o: src/%.c
	@mkdir -p `dirname $@`
	gcc -c $^ -o $@ -I src

clean:
	rm -Rf build
