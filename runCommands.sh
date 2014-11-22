#!/bin/bash
ocamllex scanner.mll
ocamlyacc parser.mly
ocamlc -c ast.mli
ocamlc -c parser.mli
ocalmc -c scanner.ml
ocamlc -c parser.ml
ocamlc -c sast.mli 
ocamlc -c symboltable.ml
ocamlc -c typecheck.ml
ocamlc -c jo.ml
ocamlc -o jo jo.cmo  parser.cmo  scanner.cmo sast.cmi symboltable.cmo  typecheck.cmo

