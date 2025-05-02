cooke_parser: front.c front.h parser.h
	gcc -o cooke_parser front.c front.h parser.h

clean:
	rm -f cooke_parser
