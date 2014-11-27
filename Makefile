#TARFILES = Makefile scanner.mll parser.mly ast.mli jo.ml
#	$(TESTS:%=tests/test-%.mc) \
#        $(TESTS:%=tests/test-%.out)

OBJS = parser.cmo scanner.cmo symboltable.cmo typecheck.cmo  jo.cmo

.PHONY: default
default: jo

.PHONY: all
all: clean jo

jo: $(OBJS)
	ocamlc -o jo $(OBJS)

scanner.ml: scanner.mll
	ocamllex scanner.mll

parser.ml parser.mli: parser.mly
	ocamlyacc parser.mly

%.cmo: %.ml
	ocamlc -c $<

%.cmi: %.mli
	ocamlc -c $<

#calculator.tar.gz : $(TARFILES)
#	cd .. && tar zcf fdl/fdl.tar.gz $(TARFILES:%=jo/%)

.PHONY: clean
clean:
	rm -rf jo parser.ml parser.mli scanner.ml *.cmo *.cmi *.log *.o *~ a.out* core

# Generated by ocamldep *.ml *.mli
typecheck.cmo: symboltable.cmo ast.cmi sast.cmi
typecheck.cmx: symboltable.cmx ast.cmx sast.cmx
jo.cmo: scanner.cmo parser.cmi ast.cmi Str.cma
jo.cmx: scanner.cmx parser.cmx ast.cmi
parser.cmo: ast.cmi parser.cmi
parser.cmx: ast.cmi parser.cmi
scanner.cmo: parser.cmi
scanner.cmx: parser.cmx
parser.cmi: ast.cmi
Str.cma:
